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

  /* ADC管脚无需配置IE使能且管脚默认IE为0，为防止用户修改IE，特在此将IE配置为0 */
//     uapi_pin_set_ie(adc_pin[channel], PIN_IE_0);
// #endif
//     uapi_pin_set_mode(adc_pin[channel], PIN_MODE_0);
//     uapi_gpio_set_dir(adc_pin[channel], GPIO_DIRECTION_INPUT);
//     uapi_pin_set_pull(adc_pin[channel], PIN_PULL_NONE);
//     uapi_adc_power_en(AFE_GADC_MODE, true);
//     uapi_adc_open_channel(channel);
//     if (self_cali) {
//         adc_calibration(AFE_GADC_MODE, true, true, true);
//     } else {
//         afe_config_t afe_cail_code = { 0 };
//         if (memcmp(&g_afe_cail_code, &afe_cail_code, sizeof(afe_config_t)) == 0) {
//             g_adc_entirely_status = true;
//             return ERRCODE_ADC_INVALID_PARAMETER;
//         }
//         adc_set_cali_code(AFE_GADC_MODE, &g_afe_cail_code);
//     }
bool adc_init_flag =false;
bool adc_timer_start_falg = false;

adc_handle_t adc_channel_1 = 0;
adc_handle_t adc_channel_2 = 0;
adc_handle_t adc_channel_3 = 0;

// adc_handle_t adc_channel_4 = 0;



static hal_adc_config_t* hal_adc_config_param = NULL;


extern void adc_timer_handle(unsigned long param);


osal_timer adc_timer_struct = 
{
    .data = 0,
    .handler = adc_timer_handle,
    .interval = 10,
    .timer = NULL,
};

void adc_timer_handle(unsigned long param)
{
    hal_adc_config_t *temp = hal_adc_config_param;
    int adc_value = 0;
    while(temp)
    {
        if(temp->data.adc_status == true)
        {
            // printf("AC\n");
            temp->data.adc_record_timer ++;
            if(temp->data.adc_sample_timer == temp->data.adc_record_timer)
            {
                temp->data.adc_record_timer = 0;
                {
                    adc_port_gadc_entirely_open(temp->data.adc_channel, true);
                    adc_value = adc_port_gadc_entirely_sample(temp->data.adc_channel);
                    adc_port_gadc_entirely_close(temp->data.adc_channel);
                    printf("C %d,V %d\n",temp->data.adc_handle, adc_value);
                }
            }
            
        }
        temp = temp->next;

    }
    osal_timer_start(&adc_timer_struct);

}





uint8_t check_adc_param_is_invaild(adc_param_t param)
{
    if(param.adc_channel > 8)
    {
        return 0;
    }
    return 1;
}


adc_handle_t BSP_ADC_init(adc_param_t *param)
{
static uint8_t fff_handle = 0;

    if(NULL == param)
    {
        printf("bsp adc init param if NULL\n");
        return RET_ADC_FAILED;
    }
    if(check_adc_param_is_invaild(*param) == 0)
    {
        printf("adc param error\n");
        return RET_ADC_FAILED;
    }
    pin_t adc_pin[] = {S_MGPIO2, S_MGPIO3, S_MGPIO4, S_MGPIO5, S_MGPIO28, S_MGPIO29, S_MGPIO30, S_MGPIO31};
  
    uint8_t channel = (*param).adc_channel;
    uapi_pin_set_ie(adc_pin[channel], PIN_IE_0);
    uapi_pin_set_mode(adc_pin[channel], PIN_MODE_0);
    uapi_gpio_set_dir(adc_pin[channel], GPIO_DIRECTION_INPUT);
    uapi_pin_set_pull(adc_pin[channel], PIN_PULL_NONE);
    uapi_adc_power_en(0, true);
    uapi_adc_open_channel(channel);
    adc_calibration(0, true, true, true);


    fff_handle ++;
    hal_adc_config_t *new_node = (hal_adc_config_t *)malloc(sizeof(hal_adc_config_t));
    memset(new_node, 0, sizeof(hal_adc_config_t));
   
    if(NULL == new_node)
    {
        printf("adc malloc failed\n");
    }
    new_node->data = *param;
    new_node->data.adc_handle = fff_handle;
    if(NULL == hal_adc_config_param)
    {
        new_node->next = hal_adc_config_param;
        hal_adc_config_param = new_node;
        return fff_handle;
    }
    hal_adc_config_t *temp = hal_adc_config_param;
    hal_adc_config_t *temp1 = temp->next;
    while(temp1)
    {
        temp = temp1;
        temp1 = temp1->next;
    }
    temp->next = new_node;
    new_node->next = NULL;

    return fff_handle;
}

