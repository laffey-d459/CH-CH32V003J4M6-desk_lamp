#include "bsp_gpio.h"

static GPIO_InitTypeDef GPIO_TOUCH_InitStructure = {0};

/// @brief 定时器1通道2的GPIO初始化
/// @param void
/// @return void
/// @note PA1 PWM输出
void bsp_gpio_t1c2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/// @brief 触摸感应GPIO初始化
/// @param void
/// @return PC1 充电引脚, PC2 感应引脚
void bsp_gpio_touch_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);

    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
    GPIO_TOUCH_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_TOUCH_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_TOUCH_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_TOUCH_InitStructure);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
}

/// @brief 配置引脚，准备/结束测量是否发生触摸事件
/// @param is_prepare 是否准备测量（0结束，1准备）
/// @return void
uint32_t bsp_gpio_start_measure_touch(uint8_t is_prepare)
{
    if (is_prepare != 0)
    {
        GPIO_TOUCH_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOC, &GPIO_TOUCH_InitStructure);

        GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);

        GPIO_TOUCH_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOC, &GPIO_TOUCH_InitStructure);
        GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);

        Delay_Us(200);
    }
}
