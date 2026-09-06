#include "touch.h"
#include <string.h>

#define _TOUCH_SAMPLE_NUM (10)
#define _TOUCH_DISCARD_NUM (4)

/// @brief 单次测量timcvr
/// @param htouch 触摸句柄
/// @param out_cvr 输出值
/// @retval TOUCH_OK 初始化完成
/// @retval TOUCH_BUSY 已有项目正在测量
/// @retval TOUCH_TIMEOUT 测量超时
static TOUCH_STATE_T _measure_once(touch_t *htouch, uint32_t *out_cvr)
{
    if (htouch->is_measuring == 1)
    {
        return TOUCH_BUSY;
    }

    htouch->is_measuring = 1;
    htouch->conf_tim_sr(1);
    htouch->conf_pin(1);
    for (volatile uint32_t i = 0; i <= 0xFFFFF && htouch->is_measuring == 1; ++i)
    {
        ;
    }
    htouch->conf_tim_sr(0);
    htouch->conf_pin(0);

    if (htouch->is_measuring == 1)
    {
        htouch->is_measuring = 0;
        return TOUCH_TIMEOUT;
    }

    *out_cvr = htouch->get_tim_cvr(0);
    htouch->is_measuring = 0;
    return TOUCH_OK;
}

/// @brief 去除极值后的平均值滤波
/// @param htouch 触摸句柄
/// @return 滤波值
static uint32_t _measure_filtered(touch_t *htouch)
{
    uint32_t samples[_TOUCH_SAMPLE_NUM];
    uint32_t ok_cnt = 0;

    for (uint32_t i = 0; i < _TOUCH_SAMPLE_NUM; ++i)
    {
        if (_measure_once(htouch, &samples[ok_cnt]) == TOUCH_OK)
        {
            ok_cnt++;
        }
    }

    if (ok_cnt <= _TOUCH_DISCARD_NUM * 2)
    {
        return 0;
    }

    for (uint32_t i = 0; i < ok_cnt - 1; ++i)
    {
        for (uint32_t j = 0; j < ok_cnt - 1 - i; ++j)
        {
            if (samples[j] > samples[j + 1])
            {
                uint32_t tmp = samples[j];
                samples[j] = samples[j + 1];
                samples[j + 1] = tmp;
            }
        }
    }

    uint32_t sum = 0;
    for (uint32_t i = _TOUCH_DISCARD_NUM; i < ok_cnt - _TOUCH_DISCARD_NUM; ++i)
    {
        sum += samples[i];
    }
    return sum / (ok_cnt - _TOUCH_DISCARD_NUM * 2);
}

/// @brief 触摸初始化
/// @param htouch 触摸句柄
/// @param touch_initstruch 触摸初始化结构体
/// @retval TOUCH_OK 初始化完成
/// @retval TOUCH_INVAL_VALUE 无效参数
/// @note 务必先开启捕获中断
TOUCH_STATE_T touch_init(touch_t *htouch, touch_init_t *touch_initstruch)
{
    if (htouch == 0 || touch_initstruch == 0 ||
        touch_initstruch->conf_pin == 0 || touch_initstruch->conf_tim_sr == 0 ||
        touch_initstruch->get_tim_cvr == 0)
    {
        return TOUCH_INVAL_VALUE;
    }

    htouch->is_measuring = 0;
    htouch->baseline = 0;

    htouch->conf_tim_sr = touch_initstruch->conf_tim_sr;
    htouch->get_tim_cvr = touch_initstruch->get_tim_cvr;
    htouch->conf_pin = touch_initstruch->conf_pin;
    htouch->d_threa = touch_initstruch->d_threa;

    htouch->conf_tim_sr(0);
    htouch->conf_pin(0);

    htouch->baseline = _measure_filtered(htouch);

    return TOUCH_OK;
}

/// @brief 测量是否发生触摸事件
/// @param htouch 触摸句柄
/// @param is_touching 是否发生触摸事件（0非，1是）
/// @retval TOUCH_OK 测量成功
/// @retval TOUCH_INVAL_VALUE 无效参数
/// @note 正常情况测量时间估算：大于TOUCH_SAMPLE_NUM*(conf_pin()里的引脚延时间)
TOUCH_STATE_T touch_measure_touching(touch_t *htouch, uint8_t *is_touching)
{
    if (htouch == 0 || is_touching == 0 ||
        htouch->conf_pin == 0 || htouch->conf_tim_sr == 0)
    {
        return TOUCH_INVAL_VALUE;
    }

    uint32_t avg = _measure_filtered(htouch);

    if (avg > htouch->baseline && (avg - htouch->baseline) > htouch->d_threa)
    {
        *is_touching = 1;
    }
    else
    {
        *is_touching = 0;
        htouch->baseline = (htouch->baseline * 7 + avg) / 8;
    }

    return TOUCH_OK;
}

/// @brief 触摸测量事件回调函数
/// @param htouch 触摸句柄
/// @retval TOUCH_OK 回调成功
/// @retval TOUCH_INVAL_VALUE 无效参数
/// @note 需要放到捕获中断中
TOUCH_STATE_T touch_measure_callback(touch_t *htouch)
{
    if (htouch == 0)
    {
        return TOUCH_INVAL_VALUE;
    }

    htouch->is_measuring = 0;

    return TOUCH_OK;
}