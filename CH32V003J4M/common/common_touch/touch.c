/********************************************************************************************
 * file description: 触摸按键状态扫描
 *                   方案：定时器输出pwm通过电阻后给触摸节点，触摸节点再直连adc接口。
 *                         pwm推荐配置PWM1、有效电平为高电平
 *
 * Version: V1.0.0
 * Copyright (c) 2026, laffey-d459.
 * Licensed under the MIT License.
 *
 * Modify Record:
 * 2026/09/16  laffey-d459  V1.0.0  初始版本
 ********************************************************************************************/
#include "touch.h"
#include <stdlib.h>

#define __TOUCH_CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define __TOUCH_1IIR_CLT(K_IIR1, CURR, LAST) (((K_IIR1 * CURR) + ((1024 - K_IIR1) * LAST)) >> 10)

/// @brief 触摸基线校准用的pi计算
/// @param htouch 触摸句柄
/// @param error 误差，等于当前值-基线
/// @return void
static inline void __touch_pi_calculate(touch_struct_t *htouch, int32_t error)
{
    if (error == 0)
    {
        htouch->pi_integral = 0;
        return;
    }

    int32_t ib = htouch->pi_integral;
    ib = __TOUCH_CLAMP(ib + error, htouch->pi_integral_min, htouch->pi_integral_max);
    htouch->pi_integral = ib;
    htouch->baseline += error / htouch->pi_kp + ib / htouch->pi_ki;
}

/// @brief 触摸初始化
/// @param htouch 触摸句柄
/// @param touch_initstruct 触摸初始化结构体
/// @retval TOUCH_OK 初始化完成
/// @retval TOUCH_INVAL_VALUE 无效参数
/// @note 初始化后baseline和curr_value会在定时器回调中一起更新
TOUCH_STATE_T touch_init(touch_struct_t *htouch, touch_init_t *touch_initstruct)
{
    if (htouch == 0 || touch_initstruct == 0 || touch_initstruct->get_adc_value == 0)
    {
        return TOUCH_INVAL_VALUE;
    }

    htouch->handle_state = HTOUCH_NO_INIT;

    htouch->d_threa = touch_initstruct->d_threa;

    htouch->k_1iir = touch_initstruct->k_1iir;

    htouch->pi_kp = touch_initstruct->pi_kp;
    htouch->pi_ki = touch_initstruct->pi_ki;
    htouch->pi_integral_max = touch_initstruct->pi_integral_max;
    htouch->pi_integral_min = touch_initstruct->pi_integral_min;
    htouch->pi_integral = 0;

    htouch->is_touching = 0;
    htouch->curr_value = 0;
    htouch->t_value = 0;
    htouch->check_counts = 0;
    htouch->baseline = 0xFFFF;

    htouch->get_adc_value = touch_initstruct->get_adc_value;

    htouch->handle_state = HTOUCH_INITING;

    return TOUCH_OK;
}

/// @brief 获取当前触摸状态
/// @param htouch 触摸句柄
/// @param is_touching 输出触摸状态（0：未触摸，1：触摸）
/// @retval TOUCH_OK 正常
/// @retval TOUCH_INVAL_VALUE 无效参数
TOUCH_STATE_T touch_get_touch_state(touch_struct_t *htouch, uint8_t *is_touching)
{
    if (htouch == 0 || is_touching == 0)
    {
        if (is_touching != 0)
        {
            *is_touching = 0;
        }
        return TOUCH_INVAL_VALUE;
    }

    *is_touching = htouch->is_touching;

    return TOUCH_OK;
}

/// @brief 输出pwm的定时器的回调函数
/// @param htouch 触摸句柄
/// @retval TOUCH_OK 正常
/// @retval TOUCH_INVAL_VALUE 无效参数
/// @retval TOUCH_HANDLE_UNUSABLE 句柄不可用
/// @retval TOUCH_IS_BUSY 正在测量
/// @note 放到输出pwm给触摸节点的定时器的更新中断服务函数里
TOUCH_STATE_T touch_time_callback(touch_struct_t *htouch)
{
    if (htouch == 0 || htouch->get_adc_value == 0)
    {
        return TOUCH_INVAL_VALUE;
    }

    if (htouch->handle_state == HTOUCH_NO_INIT)
    {
        return TOUCH_HANDLE_UNUSABLE;
    }

    if (++(htouch->check_counts) < 10)
    {
        htouch->t_value += htouch->get_adc_value();
        return TOUCH_IS_BUSY;
    }
    htouch->check_counts = 0;
    htouch->t_value /= 10;

    int32_t t_curr_value = htouch->curr_value;
    t_curr_value = __TOUCH_1IIR_CLT(htouch->k_1iir, htouch->t_value, t_curr_value);
    htouch->curr_value = t_curr_value;

    int32_t t_d = t_curr_value - htouch->baseline;
    if (htouch->handle_state == HTOUCH_INITING && t_d == 0)
    {
        if (++htouch->init_counts > 10)
        {
            htouch->handle_state = HTOUCH_OK;
        }
        return TOUCH_OK;
    }
    if (htouch->handle_state != HTOUCH_OK)
    {
        htouch->init_counts = 0;
    }
    else
    {
        htouch->is_touching = (abs(t_d) > (int32_t)htouch->d_threa) ? 1 : 0;
    }

    if (htouch->is_touching == 0)
    {
        __touch_pi_calculate(htouch, t_d);
    }

    return TOUCH_OK;
}
