#ifndef __BSP_KEY_H__ 
#define __BSP_KEY_H__


#define LONG_PRESS_TIME 	1500
#define MIN_PRESS_TIME  	120
#define SHAKE_WAIT_TIME     10          //key debounce tick

#define SHORT_PRESS		1
#define LONG_PRESS		2
typedef enum
{
    EN_KEY_WAIT_STATUS,
    EN_KEY_PRESS_STATUS,
    EN_KEY_RELEASE_STATUS,
    EN_KEY_DOUBLE_STATUS,
    EN_KEY_IDEL_STATUS,
}en_bsp_key_status_t;

enum
{
    EN_KEY_RELEASE,
    EN_KEY_SHORT_PRESS = 1,
    EN_KEY_LONG_PRESS
};

typedef struct key_status_mode
{
    uint16_t key_running_time;
    uint8_t key_status;
    uint16_t key_cnt;
    uint8_t key_press_mode;
}key_status_mode_t;


typedef struct 
{
    uint32_t    GPIO_PIN;
    uint8_t     PIN_PULL;
}key_pin_define_t;
typedef struct 
{
    uint8_t key_shake_time;
    uint16_t key_short_press_time;
    uint16_t key_long_press_time;
}key_shack_t;


typedef struct bsp_key_handle
{
    uint8_t handle;
    bool key_enable;
    key_pin_define_t pin_setting;
    // key_shack_t key_shack;
    key_status_mode_t key_mode;
}bsp_key_handle_t;

typedef struct bsp_key_handle_link
{
    bsp_key_handle_t key_mode;
    struct bsp_key_handle_link* next; 
}bsp_key_handle_link_t;

typedef enum key_level
{
    KEY_LEVEL_LOW,
    KEY_LEVEL_HIGH
}key_level_t;


#endif