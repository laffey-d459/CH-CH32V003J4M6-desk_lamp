#include "bsp_tim.h"

/// @brief  TIM1_CH2 PWM 初始化 (50kHz, 分辨率 960 步)
/// @param  void
/// @return void
void bsp_tim1_ch2_pwm_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    /* 时基配置: 48MHz / (0+1) = 48MHz, ARR=959 -> 50kHz */
    TIM_TimeBaseInitStructure.TIM_Period = BSP_TIM1_PWM_PERIOD;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    /* CH2 PWM 模式1 输出配置 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; /* 初始占空比 0% */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    /* 使能 CH2 预装载 */
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* 使能自动重装载预装载 */
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 高级定时器需使能主输出 MOE */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    /* 启动 TIM1 */
    TIM_Cmd(TIM1, ENABLE);
}

/// @brief  设置 TIM1_CH2 占空比
/// @param  duty  0 ~ BSP_TIM1_PWM_MAX_DUTY (960), 对应 0% ~ 100%
/// @return void
void bsp_tim1_ch2_set_duty(uint16_t duty)
{
    if (duty > BSP_TIM1_PWM_MAX_DUTY)
    {
        duty = BSP_TIM1_PWM_MAX_DUTY;
    }

    TIM_SetCompare2(TIM1, duty);
}