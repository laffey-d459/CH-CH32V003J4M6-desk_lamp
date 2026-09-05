#include "bsp_gpio.h"

/// @brief  定时器1通道2的GPIO初始化 (PA1 PWM输出)
/// @param  void
/// @return void
void bsp_gpio_t1c2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
