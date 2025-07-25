#include "soc_osal.h"
#include "osal_debug.h"
#include "platform_types.h"
#include "gpio.h"
#include "hal_gpio.h"
#include "pinctrl.h"
#include "tcxo.h"
#include "hal_pwm.h"
#include "hal_pwm_v151.h"
#include "osal_timer.h"
// #include "../byt_peripheral/byt_peripheral.h"
#include "cmsis_os2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "TMT_task_process.h"


static TMT_data_struct_t *global_TMT_task_link_list = NULL;

osal_timer tmt_task_timer = {0};
uint8_t task_1_handle = 0;
uint8_t task_2_handle = 0;
uint8_t task_3_handle = 0;
uint8_t task_4_handle = 0;

TMT_task_func_t *task_func = NULL;

/***************************************************************
 * @brief new tmt task link list node
 * @param NULL
 * @return TMT_data_struct_t data struct point
 * 
 ***************************************************************/
static TMT_data_struct_t *new_tmt_task_node(void)
{
    TMT_data_struct_t *new_node = (TMT_data_struct_t *)malloc(sizeof(TMT_data_struct_t));
    if(NULL == new_node)
    {
        printf("new_tmt new node is NULL\n");
        return NULL;
    }
    else
    {
        printf("new tmt new node is OK\n");
    }
    return (TMT_data_struct_t *)new_node;
}

/******************************************************************************************************************************
 * @brief create tmt task 
 * @param TMT_task_attr_t data struct
 * @return TMT_TSAK_HANDLE ;0xFF: create task error. other: create task success;
 ******************************************************************************************************************************/
static TMT_TASK_HANDLE tmt_task_create(TMT_task_attr_t task)
{
    static uint8_t handle = 0;
    TMT_data_struct_t *temp = global_TMT_task_link_list;
    TMT_data_struct_t *temp_loop = temp;
    TMT_data_struct_t *new_node = NULL; 

    handle ++;
    new_node = new_tmt_task_node();
    if(NULL == new_node)
        return 0xFF;
    new_node->Handle = handle;
    new_node->task_resume = false;
    new_node->tick_config  = task.tick_config;
    new_node->tick_count = 0;
    new_node->next = NULL;
    new_node->task = task.task;
    if(NULL == temp)
    {
        new_node->next = global_TMT_task_link_list;
        global_TMT_task_link_list = new_node;
        return handle;
    }
    while(temp_loop != NULL)
    {
        temp = temp_loop;
        temp_loop = temp_loop->next;
    }
    temp->next = new_node;
    return handle;
}

/***************************************************************
 * @brief delete tmt task 
 * @param handle：create task record handle 
 * @return ERR_RET 0:delete success; other: delete error
 ***************************************************************/
static ERR_RET tmt_task_delete(uint8_t handle)
{
    if(NULL == global_TMT_task_link_list)
    {
        printf("tmt task is NULL, delete failed\n");
        return 1;
    }
    TMT_data_struct_t *temp = global_TMT_task_link_list;
    TMT_data_struct_t *temp_loop = temp;
    if(temp->next == NULL)
    {
        free(temp);
        global_TMT_task_link_list = NULL;
        return 1;
    }
    while(temp_loop->Handle != handle)
    {
        temp = temp_loop;
        temp_loop = temp_loop->next;
        if(temp_loop == NULL)
            return 2;
    }
    if(temp_loop->next == NULL)
    {
        free(temp_loop);
        temp->next = NULL;
        return 0;
    }
    temp->next = temp_loop->next;
    free(temp_loop);
    return 0;
}

/***************************************************************
 * @brief start tmt task 
 * @param handle：create task record handle 
 * @return ERR_RET 0:start task success; other:start task error
 ***************************************************************/
static ERR_RET tmt_task_start(uint8_t handle)
{
    if(NULL == global_TMT_task_link_list)
    {
        printf("tmt task is NULL, start failed\n");
        return 1;
    }
    TMT_data_struct_t *temp = global_TMT_task_link_list;
    TMT_data_struct_t *temp_loop = temp;

    while(temp_loop->Handle != handle)
    {
        temp = temp_loop;
        temp_loop = temp_loop->next;
        if(temp_loop == NULL)
            return 2;
    }
    printf("tmt start success: %d\n",temp_loop->Handle);
    temp_loop->tick_count = 0;
    temp_loop->task_resume = true;
    temp_loop->task_run    = false;;
    return 0;
}

/***************************************************************
 * @brief stop tmt task 
 * @param handle：create task record handle 
 * @return ERR_RET 0:start task success; other:start task error
 ***************************************************************/
static ERR_RET tmt_task_stop(uint8_t handle)
{
    if(NULL == global_TMT_task_link_list)
    {
        printf("tmt task is NULL,stop failed\n");
        return 1;
    }
    printf("input handle : %d\n", handle);
    TMT_data_struct_t *temp = global_TMT_task_link_list;
    TMT_data_struct_t *temp_loop = temp;
    while(temp_loop->Handle != handle)
    {
        temp = temp_loop;
        temp_loop = temp_loop->next;
        printf("%d \n",temp_loop->Handle);
        if(NULL == temp_loop)
            return 2;
    }
    printf("tmt stop success: %d\n",temp_loop->Handle);
    temp_loop->task_resume = false;
    temp_loop->tick_count = 0;
    return 0;
}


/***************************************************************
 * @brief tmt task tick: user timer interrupt execupt tick add 1
 * @param NULL
 * @return ERR_RET return nor user
 ***************************************************************/
