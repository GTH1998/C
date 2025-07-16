#include "cmsis_os2.h"
#include "osal_debug.h"
#include "soc_osal.h"
#include "platform_types.h"
#include "byt_led_aurora.h"
#include "../main_config.h"
// #include "rgb_and_hsv.h"
#define QS_DATA_T   int32_t

void swap(QS_DATA_T a[], QS_DATA_T i, QS_DATA_T j)
{
    QS_DATA_T t = a[i];
    a[i] = a[j];
    a[j] = t;
}

 
 QS_DATA_T partion(QS_DATA_T a[], QS_DATA_T p, QS_DATA_T r)
 {
    QS_DATA_T i = p;
    QS_DATA_T j = r + 1;
    QS_DATA_T x = a[p];
    while(1)
    {
        while(i<r && a[++i]<x);
        while(a[--j]>x);
        if(i >= j)  break;
        swap(a, i, j);
    }
    swap(a, p, j);
    return j;
 }

 void quick_sort(QS_DATA_T a[], QS_DATA_T p,QS_DATA_T r)
 {
    if(p < r)
    {
        QS_DATA_T q = partion(a, p, r);
        quick_sort(a, p, q-1);
        quick_sort(a, q+1, r);
    }
 }

 void quick_process_func(void)
 {
    osal_printk("quick process:");
    int a[] = {50,49,48,48,47,46,1,3,5,2,4,6,30,30,30,30};
    int N = sizeof(a)/sizeof(a[1]) - 1;
    uint64_t tick_record =uapi_systick_get_us();
    uapi_systick_count_clear();
    quick_sort(a, 0 , N);
    tick_record = uapi_systick_get_count() ;
    osal_printk("tick record: %d \n",tick_record);
    for(uint8_t i = 0; i < N +1; i++)
    {
        osal_printk("%d ",a[i]);
    }
    osal_printk("\n");
 }