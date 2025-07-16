#ifndef __RGB_AND_HSV_H__
#define __RGB_AND_HSV_H__
extern const uint8_t index_wave[];

void rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b, float *h, float *s, float *v);
void hsv_to_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);

void flowing_lamp_hsv_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3]);
void flowing_lamp_hsv_s_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3]);
void flowing_lamp_hsv_v_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3]);
void flowing_lamp_wave_h_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct);
void flowing_lamp_wave_s_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct);
void flowing_lamp_wave_v_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct);
void flowing_lamp_hsv_breath_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, uint8_t (*rgb_lib)[3], uint8_t direct);
void flowing_lamp_constant_mode(uint8_t rgb_num, uint8_t (*get_rgb)[3],const uint8_t(*rgb_lib)[3]);
void flowing_lamp_hsv_h_change(uint8_t rgb_num, uint8_t (*rgb_get)[3], uint8_t lib_num, uint8_t (*rgb_lib)[3]);
uint16_t flowing_lamp_hsv_one_color_breath(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t (*rgb_lib)[3]);
void flow_lamp_seven_color_breath(uint8_t rgb_num, uint8_t (*rgb_get)[3]);
void flowing_lamp_hsv_v_sin_change(uint8_t rgb_num, uint8_t (*rgb_get)[3],uint8_t lib_num, 
                                    uint8_t (*rgb_lib)[3],bool direct);

void flowing_lamp_wave_mode(uint8_t rgb_num, uint8_t (*rgb_get)[3], const uint8_t (rgb_lib)[3],bool direct);
void diy_flowing_lamp_v_decrease(uint8_t rgb_num, uint8_t (*rgb_get)[3], uint8_t lib_num,const uint8_t (*rgb_lib)[3], bool direct);
void diy_seven_color_breath(uint8_t rgb_num, uint8_t (*rgb_get)[3], uint8_t lib_num, uint8_t (*rgb_lib)[3]);

#endif