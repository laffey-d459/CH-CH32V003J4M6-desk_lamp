#include "task_sensor.h"
#include "bsp_config.h"
#include "touch.h"

touch_t touch_0 = {0};

/// @brief  传感器任务初始化
/// @param  void
/// @return void
void task_sensor_init(void)
{
    touch_init_t touch_initstruct = {
        .d_threa = 20,
        .conf_tim_sr = bsp_tim2_conf_sr,
        .get_tim_cvr = bsp_tim2_get_ch2cvr,
        .conf_pin = bsp_gpio_start_measure_touch,
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

    touch_measure_touching(&touch_0, &is_touching);
    if (is_touching == 1 && last_touching == 0)
    {
        is_en_led = !is_en_led;
        bsp_tim1_ch2_set_duty(is_en_led ? 111 : 0);
    }
    last_touching = is_touching;
}
