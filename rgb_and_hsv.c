#include "cmsis_os2.h"
#include "osal_debug.h"
#include "soc_osal.h"
#include "platform_types.h"
#include "byt_led_aurora.h"
#include "../main_config.h"
#include "rgb_and_hsv.h"
#define COLOR_NUMBER    7
//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
const uint8_t index_wave[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 
                               4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 
                               13, 13, 14, 14, 15, 15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 30, 31, 32, 33, 
                               34, 36, 37, 38, 40, 41, 43, 45, 46, 48, 50, 52, 54, 56, 58, 60, 62, 65, 67, 70, 72, 75, 78, 81, 84, 87, 90, 
                               94, 97, 101, 105, 109, 113, 117, 122, 126, 131, 136, 141, 146, 152, 158, 164, 170, 176, 183, 190, 197, 205, 
                               213, 221, 229, 238, 247, 255, 255, 247, 238, 229, 221, 213, 205, 197, 190, 183, 176, 170, 164, 158, 152, 146, 
                               141, 136, 131, 126, 122, 117, 113, 109, 105, 101, 97, 94, 90, 87, 84, 81, 78, 75, 72, 70, 67, 65, 62, 60, 58, 
                               56, 54, 52, 50, 48, 46, 45, 43, 41, 40, 38, 37, 36, 34, 33, 32, 31, 30, 28, 27, 26, 25, 25, 24, 23, 22, 21, 20, 
                               20, 19, 18, 18, 17, 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 
                               6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
                               2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; //   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0


static const uint8_t seven_color[COLOR_NUMBER][3] = 
{
    {255, 0, 0},     // 红
    {255, 128, 0},   // 橙
    {255, 255, 0},  // 黄
    {0, 255, 0},     // 绿
    {0, 255, 255}, // 青
    {0, 0, 255},     // 蓝
    {128, 0, 255}    // 紫
};


static float min_func(float a, float b, float c)
{
    float m;
    m = a < b ? a : b;
    return (m < c ? m : c); 
}
static float max_func(float a, float b, float c)
{
    float m;
    m = a > b ? a : b;
    return (m > c ? m : c);
}
/*************************************************************************************************
 *@brief This is function user rgb change hsv
 *@param r, g, b, input rgb format，
 *@param *h, *s, *v, output point. hsv(hue, saturation ,value) format
 *************************************************************************************************/
void rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v)
{
    float red, green, blue;
    float cmax, cmin, delta;

    red = (float)r/ 255;
    green = (float)g/ 255;
    blue = (float)b /255;
    cmax = max_func(red, green, blue);
    cmin = min_func(red, green, blue);
    delta = cmax - cmin;

    if(0 == delta)
    {
        *h = 0;
    }
    else
    {
        if(cmax == red)
        {
            if(green >= blue)
            {
                *h = 60 * ((green - blue) / delta);
            }
            else
            {
                *h = 60 * ((green - blue)/ delta);
            }
        }
        else if(cmax == green)
        {
            *h = 60 * ((blue - red) / delta + 2);
        }
        else if(cmax == blue)
        {
            *h = 60 *((red - green) / delta + 4);
        }

    }

    if(cmax == 0)
    {
        *s = 0;
    }
    else
    {
        *s = delta / cmax;
    }
    *v = cmax;
}

/*************************************************************************************************
 *@brief This is function user hsv change rgb
 *@param h, s, v, input hsv(hue,saturation,value) format，
 *@param *r, *g, *b, output point. rgb format
 *************************************************************************************************/
void hsv_to_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    if(0 == s)
    {
        *r = *g = *b = v;
    }
    else
    {
        // int hi = ((int)h / 60) % 6;
        // float f = h * 1.0 /60 - hi;

        float H = h/ 60;
        int hi = (int)H;
        float f = H - hi;//h * 1.0 /60 - hi;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);
        switch(hi)
        {
            case 0:
                *r = (int)(255.0 * v + 0.5);
                *g = (int)(255.0 * t + 0.5);
                *b = (int)(255.0 * p + 0.5);
                break;
            case 1:
                *r = (int)(255.0 * q + 0.5);
                *g = (int)(255.0 * v + 0.5);
                *b = (int)(255.0 * p + 0.5);
                break;
            case 2:
                *r = (int)(255.0 * p + 0.5);
                *g = (int)(255.0 * v + 0.5);
                *b = (int)(255.0 * t + 0.5);
                break;
            case 3:
                *r = (int)(255.0 * p + 0.5);
                *g = (int)(255.0 * q + 0.5);
                *b = (int)(255.0 * v + 0.5);
                break;
            case 4:
                *r = (int)(255.0 * t + 0.5);
                *g = (int)(255.0 * p + 0.5);
                *b = (int)(255.0 * v + 0.5);
                break;
            case 5:
                *r = (int)(255.0 * v + 0.5);
                *g = (int)(255.0 * p + 0.5);
                *b = (int)(255.0 * q + 0.5);
                break;
            default:
                break;
        }
    }
}