static ERR_RET tmt_task_tick(void)
{
    if(NULL != global_TMT_task_link_list)
    {
        TMT_data_struct_t *temp = global_TMT_task_link_list;
        while(temp != NULL)
        {
            // printf("handle: %d\n", temp->Handle);
            if(temp->task_resume == true)
            {
                temp->tick_count ++;
                if(temp->tick_count == temp->tick_config)
                {
                    temp->tick_count = 0;
#if TASK_RUNNING_IN_MAIN_FUNCTION
                    temp->task_run = true;
#else
                //     printf("func running\n");
                    temp->task(); 
                // }
#endif
                }
            }
            temp = temp->next;
        }
    }
}


/***************************************************************
 * @brief tmt task running function, task tick = setting tick
 *        function is running 
 * @param NULL
 * @return return nor user
 ***************************************************************/
static ERR_RET tmt_task_running(void)
{
    if(NULL != global_TMT_task_link_list)
    {
        TMT_data_struct_t *temp = global_TMT_task_link_list;
        while(temp)
        {
            if(temp->task_resume == true)
            {
                if(temp->task_run == true)
                {
                    temp->task_run = false;
                    temp->task();
                }
                // temp->
            }
            temp = temp->next;
        }
    }
}

/******************************************************************************************************************************
 * @brief tmt task change time
 * @param handle:  tmt create record handle
 * @param count:   set task tick count
 * @return ERR_RET: 0:change task tick count success; other: error
 ******************************************************************************************************************************/
static ERR_RET  tmt_task_change_running_timer(uint8_t handle, uint32_t count)
{
    bool task_running_flag = false;
    TMT_data_struct_t *temp = global_TMT_task_link_list;
    while(temp)
    {
        if(temp->Handle == handle)
        {
            if(temp->task_resume == true)
            {
                temp->task_resume = false;
                task_running_flag = true;
            }
            temp->tick_count = 0;
            temp->tick_config = count;
            if(task_running_flag == true)
            {
                temp->task_resume = true;
            }
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}
static TMT_task_func_t TMT_task_manger = 
{
    .tmt_task_create     = tmt_task_create,
    .tmt_task_delete     = tmt_task_delete,
    .tmt_task_start      = tmt_task_start,
    .tmt_task_stop       = tmt_task_stop,
    .tmt_task_tick       = tmt_task_tick,
#if TASK_RUNNING_IN_MAIN_FUNCTION
    .tmt_task_running    = tmt_task_running,
#endif
    .tmt_task_change_running_time = tmt_task_change_running_timer
};


/******************************************************************************************************************************
 * @brief all tmt task func;expose other function user
 * @param : NULL
 * @return TMT_task_func_t data struct tmt task function point
 ******************************************************************************************************************************/
TMT_task_func_t *g_tmt_task_func(void)
{
    TMT_data_struct_t *p = &TMT_task_manger;
    return (TMT_data_struct_t *)p;
}
void tmt_timer_interrupt_cb(unsigned long arg)
{
    // printf("tmt timer running\n");
    uint8_t i = 0;
    // if(NULL != global_TMT_task_link_list)
    // {
    //     TMT_data_struct_t *temp = global_TMT_task_link_list;
    //     while(temp != NULL)
    //     {
    //         i++;
    //         // printf("handle: %d\n", temp->Handle);
    //         if(temp->task_resume == true)
    //         {
    //             temp->tick_count ++;
    //             if(temp->tick_count == temp->tick_config)
    //             {
    //                 printf("func running\n");
    //                 temp->task(); 
    //                 temp->tick_count = 0;
    //                 // if(temp->Handle == task_3_handle)
    //                 // {
    //                 //     task_func->tmt_task_delete(task_3_handle);
    //                 // }
    //             }
    //         }
    //         // printf("p1\n");
    //         temp = temp->next;
    //         // printf("p2\n");

    //     }
    // }
    task_func->tmt_task_tick();
    osal_timer_start(&tmt_task_timer);
}

void tmt_timer_interrupt_create(void)
{
    errcode_t ret;
    tmt_task_timer.timer = NULL; 
    tmt_task_timer.data = 0; 
    // tmt_task_timer.handler = flowing_lamp_interrupt_cb; /* 回调函数 */ 
    tmt_task_timer.handler = tmt_timer_interrupt_cb;
    tmt_task_timer.interval = 10; /* 500ms */ 
    ret = osal_timer_init(&tmt_task_timer); 
    if (ret != OSAL_SUCCESS) { 
        osal_printk("tmt_timer_interrupt_create create failed!\r\n"); 
    }
    osal_timer_start(&tmt_task_timer);
}

void func_1_test(void)
{
    printf("func 1 running\n");
}
void func_2_test(void)
{
    printf("func 2 running\n");
}
void func_3_test(void)
{
    printf("func 3 running\n");
}
void func_4_test(void)
{
    printf("func 4 running\n");
}
void tmt_task_test_func(void)
{
    tmt_timer_interrupt_create();
    task_func = g_tmt_task_func();
    TMT_task_attr_t func_1 = {
        .Handle = 0,
        .task = func_1_test,
        .task_resume = true,
        .tick_config = 100,
    };
    printf("F1\n");
    task_1_handle = task_func->tmt_task_create(func_1);
    printf("F2\n");

    TMT_task_attr_t func_2 = {
    .Handle = 0,
    .task = func_2_test,
    .task_resume = true,
    .tick_config = 300,
    };
    task_2_handle = task_func->tmt_task_create(func_2);
    printf("F3\n");

     TMT_task_attr_t func_3 = {
    .Handle = 0,
    .task = func_3_test,
    .task_resume = true,
    .tick_config = 500,
    };
    task_3_handle = task_func->tmt_task_create(func_3);
    printf("tmt handle:%d %d %d",task_1_handle, task_2_handle, task_3_handle);

    task_func->tmt_task_start(task_1_handle);
    task_func->tmt_task_start(task_2_handle);
    task_func->tmt_task_start(task_3_handle);

}


