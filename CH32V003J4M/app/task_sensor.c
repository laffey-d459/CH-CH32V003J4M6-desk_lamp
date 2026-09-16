#include "task_sensor.h"
#include "bsp_config.h"
#include "touch.h"

touch_struct_t touch_0 = {0};

/// @brief  传感器任务初始化
/// @param  void
/// @return void
void task_sensor_init(void)
{
    touch_init_t touch_initstruct = {
        .d_threa = 20,

        .k_1iir = 51,

        .pi_kp = 32,
        .pi_ki = 8192,
        .pi_integral_max = 16384,
        .pi_integral_min = -16384,

        .get_adc_value = bsp_adc_get_raw,
    };
    touch_init(&touch_0, &touch_initstruct);
}

/// @brief  传感器任务
/// @param  void
/// @return void
void task_sensor(void)
{
    static uint8_t is_en_led = 0;
    static uint8_t last_touching = 0;
    uint8_t is_touching = 0;

    touch_get_touch_state(&touch_0, &is_touching);
    if (is_touching == 1 && last_touching == 0)
    {
        is_en_led = !is_en_led;
        bsp_tim1_ch4_set_duty(is_en_led ? 960 : 0);
    }
    last_touching = is_touching;
}
