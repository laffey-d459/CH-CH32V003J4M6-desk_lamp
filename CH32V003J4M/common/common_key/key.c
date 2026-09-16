/********************************************************************************************
 * file description: 按键扫描
 *
 * Version: V2.0.0
 * Copyright (c) 2026, 江协科技.
 * Licensed under the MIT License.
 *
 * Modify Record:
 * 2026/09/16  江协科技  V1.0.0  初始版本
 * 2026/09/20  laffey-d459  V2.0.0  封装成结构体，通过不同结构体进行操作
 ********************************************************************************************/
#include "key.h"

/// @brief 按键初始化
/// @param hkey 按键句柄
/// @param key_initstruct 按键初始化结构体
/// @retval KEY_OK 初始化完成
/// @retval KEY_INVAL_VALUE 无效参数
KEY_STATE_T key_init(key_struct_t *hkey, key_init_t *key_initstruct)
{
    if (hkey == 0 || key_initstruct == 0 || key_initstruct->get_state == 0)
    {
        return KEY_INVAL_VALUE;
    }

    hkey->key_flag = 0;
    hkey->curr_state = 0;
    hkey->prev_state = 0;
    hkey->sta_mac_state = 0;
    hkey->pressed_time = 0;

    hkey->time_double = key_initstruct->time_double;
    hkey->time_long = key_initstruct->time_long;
    hkey->time_repeat = key_initstruct->time_repeat;
    hkey->get_state = key_initstruct->get_state;

    return KEY_OK;
}

/// @brief 获取指定按键的标志位是否置位
/// @param hkey 按键句柄
/// @param flag 标志位。详见KEY_FLAG_T
/// @param p_is_set 是否按下
/// @retval 1：指定的标志位置位
/// @retval 0：指定的标志位未置位
KEY_STATE_T key_check_flag(key_struct_t *hkey, KEY_FLAG_T flag, uint8_t *p_is_set)
{
    if (hkey == 0 || p_is_set == 0)
    {
        if (p_is_set != 0)
        {
            *p_is_set = 0;
        }
        return KEY_INVAL_VALUE;
    }

    if (hkey->key_flag & flag)
    {
        if (flag != KEY_HOLD)
        {
            hkey->key_flag &= ~flag;
        }
        *p_is_set = 1;

        return KEY_OK;
    }
    *p_is_set = 0;

    return KEY_OK;
}

/// @brief 按键扫描
/// @param hkey 按键句柄
/// @retval KEY_OK 初始化完成
/// @retval KEY_INVAL_VALUE 无效参数
/// @note 定时1ms调用，内部自动进行20ms消抖
KEY_STATE_T key_scan(key_struct_t *hkey)
{
    if (hkey == 0 || hkey->get_state == 0)
    {
        return KEY_INVAL_VALUE;
    }

    // 按下时间递减
    if (hkey->pressed_time > 0)
    {
        --(hkey->pressed_time);
    }

    // 20ms消抖
    if (++hkey->debounce_cnt < 20)
    {
        return KEY_DEBOUNCING;
    }
    hkey->debounce_cnt = 0;

    // 读取状态
    hkey->prev_state = hkey->curr_state;
    hkey->curr_state = hkey->get_state();

    // HOLD 标志
    if (hkey->curr_state)
    {
        hkey->key_flag |= KEY_HOLD;
    }
    else
    {
        hkey->key_flag &= ~KEY_HOLD;
    }

    // DOWN / UP 边沿检测
    if (hkey->curr_state && !hkey->prev_state)
    {
        hkey->key_flag |= KEY_DOWN;
    }
    if (!hkey->curr_state && hkey->prev_state)
    {
        hkey->key_flag |= KEY_UP;
    }

    // 状态机
    switch (hkey->sta_mac_state)
    {
    case 0: // 空闲
        if (hkey->curr_state)
        {
            hkey->pressed_time = hkey->time_long;
            hkey->sta_mac_state = 1;
        }
        break;
    case 1: // 等待松开或长按
        if (!hkey->curr_state)
        {
            hkey->pressed_time = hkey->time_double;
            hkey->sta_mac_state = 2;
        }
        else if (hkey->pressed_time == 0)
        {
            hkey->pressed_time = hkey->time_repeat;
            hkey->key_flag |= KEY_LONG;
            hkey->sta_mac_state = 4;
        }
        break;
    case 2: // 等待双击或超时（单击）
        if (hkey->curr_state)
        {
            hkey->key_flag |= KEY_DOUBLE;
            hkey->sta_mac_state = 3;
        }
        else if (hkey->pressed_time == 0)
        {
            hkey->key_flag |= KEY_SINGLE;
            hkey->sta_mac_state = 0;
        }
        break;
    case 3: // 双击后等待松开
        if (!hkey->curr_state)
        {
            hkey->sta_mac_state = 0;
        }
        break;
    case 4: // 长按重复
        if (!hkey->curr_state)
        {
            hkey->sta_mac_state = 0;
        }
        else if (hkey->pressed_time == 0)
        {
            hkey->pressed_time = hkey->time_repeat;
            hkey->key_flag |= KEY_REPEAT;
        }
        break;
    default:
        hkey->sta_mac_state = 0;
        break;
    }

    return KEY_OK;
}
