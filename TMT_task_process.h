#ifndef __TMT_TEST_PROCESS_H__
#define __TMT_TEST_PROCESS_H__

#define TMT_TASK_HANDLE uint8_t  
#define ERR_RET         uint8_t

#define TASK_RUNNING_IN_MAIN_FUNCTION   0

typedef struct TMT_data_struct
{
    uint8_t Handle;
    uint32_t tick_config;
    uint32_t tick_count;
    bool task_resume;
    bool task_run;
    void (*task)(void);
    struct TMT_data_struct *next;
}TMT_data_struct_t;

typedef struct TMT_task_attr
{
    uint8_t Handle;
    uint32_t tick_config;
    uint32_t tick_count;
    bool task_resume;
    bool task_run;
    void (*task)(void);
}TMT_task_attr_t;


typedef struct TMT_task_func
{
    TMT_TASK_HANDLE (*tmt_task_create)(TMT_task_attr_t task);
    ERR_RET         (*tmt_task_delete)(uint8_t handle);
    ERR_RET         (*tmt_task_start)(uint8_t handle);
    ERR_RET         (*tmt_task_stop)(uint8_t handle);
    ERR_RET         (*tmt_task_tick)(void);
    ERR_RET         (*tmt_task_running)(void);
    ERR_RET         (*tmt_task_change_running_time)(uint8_t handle, uint32_t tick_count);
}TMT_task_func_t;

TMT_task_func_t *g_tmt_task_func(void);

#endif