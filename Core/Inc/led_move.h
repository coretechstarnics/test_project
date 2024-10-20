#ifndef _LED_MOVE_H_
#define _LED_MOVE_H_

#include "main.h"
#include "led_conf.h"


void led_color_init             (rgb_color* , LedTypeDef *);

uint8_t led_shift_through  		  (rgb_color *, rgb_color,  rgb_color, LedTypeDef *, LedTypeDef *, LedTypeDef *, FlagStatus);

uint8_t led_left_through  		  (rgb_color *, rgb_color,  rgb_color, LedTypeDef *, LedTypeDef *, LedTypeDef *, FlagStatus);
uint8_t led_right_through  		  (rgb_color *, rgb_color,  rgb_color, LedTypeDef *, LedTypeDef *, LedTypeDef *, FlagStatus);

uint8_t led_shift_left          (rgb_color *, LedTypeDef *, FlagStatus);
uint8_t led_shift_left_num      (rgb_color *, LedTypeDef *, FlagStatus, uint8_t);
uint8_t led_shift_left_num_fill (rgb_color *, LedTypeDef *, FlagStatus, uint8_t);
uint8_t led_shift_left_color  (rgb_color *, LedTypeDef *, FlagStatus);

uint8_t led_shift_right       (rgb_color *, LedTypeDef *, FlagStatus);
uint8_t led_shift_right_num   (rgb_color *, LedTypeDef *, FlagStatus, uint8_t);
uint8_t led_shift_right_color (rgb_color *, LedTypeDef *, FlagStatus);

void led_rotate_left       (rgb_color *, LedTypeDef *);
void led_rotate_right      (rgb_color *, LedTypeDef *);

uint8_t led_rotate_left_thru  (rgb_color *, LedTypeDef *);
uint8_t led_rotate_right_thru (rgb_color *, LedTypeDef *);

void read_key(void);

#endif
