#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "ch32v00x.h"

/* TIM1_CH4 PWM 参数: 48MHz / (PSC+1) / (ARR+1) = 48MHz / 1 / 960 = 50kHz */
#define BSP_TIM1_PWM_PERIOD    959   /* ARR = 959, 计数 960 个 */
#define BSP_TIM1_PWM_MAX_DUTY  960   /* 占空比分辨率 960 步, 每步约 0.104% */

void bsp_tim1_ch4_pwm_init(void);
void bsp_tim1_ch4_set_duty(uint16_t duty);  /* duty: 0 ~ 960, 对应 0% ~ 100% */

#endif