#include "bsp_config.h"

/// @brief  板级初始化
/// @param  void
/// @return void
void board_init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_SetPriority(TIM2_IRQn, 3);
    NVIC_EnableIRQ(TIM2_IRQn);

    SystemCoreClockUpdate();
    Delay_Init();

    bsp_per_clock_init();

    bsp_gpio_t1c4_init();
    bsp_gpio_t2c2_init();
    bsp_gpio_touch_init();

    bsp_tim1_ch4_pwm_init();
    bsp_tim2_ch2_pwm_init();

    bsp_adc_ch0_init();
}
