#ifndef __RTOS_KEY_SCAN_H
#define __RTOS_KEY_SCAN_
#include "../mouse_button/mouse_button.h"
extern void flowing_lamp_key_init(void);

// typedef enum {
//     KEY_DEBOUNCE_IDLE,      // 按键去抖初始化状态
//     KEY_HW_DEBOUNCE_STATE,  // 按键消除硬件抖状态
//     KEY_SW_DEBOUNCE_STATE,  // 按键软件件去抖状态
// } debounce_state_t;

// typedef struct {
//     debounce_state_t state;  // 按键去抖动状态
//     uint8_t hw_debouce_time; // 按键硬件抖动时间范围：暂定1-5ms之间，以实测结论调定
//     uint8_t sw_debouce_time; // 按键软件件抖时间范围：暂定5-15ms之间，以实测结论值定
// } mouse_key_debounce_t;

#endif