#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "ch32v00x.h"

void bsp_gpio_t1c2_init(void);
void bsp_gpio_touch_init(void);

uint32_t bsp_gpio_start_measure_touch(uint8_t data);

#endif
