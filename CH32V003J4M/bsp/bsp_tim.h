#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "ch32v00x.h"

void bsp_tim1_ch2_pwm_init(void);
void bsp_tim2_ch2_ic_init(void);

void bsp_tim1_ch2_set_duty(uint16_t duty);
uint32_t bsp_tim2_conf_sr(uint8_t state);
uint32_t bsp_tim2_get_ch2cvr(uint8_t data);

#endif
