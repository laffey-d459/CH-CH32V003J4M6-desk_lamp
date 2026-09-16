#include "bsp_gpio.h"

/// @brief 定时器1通道4的GPIO初始化
/// @param void
/// @return void
/// @note PC4 PWM输出
void bsp_gpio_t1c4_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/// @brief 定时器2通道2的GPIO初始化
/// @param void
/// @return void
/// @note PC2 PWM输出
void bsp_gpio_t2c2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/// @brief 触摸感应GPIO初始化
/// @param void
/// @return PA2 感应引脚
void bsp_gpio_touch_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
