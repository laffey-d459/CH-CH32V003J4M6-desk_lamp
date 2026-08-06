#include "bsp_clock.h"
#include "ch32v00x.h"

/// @brief  芯片的外设的时钟初始化
/// @param  void
/// @return void
void bsp_per_clock_init(void)
{
    /* 使能 GPIOA、GPIOC、GPIOD 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    /* 使能 TIM1 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}