#include <stdio.h>

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
#include "byt_peripheral_process.h"
#include "../main_config.h"

typedef uint8_t filler_data_type;


void bubble_sort_process(filler_data_type *input_output, uint16_t data_len)
{
    filler_data_type temp ;
    for(uint16_t i = 0; i < data_len - 1; i++ )
    {
        for(uint16_t j = 0; j < data_len - i -1; j++)
        {
            if(input_output[j] > input_output[j+1])
            {
                temp = input_output[j];
                input_output[j] = input_output[j + 1];
                input_output[j + 1] = temp;
            }
        }
    }
}

void average_filler_and_extremum_process(filler_data_type *input,filler_data_type *output, uint16_t data_len)
{
    bubble_sort_process(input, data_len);
    double temp = 0;
    for(uint16_t i = 1; i < data_len - 1; i++)
    {
        temp += input[i];
    }
    *output = temp/(data_len - 2);
}

void moving_average_filler_process(filler_data_type *input,filler_data_type *output, uint16_t data_len, uint8_t window)
{
    if(window > data_len)
    {
        printf("window over the range\n");
        return;
    }
    
}