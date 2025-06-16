#ifndef __BSC_ADC_CONFIG_H__
#define __BSC_ADC_CONFIG_H__


#define adc_handle_t uint8_t 

enum
{
    EN_ADC_CHANNEL_0,
    EN_ADC_CHANNEL_1,
    EN_ADC_CHANNEL_2,
    EN_ADC_CHANNEL_3,
    EN_ADC_CHANNEL_4,
    EN_ADC_CHANNEL_5,
    EN_ADC_CHANNEL_6,
    EN_ADC_CHANNEL_7,
    EN_ADC_CHANNEL_8,
};


typedef enum
{
    RET_ADC_SUCCESS,
    RET_ADC_FAILED
}en_ret_adc;

typedef struct adc_param 
{
    uint8_t adc_handle;
    uint8_t adc_channel;
    uint32_t adc_sample_timer;
    uint32_t adc_record_timer;
    bool adc_status;
    // bool adc_running;
}adc_param_t;


 typedef struct hal_adc_config
 {
    adc_param_t data;
    struct hal_adc_config *next;
 }hal_adc_config_t;

typedef struct adc_all_func
{
    adc_handle_t (*BSP_ADC_init)(adc_param_t *param);
    en_ret_adc (*adc_start)(adc_handle_t handle);
    en_ret_adc (*adc_stop)(adc_handle_t handle);
    en_ret_adc (*adc_delete)(adc_handle_t handle);
}adc_all_func_t;

#endif