/*************************************************************************************************
 *@brief This is function user rgb change hsv
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param *r, *g, *b, output point. rgb format
 effect： like neno 
 *************************************************************************************************/
void flowing_lamp_hsv_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3])
{
    float h,s,v;       
    uint8_t rgb_tem[3];
    static uint16_t temp = 0; 
    static float s_temp = 1;
    static bool s_toggle_flag = 1;
    rgb_to_hsv(rgb_lib[0][0], rgb_lib[0][1], rgb_lib[0][2], &h, &s, &v);
    if(s_toggle_flag == 1)
    {
        s_temp -= 0.01f;
        if(s_temp <= 0.8)
        {
            s_toggle_flag = 0;
        }
    }
    else
    {
        s_temp += 0.01f;
        if(s_temp >= 1)
        {
            s_toggle_flag = 1;
        }
    }
    h += temp ;
    if(s_temp <= 0.5 || s_temp >= 1)
    {
        temp = (1 + temp) % 360;

    }
    hsv_to_rgb(h, s, s_temp, &rgb_tem[1], &rgb_tem[0], &rgb_tem[2]);
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_tem, 3);
    }
}

/*************************************************************************************************
 *@brief This is function user rgb change hsv
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param *r, *g, *b, output point. rgb format
 change saturation and hue
 effect： like neno 
 *************************************************************************************************/
#define SATURATION_LOW_LIMIT    0.9
void flowing_lamp_hsv_s_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3])
{
    float h,s,v;       
    uint8_t rgb_tem[3];
    static uint16_t temp = 0; 
    static float s_temp = 1;
    static bool s_toggle_flag = 1;
    rgb_to_hsv(rgb_lib[0][0], rgb_lib[0][1], rgb_lib[0][2], &h, &s, &v);
    if(s_toggle_flag == 1)
    {
        s_temp -= 0.01f;
        if(s_temp <= SATURATION_LOW_LIMIT)
        {
            s_toggle_flag = 0;
        }
    }
    else
    {
        s_temp += 0.01f;
        if(s_temp >= 1)
        {
            s_toggle_flag = 1;
        }
    }
    h += temp ;
    if(s_temp <= SATURATION_LOW_LIMIT || s_temp >= 1)
    {
        temp = (10 + temp) % 360;

    }
    hsv_to_rgb(h, s_temp, v, &rgb_tem[1], &rgb_tem[0], &rgb_tem[2]);
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_tem, 3);
    }
}


/*************************************************************************************************
 *@brief This is function user rgb change hsv
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param *r, *g, *b, output point. rgb format
 change value(brightness) and hue
 effect： like neno 
 *************************************************************************************************/
#define VALUE_LOW_LIMIT 0.8
void flowing_lamp_hsv_v_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3])
{
    float h,s,v;       
    uint8_t rgb_tem[3];
    static uint16_t temp = 0; 
    static float v_temp = 1;
    static bool v_toggle_flag = 1;
    rgb_to_hsv(rgb_lib[0][0], rgb_lib[0][1], rgb_lib[0][2], &h, &s, &v);
    if(v_toggle_flag == 1)
    {
        v_temp -= 0.01f;
        if(v_temp <= VALUE_LOW_LIMIT)
        {
            v_toggle_flag = 0;
        }
    }
    else
    {
        v_temp += 0.01f;
        if(v_temp >= 1)
        {
            v_toggle_flag = 1;
        }
    }
    h += temp ;
    if(v_temp <= VALUE_LOW_LIMIT || v_temp >= 1)
    {
        temp = (6 + temp) % 360;
    }
    hsv_to_rgb(h, s, v_temp, &rgb_tem[1], &rgb_tem[0], &rgb_tem[2]);
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_tem, 3);
    }
}


