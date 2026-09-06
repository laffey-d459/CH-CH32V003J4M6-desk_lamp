#ifndef __BSP_TIM_H
#define __BSP_TIM_H

#include "ch32v00x.h"

/* TIM1_CH2 PWM 参数: 48MHz / (PSC+1) / (ARR+1) = 48MHz / 1 / 960 = 50kHz */
#define BSP_TIM1_PWM_PERIOD    959   /* ARR = 959, 计数 960 个 */
#define BSP_TIM1_PWM_MAX_DUTY  960   /* 占空比分辨率 960 步, 每步约 0.104% */

void bsp_tim1_ch2_pwm_init(void);
void bsp_tim2_ch2_ic_init(void);
void bsp_tim1_ch2_set_duty(uint16_t duty);  /* duty: 0 ~ 960, 对应 0% ~ 100% */
uint32_t bsp_tim2_conf_sr(uint8_t state);
uint32_t bsp_tim2_get_ch2cvr(uint8_t data);

#endif
