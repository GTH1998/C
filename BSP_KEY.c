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
#include "main_data_stuct.h"
#include "BSP_ADC_config.h"
#include "BSP_KEY.h"





uint8_t key_status = 0;
uint8_t key_cnt = 0;
uint32_t key_val = 0;
uint8_t key_press = 0;
uint32_t key_load(void);


void key_scan_process(void)
{
    static uint8_t tick_timer = 0;
    uint32_t key_temp = key_load(); 
    switch(key_status)
    {
        case EN_KEY_WAIT_STATUS:
            if(key_temp != 0x00)
            {
                if(++tick_timer > SHAKE_WAIT_TIME)
                {
                    key_status = EN_KEY_PRESS_STATUS;
                    tick_timer = 0;
                }
            }
            else
            {
                tick_timer = 0;
            }
            break;
        case EN_KEY_PRESS_STATUS:
            if(key_temp != 0x00)
            {
                key_val = key_temp;
                key_status = EN_KEY_RELEASE_STATUS;
            }
            else
            {   
                key_status = EN_KEY_WAIT_STATUS;
                tick_timer = 0;
            }
            break;
        case EN_KEY_RELEASE_STATUS:
            if(0x00 != key_temp)
            {
                if(key_temp != key_val)
                {
                    tick_timer ++;
                    if(tick_timer > MIN_PRESS_TIME)
                        key_val |= key_temp;

                }
                if(++key_cnt >= LONG_PRESS_TIME)
                {
                    key_cnt = 0;
                    key_press = LONG_PRESS;
                    key_status = EN_KEY_IDEL_STATUS;
                }
            }
            else
            {
                key_press = SHORT_PRESS;
                key_status = EN_KEY_WAIT_STATUS;
            }
            break;
        case EN_KEY_IDEL_STATUS:
            if(0x00 == key_temp)
            {
                tick_timer = 0;
                key_status = EN_KEY_WAIT_STATUS;
            }
            break;
        default:
            break;
    }

}
bsp_key_handle_link_t *key_link_list = NULL;

void key_pin_init(key_pin_define_t param)
{

}

bool key_scan_value(uint32_t pin)
{

}

bsp_key_handle_t* key_handle_init(bsp_key_handle_t param)
{
    static uint8_t handle = 0;
    bsp_key_handle_link_t *new_node = (bsp_key_handle_link_t *)malloc(sizeof(bsp_key_handle_link_t));
    if(NULL == new_node)
        return 0;


    key_pin_init(param.pin_setting);
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

void key_scan_process_f(bsp_key_handle_t *param)
{
    bool temp_key_value = key_scan_value(param->pin_setting.GPIO_PIN);
    switch(param->key_mode.key_status)
    {
        case EN_KEY_WAIT_STATUS:
            if(true == temp_key_value)
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
            if(temp_key_value == true)
            {
                param->key_mode.key_status = EN_KEY_RELEASE_STATUS;
            }
            else
            {
                param->key_mode.key_status = EN_KEY_WAIT_STATUS;
            }
            break;
        case EN_KEY_RELEASE_STATUS:
            if(temp_key_value == true)
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
                param->key_mode.key_status     = EN_KEY_WAIT_STATUS;
            }

            break;
        case EN_KEY_IDEL_STATUS:
            if(temp_key_value == false)
            {
                 param->key_mode.key_running_time = 0;
                 param->key_mode.key_press_mode = EN_KEY_WAIT_STATUS;
            }
            break;
    }
}
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