/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  1: R shift; 0: L shift
 change hue
 effect： like fowing light lamp 
 *************************************************************************************************/
void flowing_lamp_wave_h_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct)
{
    float h,s,v;
    uint8_t rgb_temp[3] = {0};
    static uint16_t h_temp = 0;
    uint16_t temp = h_temp;
    if(direct == 1)
    {
        for(uint8_t i = 0; i < rgb_num; i++)
        {
            temp = (temp + i) % 360;
            rgb_to_hsv(rgb_lib[0][0], rgb_lib[0][1], rgb_lib[0][2], &h, &s, &v);
            hsv_to_rgb(temp, s, v, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(&rgb_get[i], rgb_temp, 3);
        }
    }
    else
    {
        for(uint8_t i = 0; i <= rgb_num; i++)
        {
            temp = (temp + i) % 360;
            rgb_to_hsv(rgb_lib[0][0], rgb_lib[0][1], rgb_lib[0][2], &h, &s, &v);
            hsv_to_rgb(temp, s, v, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(&rgb_get[rgb_num - i], rgb_temp, 3);
        }
    }

    h_temp = (h_temp + 1) % 360;

}



/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  1: R shift; 0: L shift
 change hue and saturation(main)  
 effect： like fowing light lamp 
 *************************************************************************************************/
#define WAVE_SATURATION_LOW_LIMIT 0.8
void flowing_lamp_wave_s_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct)
{
    float h,s,v;
    uint8_t rgb_temp[3] = {0};
    static float s_temp = 1;
    static uint16_t h_temp = 0;
    uint16_t h_t = h_temp;

    static bool s_toggle_flag = true;
    if(s_toggle_flag == 1)
    {
        s_temp -= 0.01;
        if(s_temp <= WAVE_SATURATION_LOW_LIMIT)
        {
            s_toggle_flag = false;
        }
    }
    else
    {
        s_temp += 0.01;
        if(s_temp >= 1)
        {
            s_toggle_flag = true;
        }
    }
    if(direct == 1)
    {
        for(uint8_t i = 0; i < rgb_num; i ++)
        {
            rgb_to_hsv(rgb_lib[0][0], rgb_lib[1][0], rgb_lib[3][0], &h, &s, &v);
            hsv_to_rgb(h_t, s_temp, v, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(&rgb_get[i], rgb_temp, 3);
            h_t = (h_t + i) % 360;
        }
    }
    else
    {
        for(uint8_t i = 0; i <= rgb_num; i++)
        {
            rgb_to_hsv(rgb_lib[0][0], rgb_lib[1][0], rgb_lib[3][0], &h, &s, &v);
            hsv_to_rgb(h_t, s_temp, v, rgb_temp[1], rgb_temp[0], rgb_temp[2]);
            memcpy(&rgb_get[rgb_num - i], rgb_temp, 3);
            h_t = (h_t + i) % 360;
        }
    }
    if(s_temp <= WAVE_SATURATION_LOW_LIMIT)
    {
        h_temp = (h_temp + 20) %360;
    }
}

/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  1: R shift; 0: L shift
 change hue and value(main)  
 effect： like fowing light lamp and neno
 *************************************************************************************************/
//timer 10ms
#define WAVE_VALUE_LOW_LIMIT    0.9
void flowing_lamp_wave_v_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct)
{
    float h,s,v;
    uint8_t rgb_temp[3] = {0};
    static float v_temp = 1;
    static uint16_t h_temp = 0;
    uint16_t h_t = h_temp;

    static bool v_toggle_flag = true;
    if(v_toggle_flag == 1)
    {
        v_temp -= 0.01;
        if(v_temp <= WAVE_VALUE_LOW_LIMIT)
        {
            v_toggle_flag = false;
        }
    }
    else
    {
        v_temp += 0.01;
        if(v_temp >= 1)
        {
            v_toggle_flag = true;
        }
    }
    if(direct == 1)
    {
        for(uint8_t i = 0; i < rgb_num; i ++)
        {
            rgb_to_hsv(rgb_lib[0][0], rgb_lib[1][0], rgb_lib[3][0], &h, &s, &v);
            hsv_to_rgb(h_t, s, v_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(&rgb_get[i], rgb_temp, 3);
            h_t = (h_t + i) % 360;
        }
    }
    else
    {
        for(uint8_t i = 0; i <= rgb_num; i++)
        {
            rgb_to_hsv(rgb_lib[0][0], rgb_lib[1][0], rgb_lib[3][0], &h, &s, &v);
            hsv_to_rgb(h_t, s, v_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(&rgb_get[rgb_num - i], rgb_temp, 3);
            h_t = (h_t + i) % 360;
        }
    }
    if(v_temp <= WAVE_VALUE_LOW_LIMIT)
    {
        h_temp = (h_temp + 5) %360;
    }
}

/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  1: R shift; 0: L shift
 change hue and value(main)  
 effect： like fowing light ,hue breath
 *************************************************************************************************/
//timer 10ms,one timer breath is 3s
#define HUE_BREATH_STEP 40
void flowing_lamp_hsv_breath_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct)
{
    float h,s,v;
    uint8_t rgb_temp[3];
    static uint16_t h_temp = 0;
    static float v_temp = 1;
    static bool v_toggle_flag = true;
    static uint16_t rgb_index = 0;

    rgb_to_hsv(rgb_lib[0][0],rgb_lib[0][1],rgb_lib[0][2], &h, &s, &v);
    hsv_to_rgb(h_temp, s, v_temp,&rgb_temp[1], &rgb_temp[0], &rgb_temp[1]);
    v_temp = index_wave[rgb_index] * 0.00392f;  // 1 / 255 = 0.00392
    rgb_index ++;
    if(rgb_index >= 300)
    {
        rgb_index = 0;
        h_temp = (h_temp + HUE_BREATH_STEP)%360;
    }

    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_temp, 3);
    }
    
}

