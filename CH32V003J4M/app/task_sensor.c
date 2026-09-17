#include "task_sensor.h"
#include "bsp_config.h"
#include "touch.h"
#include "key.h"

touch_struct_t touch_0 = {0};
key_struct_t key_0 = {0};

uint16_t led_light_arr[] = {0, 96 * 3, 96 * 6, 96 * 10};

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
        .time_double = 0,
        .time_long = 300,
        .time_repeat = 20,

        .get_state = get_touch_key_state,
    };
    key_init(&key_0, &key_initstruct);
}

/// @brief  传感器任务
/// @param  void
/// @return void
void task_sensor(void)
{
    static uint8_t led_light_index = 0;
    static int32_t led_light = 0;
    static int8_t led_light_step = 5;
    static uint8_t was_long = 0;

    key_scan(&key_0);

    uint8_t is_single = 0, is_long = 0, is_repeat = 0, is_up = 0;
    key_check_flag(&key_0, KEY_SINGLE, &is_single);
    key_check_flag(&key_0, KEY_LONG, &is_long);
    key_check_flag(&key_0, KEY_REPEAT, &is_repeat);
    key_check_flag(&key_0, KEY_UP, &is_up);
    if (is_single)
    {
        led_light_index = (led_light_index + 1) % (sizeof(led_light_arr) / sizeof(led_light_arr[0]));
        led_light = led_light_arr[led_light_index];
        if (led_light >= 960)
        {
            led_light_step -= 5;
        }
        else if (led_light <= 0)
        {
            led_light_step += 5;
        }
    }
    if (is_long || is_repeat)
    {
        was_long = 1;
        led_light += led_light_step;
        if (led_light >= 960)
        {
            led_light = 960;
        }
        else if (led_light <= 0)
        {
            led_light = 0;
        }
    }
    if (is_up && was_long)
    {
        if (led_light >= 960)
        {
            led_light_step = -5;
        }
        else if (led_light <= 0)
        {
            led_light_step = 5;
        }
        was_long = 0;
    }
    bsp_tim1_ch4_set_duty(led_light);


    Delay_Ms(1);
}
