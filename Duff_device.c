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


void duff_device_copy(uint8_t *to,uint8_t *from, uint16_t data_size)
{
    uint16_t n = (data_size + 7) / 8;
    switch(data_size%8)
    {
        
            case 0:do{ *to++ = *from ++;
            case 7:*to++ = *from ++;
            case 6:*to++ = *from ++;
            case 5:*to++ = *from ++;
            case 4:*to++ = *from ++;
            case 3:*to++ = *from ++;
            case 2:*to++ = *from ++;
            case 1:*to++ = *from ++;
            /* code */
        } while (--n > 0);
        
    } 
}

#include <stdio.h>
 
// 滑动平均滤波函数
void moving_average_filter(double *data, double *output, int data_size, int window_size) {
    // 如果窗口大小大于数据长度，则不进行滤波
    if (window_size > data_size) {
        printf("窗口大小不能大于数据大小。\n");
        return;
    }
    
    // 计算每个点的滑动平均值
    for (int i = 0; i < data_size; i++) {
        double sum = 0.0;
        int count = 0;
        
        // 计算窗口内的平均值
        for (int j = i - (window_size - 1) / 2; j <= i + (window_size - 1) / 2; j++) {
            // 确保索引在有效范围内
            if (j >= 0 && j < data_size) {
                sum += data[j];
                count++;
            }
        }
        
        // 计算并存储当前位置的滑动平均值
        output[i] = sum / count;
    }
}
 
int main() {
    int n, window_size;
    
    // 输入数据大小和窗口大小
    printf("请输入数据的大小: ");
    scanf("%d", &n);
    printf("请输入窗口大小 (奇数): ");
    scanf("%d", &window_size);
    
    // 输入数据
    double data[n];
    printf("请输入数据:\n");
    for (int i = 0; i < n; i++) {
        scanf("%lf", &data[i]);
    }
    
    // 创建一个数组来存储滤波后的结果
    double output[n];
    
    // 调用滑动平均滤波函数
    moving_average_filter(data, output, n, window_size);
    
    // 输出滤波后的结果
    printf("滑动平均滤波后的数据:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", output[i]);
    }
    printf("\n");
    
    return 0;
}