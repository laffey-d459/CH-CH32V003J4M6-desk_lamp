#include "bsp_config.h"
#include "debug.h"

/// @brief  板级初始化
/// @param  void
/// @return void
void board_init (void) 
{
    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    
    bsp_per_clock_init();

    bsp_gpio_init();

    bsp_tim1_ch4_pwm_init();
}
