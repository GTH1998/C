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

#include "../../../../../kernel/osal/include/osal_errno.h"
#include "../../../../../kernel/osal/include/msgqueue/osal_msgqueue.h"

#include "../../../../../drivers/drivers/hal/pwm/hal_pwm.h"
#include "../../../../../kernel/liteos/liteos_v208.6.0_b017/Huawei_LiteOS/kernel/include/los_typedef.h"
#include "../../../../../../../kernel/liteos/liteos_v208.6.0_b017/Huawei_LiteOS/open_source/musl/include/stdarg.h"
#include "../../../../../../../kernel\osal\include\memory\osal_addr.h"
// #include "main_data_stuct.h"
// #include "BSP_ADC_config.h"
#include "BSP_KEY.h"



bsp_key_handle_link_t *key_link_list = NULL;


uint8_t key_status = 0;
uint8_t key_cnt = 0;
uint32_t key_val = 0;
uint8_t key_press = 0;

uint32_t key_load(void);
bool key_init_flag = false;
extern uint8_t key_interrupt_running(void);
bsp_key_handle_t *key_1 = NULL;
bsp_key_handle_t *key_2 = NULL;
bsp_key_handle_t *key_3 = NULL;
bsp_key_handle_t *key_4 = NULL;
bsp_key_handle_t *key_5 = NULL;



extern void key_timer_handle(unsigned long param);


osal_timer key_timer_struct = 
{
    .data = 0,
    .handler = key_timer_handle, 
    .interval = 1,
    .timer = NULL,
};

//set this function in timer interrupt or sort timer
void key_timer_handle(unsigned long param)
{
    key_interrupt_running();
    osal_timer_start(&key_timer_struct);
}

void key_timer_init(void)
{
    if(key_init_flag != true)
    {
        key_init_flag = true;
        int ret = osal_timer_init(&key_timer_struct);
        if (ret != OSAL_SUCCESS) {
            osal_printk("g_mouse_pwm_init create failed! %d\n",ret);
        }
    }
    osal_timer_start(&key_timer_struct);
}

// void key_scan_process(void)
// {
//     static uint8_t tick_timer = 0;
//     uint32_t key_temp = key_load(); 
//     switch(key_status)
//     {
//         case EN_KEY_WAIT_STATUS:
//             if(key_temp != 0x00)
//             {
//                 if(++tick_timer > SHAKE_WAIT_TIME)
//                 {
//                     key_status = EN_KEY_PRESS_STATUS;
//                     tick_timer = 0;
//                 }
//             }
//             else
//             {
//                 tick_timer = 0;
//             }
//             break;
//         case EN_KEY_PRESS_STATUS:
//             if(key_temp != 0x00)
//             {
//                 key_val = key_temp;
//                 key_status = EN_KEY_RELEASE_STATUS;
//             }
//             else
//             {   
//                 key_status = EN_KEY_WAIT_STATUS;
//                 tick_timer = 0;
//             }
//             break;
//         case EN_KEY_RELEASE_STATUS:
//             if(0x00 != key_temp)
//             {
//                 if(key_temp != key_val)
//                 {
//                     tick_timer ++;
//                     if(tick_timer > MIN_PRESS_TIME)
//                         key_val |= key_temp;

//                 }
//                 if(++key_cnt >= LONG_PRESS_TIME)
//                 {
//                     key_cnt = 0;
//                     key_press = LONG_PRESS;
//                     key_status = EN_KEY_IDEL_STATUS;
//                 }
//             }
//             else
//             {
//                 key_press = SHORT_PRESS;
//                 key_status = EN_KEY_WAIT_STATUS;
//             }
//             break;
//         case EN_KEY_IDEL_STATUS:
//             if(0x00 == key_temp)
//             {
//                 tick_timer = 0;
//                 key_status = EN_KEY_WAIT_STATUS;
//             }
//             break;
//         default:
//             break;
//     }
// }


/***********************************************************************************************
 * @brief : key can input pin value
 * @param : uint32_t pin. pin input
 * @return: data struct key_level_t;
***********************************************************************************************/
void key_pin_gpio_init(key_pin_define_t param)
{

}

/***********************************************************************************************
 * @brief : key can input pin value
 * @param : uint32_t pin. pin input
 * @return: data struct key_level_t;
***********************************************************************************************/

