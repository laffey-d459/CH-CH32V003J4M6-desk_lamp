#include "bsp_adc.h"

/// @brief adc通道0初始化
/// @param void
/// @return void
void bsp_adc_ch0_init(void)
{
    ADC_InitTypeDef adc_ch0_initstruct = {0};
    adc_ch0_initstruct.ADC_ContinuousConvMode = DISABLE;
    adc_ch0_initstruct.ADC_DataAlign = ADC_DataAlign_Right;
    adc_ch0_initstruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adc_ch0_initstruct.ADC_Mode = ADC_Mode_Independent;
    adc_ch0_initstruct.ADC_NbrOfChannel = 1;
    adc_ch0_initstruct.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC1, &adc_ch0_initstruct);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_241Cycles);

    ADC_Cmd(ADC1, ENABLE);

    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET)
    {
        ;
    }
}

/// @brief adc通道0开始转换，阻塞式获取adc转换原始值
/// @param void
/// @return adc转换原始值
uint32_t bsp_adc_get_raw(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetSoftwareStartConvStatus(ADC1) == SET)
    {
        ;
    }
    return ADC_GetConversionValue(ADC1);
}