/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  1: R shift; 0: L shift
 change hue and value(main)  
 effect： like fowing light ,hue breath
 *************************************************************************************************/

void flowing_lamp_hsv_h_change(uint8_t rgb_num, uint8_t (*rgb_get)[3], uint8_t lib_num, uint8_t (*rgb_lib)[3])
{
    float h,s,v;
    uint8_t rgb_temp[3];
    static uint16_t h_temp = 0;
    rgb_to_hsv(rgb_lib[3][0],rgb_lib[3][1],rgb_lib[3][2], &h, &s, &v);
    hsv_to_rgb(h_temp, s, v, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
    h_temp = (h_temp + 1) % 360;
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_temp, 3);
    }
}


/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  1: R shift; 0: L shift
 change hue and value(main)  
 effect： one color breath mode
 *************************************************************************************************/
//timer 10ms,one timer breath is 3s
uint16_t flowing_lamp_hsv_one_color_breath(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t (*rgb_lib)[3])
{
    static float h,s,v;
    static float v_temp = 1;
    static bool frist_get_hsv = false;
    static bool v_toggle_flag = 1;
    static uint16_t rgb_index = 0;
    uint8_t rgb_temp[3] = {0};
    // if(frist_get_hsv == false)
    // {
    //     frist_get_hsv = true;
        rgb_to_hsv(rgb_lib[0][0], rgb_lib[0][1], rgb_lib[0][2], &h, &s, &v);
        // v_temp = v;
    // }
    v_temp = index_wave[rgb_index] * 0.00392f;
    rgb_index ++;
    if(rgb_index >= 300)
    {
        rgb_index = 0;
    }
    hsv_to_rgb(h,s,v_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
    for(uint8_t i = 0; i < rgb_num; i ++)
    {
        memcpy(&rgb_get[i], rgb_temp, 3);
    }
    return rgb_index;
}


/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 change value(main)  
 effect： seven color breath mode
 *************************************************************************************************/
//timer 10ms,one color breath is 3s ;7 color breath is 21s
void flow_lamp_seven_color_breath(uint8_t rgb_num, uint8_t (*rgb_get)[3])
{
    static uint8_t rgb_step = 0;
    uint8_t rgb_temp[3] = {0};
    uint16_t ret = 0;
    switch(rgb_step)
    {
        case 0:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get,&seven_color[0]);
            if(0 == ret)
            {
                rgb_step ++;
            }
            break;
        case 1:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get,&seven_color[1]);
            if(0 == ret)
            {
                rgb_step ++;
            }
            break;
        case 2:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get, &seven_color[2]);
            if(0 == ret)
            {
                rgb_step ++;
            }
            break;
        case 3:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get, &seven_color[3]);
            if(0 == ret)
            {
                rgb_step ++;
            }
            break;
        case 4:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get, &seven_color[4]);
            if(0 == ret)
            {
                rgb_step ++;
            }
            break;
        case 5:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get, &seven_color[5]);
            if(0 == ret)
            {
                rgb_step ++;
            }
            break;
        case 6:
            ret = flowing_lamp_hsv_one_color_breath(rgb_num, rgb_get, &seven_color[6]);
            if(0 == ret)
            {
                rgb_step = 0;
            }
            break;
        default:
            break;
    }
}


