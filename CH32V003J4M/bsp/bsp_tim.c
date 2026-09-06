#include "bsp_tim.h"

#define BSP_TIM1_PWM_MAX_DUTY 960

/// @brief TIM1_CH2 PWM 初始化 (50kHz, 分辨率 960 步)
/// @param void
/// @return void
void bsp_tim1_ch2_pwm_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    TIM_TimeBaseInitStructure.TIM_Period = BSP_TIM1_PWM_MAX_DUTY - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
}

/// @brief TIM2 _CH2 IC 初始化
/// @param void
/// @return void
void bsp_tim2_ch2_ic_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_ICInitTypeDef TIM_ICInitStructure = {0};

    TIM_TimeBaseInitStructure.TIM_Period = 65535;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);

    TIM_Cmd(TIM2, DISABLE);
}

/// @brief 设置 TIM1_CH2 占空比
/// @param duty  0 ~ BSP_TIM1_PWM_MAX_DUTY (960), 对应 0% ~ 100%
/// @return void
void bsp_tim1_ch2_set_duty(uint16_t duty)
{
    if (duty > BSP_TIM1_PWM_MAX_DUTY)
    {
        duty = BSP_TIM1_PWM_MAX_DUTY;
    }

    TIM_SetCompare2(TIM1, duty);
}

/// @brief 设置tim2开关
/// @param dustatety 定时器开关状态（0关，1开）
/// @return void
uint32_t bsp_tim2_conf_sr(uint8_t state)
{
    if (state != 0)
    {
        TIM_Cmd(TIM2, ENABLE);
    }
    else
    {
        TIM_Cmd(TIM2, DISABLE);
        TIM_SetCounter(TIM2, 0);
    }
}

/// @brief 获取tim2ch2捕获值
/// @param data void
/// @return 捕获值
uint32_t bsp_tim2_get_ch2cvr(uint8_t data)
{
    return TIM2->CH2CVR;
}
