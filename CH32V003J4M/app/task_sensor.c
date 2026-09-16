#include "task_sensor.h"
#include "bsp_config.h"
#include "touch.h"
#include "key.h"

touch_struct_t touch_0 = {0};
key_struct_t key_0 = {0};

///@brief 获取按键状态
///@param void
///@retval 0未按下
///@retval 1已按下
static uint8_t get_touch_key_state(void)
{
    uint8_t is_touching = 0;
    touch_get_touch_state(&touch_0, &is_touching);

    return is_touching;
}

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

    key_init_t key_initstruct = {
        .time_double = 500,
        .time_long = 1000,
        .time_repeat = 400,

        .get_state = get_touch_key_state,
    };
    key_init(&key_0, &key_initstruct);
}

/// @brief  传感器任务
/// @param  void
/// @return void
void task_sensor(void)
{
    static uint8_t is_en_led = 0;

    key_scan(&key_0);

    uint8_t is_put_dowm = 0;
    key_check_flag(&key_0, KEY_DOWN, &is_put_dowm);
    if (is_put_dowm == 1)
    {
        is_en_led = !is_en_led;
        bsp_tim1_ch4_set_duty(is_en_led ? 960 : 0);
    }

    Delay_Ms(1);
}