/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number// input array length
 *@param rgb_get: rgb output array[rgb_num][3],
 *@param lib_num: input RGB libarary length 
 *@param rgb_lib: input RGB libarary array length array[lib_num][3] 
 change value(main)  
 effect： seven color breath mode
 *************************************************************************************************/
//timer 10ms,one color breath is 3s ;7 color breath is 21s
void diy_seven_color_breath(uint8_t rgb_num, uint8_t (*rgb_get)[3], uint8_t lib_num, uint8_t (*rgb_lib)[3])
{
    float h,s,v;
    static uint8_t color_step_record = 0;
    static uint16_t rgb_index   = 0;
    uint8_t rgb_temp[3] = {0};
    float v_step_record = 0;
    v_step_record = index_wave[rgb_index] * 0.00392f;  // 1 / 255 = 0.00392;
    rgb_to_hsv(rgb_lib[color_step_record][0], rgb_lib[color_step_record][1], rgb_lib[color_step_record][2], &h, &s, &v);
    hsv_to_rgb(h, s, v_step_record, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_temp, 3);
    }
    rgb_index++;
    if(rgb_index >= 300)
    {
        rgb_index = 0;
        color_step_record ++;
        if(color_step_record >= lib_num)
        {
            color_step_record = 0;
        }
    }

}

/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  nor user
 change hue and value ;x = hue, y = value ,y = 0.9 + 0.1sin*(Omega * x)；
//  cycle = 
 effect： one color breath mode
 *************************************************************************************************/
void flowing_lamp_hsv_v_sin_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, 
                                    uint8_t (*rgb_lib)[3],bool direct)
{
    static float h,s,v;
    static  h_temp = 0;
    static float v_temp = 0;
    static float x_temp = 0;
    static float y_temp = 0;
    static bool first_get_rgb_flag = false;
    int Omega = 6;
    uint8_t rgb_temp[3] = {0};
    if(first_get_rgb_flag == false)
    {
        first_get_rgb_flag = true;
        rgb_to_hsv(rgb_lib[0][0],rgb_lib[0][1], rgb_lib[0][2], &h,&s,&v);
        h_temp = h;
        v_temp = v;
    }
    y_temp = 0.9 + 0.1*sin(Omega * x_temp);
    hsv_to_rgb(x_temp/Omega, 1, y_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
    x_temp += 1;
    if(x_temp >= 360*Omega)
    {
        x_temp = 0;
    }
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i],rgb_temp, 3);
    }
    

}

/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp length,
 *@param rgb_get: rgb output array[rgb_num][3],
 *@param rgb_lib: input rgb color
 *************************************************************************************************/
void flowing_lamp_constant_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3], const uint8_t (*rgb_lib)[3])
{
    for(uint8_t i = 0; i < rgb_num; i++)
    {
        memcpy(&rgb_get[i], rgb_lib, 3);
    }
}


/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number array,
 *@param rgb_get: rgb output array[3],
 *@param lib_num: input rgb array number
 *@param rgb_lib: input rgb lib array[3]
 *@param direct:  true： R shift； false： L shift

//  cycle = （1 - WAVE_VALUE）/ WAVE_AVLUE_STEP
 effect： color flowing lamp ，v decrease effect flowing
 *************************************************************************************************/