void adc_timer_init(void)
{
    if(adc_init_flag != true)
    {
        adc_init_flag = true;
        int ret = osal_timer_init(&adc_timer_struct);
        if (ret != OSAL_SUCCESS) {
            osal_printk("g_mouse_pwm_init create failed! %d\n",ret);
        }
    }
}
void hal_start_adc()
{
    if(adc_timer_start_falg != true)
    {
        adc_timer_start_falg = true;
        osal_timer_start(&adc_timer_struct);
    }
}
void hal_start_adc_timer(adc_handle_t param)
{
    hal_adc_config_t *temp = hal_adc_config_param;
    bool ret = false;
    if(NULL == temp)
        return ;
    while(temp)
    {
        if(temp->data.adc_handle == param)
        {
            // temp->data.adc_status = true;
            ret = true;
            break;
        }
        temp = temp->next;
    }
    if(ret == true)
    {

    }
    
    adc_timer_init();
    hal_start_adc();
}

en_ret_adc hal_adc_start(adc_handle_t handle)
{
    printf("hal_adc_start\n");
    en_ret_adc ret = RET_ADC_FAILED;
    if(NULL == hal_adc_config_param)
    {
        return RET_ADC_FAILED;
    }
    printf("s 1\n");
    hal_adc_config_t *temp = hal_adc_config_param;
    while(temp)
    {
        printf("s 2\n");

        if(handle == temp->data.adc_handle)
        {
            printf("start handle %d。status 1 %d\n",handle, temp->data.adc_status);
            (*temp).data.adc_status = true;
            printf("start status 2: %d\n",temp->data.adc_status);

            ret = RET_ADC_SUCCESS;
            break;
        }
        temp = temp->next;
    }
    printf("s 3\n");

    if(ret == RET_ADC_SUCCESS)
    {
        hal_start_adc_timer(handle);
    }
    return ret;
}

en_ret_adc hal_adc_stop(adc_handle_t handle)
{
    en_ret_adc ret = RET_ADC_FAILED;
    if(NULL == hal_adc_config_param)
    {
        printf("hal adc link list failed\n");
        return RET_ADC_FAILED;
    }
    hal_adc_config_t *temp = hal_adc_config_param;
    while(temp)
    {
        if(temp->data.adc_handle == handle)
        {
            printf("stop status 1: %d\n",temp->data.adc_status);
            (*temp).data.adc_status = false;
            printf("stop status 2: %d\n",temp->data.adc_status);

            printf("hal_adc_stop handle:%d\n",handle);
            return RET_ADC_SUCCESS;
        }
        temp = temp->next;
    }
    printf("handle is vaild\n");
}

