#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "ch32v00x.h"

void bsp_adc_ch0_init(void);

uint32_t bsp_adc_get_raw(void);

#endif
