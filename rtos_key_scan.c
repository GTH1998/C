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
#include "rtos_key_scan.h"
#include "../main_config.h"


uint8_t *p_send = NULL;
uint8_t test = 1;

#define BUTTON_PIN_1    23
#define KEY_SW_DEBOUNCE_TIME_MS  6 // 5ms-20ms
static void key_1_timer_cb(unsigned long arg);
bool g_key_1 = 0;
static debounce_state_t g_mouse_flowing_lamp_state; // 单键，DPI切换键状态
extern peripheral_process_cb flowing_lamp_process_cb;



static osal_timer g_mouse_flowing_lamp_irqs_timer = {0, key_1_timer_cb, 0, KEY_SW_DEBOUNCE_TIME_MS};



void config_flowing_lamp_key_cb(uint8_t pin)
{
    int ret;
    osal_printk("key down %d\n", pin);
    uapi_gpio_disable_interrupt(BUTTON_PIN_1);
     ret = osal_timer_start(&g_mouse_flowing_lamp_irqs_timer);
     if(0 == ret)
     {
        g_mouse_flowing_lamp_state = KEY_HW_DEBOUNCE_STATE;
     }
     else
     {
        g_mouse_flowing_lamp_state = KEY_DEBOUNCE_IDLE;
        uapi_gpio_enable_interrupt(BUTTON_PIN_1);
     }
}


static void key_1_timer_cb(unsigned long arg)
{
    UNUSED(arg);
    gpio_level_t io_level ;
    debounce_state_t flowing_lamp_key_state = g_mouse_flowing_lamp_state;
    switch (flowing_lamp_key_state) {
        case KEY_HW_DEBOUNCE_STATE:
            g_key_1 = uapi_gpio_get_val(BUTTON_PIN_1);
            g_mouse_flowing_lamp_state = KEY_SW_DEBOUNCE_STATE;
            osal_timer_start(&g_mouse_flowing_lamp_irqs_timer);
            break;
        case KEY_SW_DEBOUNCE_STATE:
            io_level = uapi_gpio_get_val(BUTTON_PIN_1);
            if (g_key_1 == io_level) {
                if (io_level == GPIO_LEVEL_HIGH) {
                     osal_printk("app_mouse_flowing_lamp_timer_cbk\n");
                     p_send = &test;
                     *p_send = 1;
                     flowing_lamp_process_cb(1, 1, p_send);

                    // if(get_sle_mouse_connected() == SLE_ACB_STATE_CONNECTED || g_sle_mouse_mode == byt_mouse_wired)
                    // {
                    //     dpi_change_process_cb( EN_KEY_CHANGE_DPI, 0, NULL);
                    // }

                }
                else
                {
                    osal_printk("AAA\n");
                }
            }
            // byt_reboot_from_idle();
            // app_mouse_idle_time_reset();
            // app_mouse_pair_timer_reset();
            osal_timer_stop(&g_mouse_flowing_lamp_irqs_timer);
            g_mouse_flowing_lamp_state = KEY_DEBOUNCE_IDLE;
            uapi_gpio_enable_interrupt(BUTTON_PIN_1);
            osal_printk("key down end\n");

            break;
        default:
            break;
    }
}


// pin interrupt function
void key_intrrupt_func(uint8_t pin)
{

    // timer_start_func();
}
void flowing_lamp_key_init(void)
{
    uapi_gpio_init();
    uapi_pin_set_mode(BUTTON_PIN_1, (pin_mode_t)HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(BUTTON_PIN_1, GPIO_DIRECTION_INPUT);
    uapi_pin_set_pull(BUTTON_PIN_1, PIN_PULL_UP);
    uapi_gpio_register_isr_func(BUTTON_PIN_1, GPIO_INTERRUPT_DEDGE,
    (gpio_callback_t)config_flowing_lamp_key_cb);
    int ret = osal_timer_init(&g_mouse_flowing_lamp_irqs_timer); 
    osal_printk("flowing lamp init ret %d\n", ret);
    if (ret != 0) {
        osal_printk("g_mouse_flowing_lamp_irqs_timer create failed!\n");
    }
}