key_level_t key_scan_pin_value(uint32_t pin)
{
    return uapi_gpio_get_val(pin);
}
/******************************************************************************************************
 * @brief  :function user key init
 * @param  :param ;data struct @bsp_key_handle_t
 * @return :bsp_key_handle_t struct point; 
******************************************************************************************************/
bsp_key_handle_t* key_handle_init(bsp_key_handle_t param)
{
    static uint8_t handle = 0;
    bsp_key_handle_link_t *new_node = (bsp_key_handle_link_t *)malloc(sizeof(bsp_key_handle_link_t));
    if(NULL == new_node)
        return 0;
    key_pin_gpio_init(param.pin_setting);
    handle ++;
    new_node->key_mode = param;
    new_node->key_mode.handle = handle; 
    if(NULL == key_link_list)
    {
        new_node->next = key_link_list;
        key_link_list = new_node;
        return &(new_node->key_mode);
    }
    bsp_key_handle_link_t *temp = key_link_list;
    while(temp->next)
    {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->next = NULL;
    return &(new_node->key_mode);
}

/************************************************************************************************************
 * @brief:  key stop scan 
 * @param:  *param , bsp_key_handle_t data struct
 * @return: 1: param is NULL; 0: stop key ok
************************************************************************************************************/
uint8_t key_stop_scan(bsp_key_handle_t *param)
{
    if(NULL == param)
    {
        return 1;
    }
    param->key_enable = false;
    return 0;
}


/************************************************************************************************************
 * @brief:  key start scan 
 * @param:  *param , bsp_key_handle_t data struct
 * @return: 1: param is NULL; 0: stop key ok
************************************************************************************************************/
uint8_t key_start_scan(bsp_key_handle_t *param)
{
    if(NULL == param)
    {
        return 1;
    }
    param->key_enable = true;
    return 0;
}


// key exemple code
void gth_key_test(void)
{
    bsp_key_handle_t key1_init_t = 
    {
        .handle = 0,
        .key_enable = true, 
        .pin_setting.GPIO_PIN = 2,
    };
    key_1 = key_handle_init(key1_init_t);

    bsp_key_handle_t key2_init_t = 
    {
        .handle = 0,
        .key_enable = true, 
        .pin_setting.GPIO_PIN = 3,
    };
    key_2 = key_handle_init(key2_init_t);

    bsp_key_handle_t key3_init_t = 
    {
        .handle = 0,
        .key_enable = true, 
        .pin_setting.GPIO_PIN = 4,
    };
    key_3 = key_handle_init(key3_init_t);

    bsp_key_handle_t key4_init_t = 
    {
        .handle = 0,
        .key_enable = true, 
        .pin_setting.GPIO_PIN = 5,
    };
    key_4 = key_handle_init(key4_init_t);

      bsp_key_handle_t key5_init_t = 
    {
        .handle = 0,
        .key_enable = true, 
        .pin_setting.GPIO_PIN = 6,
    };
    key_5 = key_handle_init(key5_init_t);
    key_timer_init();

}


// this function is BSP key scan function 
// param : is key scan link list
void key_scan_process_f(bsp_key_handle_t *param)
{
    key_level_t temp_key_value = key_scan_pin_value(param->pin_setting.GPIO_PIN);
    switch(param->key_mode.key_status)
    {
        case EN_KEY_WAIT_STATUS:
            if(KEY_LEVEL_LOW == temp_key_value)
            {
                if(++param->key_mode.key_running_time > SHAKE_WAIT_TIME)
                {
                    param->key_mode.key_status = EN_KEY_PRESS_STATUS;
                    param->key_mode.key_running_time = 0;
                }
            }
            else
            {
                param->key_mode.key_running_time = 0;
            }
            break;
        case EN_KEY_PRESS_STATUS:
            if(temp_key_value == KEY_LEVEL_LOW)
            {
                param->key_mode.key_status = EN_KEY_RELEASE_STATUS;
            }
            else
            {
                param->key_mode.key_status = EN_KEY_WAIT_STATUS;
            }
            break;
        case EN_KEY_RELEASE_STATUS:
            if(temp_key_value == KEY_LEVEL_LOW)
            {
                if(++param->key_mode.key_cnt > LONG_PRESS_TIME)
                {
                    param->key_mode.key_cnt = 0;
                    param->key_mode.key_press_mode = EN_KEY_LONG_PRESS;
                    param->key_mode.key_status = EN_KEY_IDEL_STATUS;
                }
            }
            else
            {
                param->key_mode.key_cnt = 0;
                param->key_mode.key_press_mode = EN_KEY_SHORT_PRESS;
                param->key_mode.key_status     = EN_KEY_DOUBLE_STATUS;
            }

            break;
        case EN_KEY_DOUBLE_STATUS:
            
            if(++param->key_mode.key_running_time > 500)
            {
                param->key_mode.key_running_time = 0;
                if(KEY_LEVEL_LOW == temp_key_value)
                {
                    param->key_mode.key_status = EN_KEY_IDEL_STATUS;
                }
                else
                {
                     param->key_mode.key_status = EN_KEY_IDEL_STATUS;
                }
            }
        
            break;
        case EN_KEY_IDEL_STATUS:
            if(temp_key_value == GPIO_LEVEL_HIGH)
            {
                 param->key_mode.key_running_time = 0;
                 param->key_mode.key_status = EN_KEY_WAIT_STATUS;
            }
            break;
    }
}
/***********************************************************************************************
 * @brief :key scan function init down, this function set the timer interrupt os sort tiemr
 * 
 * @return: 1: key init is NULL
************************************************************************************************/
uint8_t key_interrupt_running(void)
{
    if(NULL == key_link_list)
    {
        return 1;
    }
    bsp_key_handle_link_t *temp = key_link_list; 
    while(temp)
    {
        if(temp->key_mode.key_enable == true)
        {
            key_scan_process_f(&(*temp).key_mode);
        }
        temp = temp->next;
    }    
}