en_ret_adc hal_adc_delete(adc_handle_t handle)
{
    printf("hal_adc_delete\n");
    en_ret_adc ret = RET_ADC_FAILED;
    bool handle_is_empty = false;
    if(NULL == hal_adc_config_param)
    {
        printf("delete ling lisk is NULL\n");
        return RET_ADC_FAILED;
    }
    hal_adc_config_t *temp = hal_adc_config_param;
    hal_adc_config_t *temp1 = temp->next;

        if(NULL == temp->next)
        {
            if(temp->data.adc_handle == handle)
            {
                free(temp);
                hal_adc_config_param = NULL;
                printf("adc delete is success,ADC handle %d\n",handle);
                return RET_ADC_SUCCESS;
            }
            else
            {
                printf("adc delete handle is empty 1\n");
                return RET_ADC_FAILED;
            }
        }
        else
        {
            
            if(temp->data.adc_handle == handle)
            {
                printf("adc delete handle is empty 2\n");

                free(temp);
                hal_adc_config_param = temp1;
                return RET_ADC_SUCCESS;
            }
            while(temp1)
            {
                printf("tem handle %d, input handle %d\n",temp1->data.adc_handle, handle);
                if(temp1->data.adc_handle == handle)
                {
                    temp->next = temp1->next;
                    free(temp1);
                    printf("adc delete is success,ADC handle %d\n",handle);
                    return RET_ADC_SUCCESS;
                    // break;
                }
                temp = temp1;
                temp1 = temp->next;
            }
            printf("adc delete handle is empty 3\n");
        }

    // if(NULL == temp->next)
    // {
    //     hal_adc_stop(handle);
    // }
}

adc_all_func_t adc_all_func_x = 
{
    .BSP_ADC_init = BSP_ADC_init,
    .adc_start    = hal_adc_start,
    .adc_stop     = hal_adc_stop,
    .adc_delete   = hal_adc_delete
};

adc_all_func_t* get_adc_all_func(void)
{
    adc_all_func_t *temp = &adc_all_func_x;
    return (adc_all_func_t *)temp;
}

void check_adc_link_list(hal_adc_config_t *handle)
{
    hal_adc_config_t *temp = handle;
    uint8_t t = 0;
    while(temp)
    {
        t++;
        printf("check:timer: %d; channel: %d; handle %d; status %d\n",t,temp->data.adc_channel, temp->data.adc_handle,temp->data.adc_status);
        temp = temp->next;
    }
}
void adc_test_process(void)
{
    adc_all_func_t *adc_func = get_adc_all_func();
    
    adc_param_t adc_para_t =
    {
        .adc_channel = EN_ADC_CHANNEL_4,
        .adc_handle = 0,
        .adc_record_timer = 0,
        .adc_sample_timer = 100,
        .adc_status = 0
    };
    printf("adc 1\n");
    adc_channel_1 = adc_func->BSP_ADC_init(&adc_para_t);//BSP_ADC_init(&adc_para_t);
    printf("adc_channel_1 %d\n",adc_channel_1);

     adc_param_t adc_para1_t =
    {
        .adc_channel = EN_ADC_CHANNEL_5,
        .adc_handle = 0,
        .adc_record_timer = 0,
        .adc_sample_timer = 200,
        .adc_status = 0
    };
    printf("adc 2\n");
    
    adc_channel_2 = adc_func->BSP_ADC_init(&adc_para1_t);//BSP_ADC_init(&adc_para1_t);
    printf("adc_channel_2 %d\n",adc_channel_2);


    adc_param_t adc_para2_t =
    {
        .adc_channel = EN_ADC_CHANNEL_6,
        .adc_handle = 0,
        .adc_record_timer = 0,
        .adc_sample_timer = 300,
        .adc_status = 0
    };
    printf("adc 3\n");
    
    adc_channel_3 = BSP_ADC_init(&adc_para2_t);

    adc_func->adc_start(adc_channel_1);
    check_adc_link_list(hal_adc_config_param);

    adc_func->adc_start(adc_channel_2);
    check_adc_link_list(hal_adc_config_param);

     adc_func->adc_start(adc_channel_3);
    check_adc_link_list(hal_adc_config_param);

    adc_func->adc_delete(adc_channel_2);
    // check_adc_link_list(hal_adc_config_param);
    // hal_adc_start(adc_channel_1);
    // hal_adc_start(adc_channel_2);
    // hal_adc_start(adc_channel_3);
    printf("adc handle: %d %d %d\n",adc_channel_1,adc_channel_2, adc_channel_3);




}