#define WAVE_VALUE_STEP 0.1
#define WAVE_VALUE_LOW_LIMIT    0
#define WAVE_HUE_STEP       10
void flowing_lamp_wave_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3], const uint8_t (rgb_lib)[3],bool direct)
{
    float h,s,v;
    static uint16_t h_record= 0;
    uint16_t h_temp = h_record;
    static float    v_record = 1;
    float    v_temp = v_record;
    uint8_t rgb_temp[3];
    if(direct == true)
    {
        for(uint8_t i =0; i < rgb_num; i++)
        {
            rgb_to_hsv(rgb_lib[0], rgb_lib[1], rgb_lib[2], &h, &s, &v);
            
            hsv_to_rgb(h_temp, s,v_temp, &rgb_temp[1], &rgb_temp[0],&rgb_temp[2]);
            memcpy(&rgb_get[i], rgb_temp, 3);
            v_temp -= WAVE_VALUE_STEP;
            if(v_temp <= WAVE_VALUE_LOW_LIMIT)
            {
                v_temp = 1;
                h_temp = (h_temp + WAVE_HUE_STEP) % 360;
            }
        }
    }
    else
    {
        for(uint8_t i = 0;i <= rgb_num; i++)
        {
            rgb_to_hsv(rgb_lib[0], rgb_lib[1], rgb_lib[2], &h, &s, &v);
            hsv_to_rgb(h_temp, s, v_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(&rgb_get[rgb_num - i], rgb_temp, 3);
            v_temp -= WAVE_VALUE_STEP;
            if(v_temp <= WAVE_VALUE_LOW_LIMIT)
            {
                v_temp = 1;
                h_temp = (h_temp + WAVE_HUE_STEP) % 360;
            }
        }
    }
    v_record -= WAVE_VALUE_STEP;
    if(v_record <= WAVE_VALUE_LOW_LIMIT)
    {
        v_record = 1;
        h_record = (h_record + WAVE_HUE_STEP) % 360;
    }
}

typedef struct rgb_diy_flowing_lamp
{
    uint8_t lib_num;
    uint8_t (*rgb_lib)[3];
    bool direct;
}rgb_diy_flowing_lamp_t;


/*************************************************************************************************
 *@brief flowing lamp light
 *@param rgb_num:   output rgb lamp number length,
 *@param rgb_get: rgb output array[rgb_num][3],
 *@param lib_num: input rgb array length //  length is flowing lamp wava number
 *@param rgb_lib: input rgb lib array[lib_num][3]. //is wave color numbers
 *@param direct:  true： R shift； false： L shift

 effect： color flowing lamp ，v decrease effect flowing/
 *************************************************************************************************/
#define DIY_WAVE_VALUE_STEP 0.1             //value step
#define DIY_WAVE_VALUE_LOW_LIMIT    0.5     //value low limit

void diy_flowing_lamp_v_decrease(uint8_t rgb_num, uint8_t (*rgb_get)[3], uint8_t lib_num,const uint8_t (*rgb_lib)[3], bool direct)
{
    float  h,s,v;
    static uint16_t h_record = 0;
    static float v_record = 0;
    uint16_t h_temp = h_record;
    float    v_temp = v_record;
    static uint8_t color_step_record = 0;
    uint8_t color_step_temp = color_step_record;
    uint8_t rgb_temp[3] = {0};
    if(direct == true)
    {
        for(uint8_t i = 0; i < rgb_num; i++)
        {
            v_temp -= DIY_WAVE_VALUE_STEP;
            rgb_to_hsv(rgb_lib[color_step_temp][0], rgb_lib[color_step_temp][1], rgb_lib[color_step_temp][2], &h, &s, &v);
            hsv_to_rgb(h, s, v_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(rgb_get[i], rgb_temp, 3);
            if(v_temp <= DIY_WAVE_VALUE_LOW_LIMIT)
            {
                v_temp = 1;
                color_step_temp ++;
                if(color_step_temp >= lib_num)
                {
                    color_step_temp = 0;
                }

            }
        }
    }
    else
    {
        for(uint8_t i = 0; i <= rgb_num; i ++)
        {
            v_temp -= DIY_WAVE_VALUE_STEP;
            rgb_to_hsv(rgb_lib[color_step_temp][0], rgb_lib[color_step_temp][1], rgb_lib[color_step_temp][2], &h, &s, &v);
            hsv_to_rgb(h, s, v_temp, &rgb_temp[1], &rgb_temp[0], &rgb_temp[2]);
            memcpy(rgb_get[rgb_num - i], rgb_temp, 3);
            if(v_temp <= DIY_WAVE_VALUE_LOW_LIMIT)
            {
                v_temp = 1;
                color_step_temp ++;
                if(color_step_temp >= lib_num)
                {
                    color_step_temp = 0;
                }
            }
        }
    }
    v_record -= DIY_WAVE_VALUE_STEP;
    if(v_record <= DIY_WAVE_VALUE_LOW_LIMIT)
    {
        v_record = 1;
        color_step_record ++;
        if(color_step_record >= lib_num)
        {
            color_step_record = 0;
        }
    }

    
}