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
#ifndef __TOUCH_H
#define __TOUCH_H

#include <stdint.h>

typedef enum
{
    HTOUCH_NO_INIT = 0, // 未初始化
    HTOUCH_INITING,     // 正在初始化
    HTOUCH_OK,          // 可用
} HTOUCH_STATE_T;

typedef enum
{
    TOUCH_OK = 0,          // 操作成功
    TOUCH_INVAL_VALUE,     // 无效参数
    TOUCH_HANDLE_UNUSABLE, // 句柄无法使用
    TOUCH_IS_BUSY,         // 正在忙
} TOUCH_STATE_T;

typedef uint32_t (*touch_func_t)(void);

typedef struct
{
    uint32_t d_threa; // 最大值差值阈值，当前最大值比基准最大值超过该值认为触摸

    uint32_t k_1iir; //  一阶iir滤波系数(0~1024。内部采用Q10计算，运算用的系数实际是k_iir/1024)

    int32_t pi_kp;           // p项系数（用于校准基线）。注意：比例输出的是：(p项)/(p项系数)
    int32_t pi_ki;           // i项系数（用于校准基线）。注意：积分输出的是：(i项)/(i项系数)
    int32_t pi_integral_max; // 积分最大值（用于校准基线）
    int32_t pi_integral_min; // 积分最小值（用于校准基线）

    touch_func_t get_adc_value; /*@brief 阻塞式获取adc原始值
                                 *@param void
                                 *@return adc原始值
                                 **/

} touch_init_t;

typedef struct
{
    volatile uint8_t is_touching;         // 是否触摸
    volatile HTOUCH_STATE_T handle_state; // 句柄状态

    uint32_t d_threa; // 最大值差值阈值

    uint32_t k_1iir; //  iir滤波系数(0~1024).内部采用Q10计算

    int32_t pi_kp;                // p项系数（用于pi控制器校准基准）。注意：比例输出的是：(p项)/(p项系数)
    int32_t pi_ki;                // i项系数（用于pi控制器校准基准）。注意：积分输出的是：(i项)/(i项系数)
    int32_t pi_integral_max;      // 积分最大值（用于pi控制器校准基准）
    int32_t pi_integral_min;      // 积分最小值（用于pi控制器校准基准）
    volatile int32_t pi_integral; // 误差累计值（用于pi控制器校准基准）

    volatile int32_t baseline;      // 未触摸时的基准
    volatile uint32_t t_value;      // 临时变量,原始adc值中值滤波用
    volatile int32_t curr_value;    // 当前触摸点测量值
    volatile uint32_t check_counts; // 采样计数（滤波用）
    volatile uint32_t init_counts;  // baseline初始化用的时间计数

    touch_func_t get_adc_value;

} touch_struct_t;

TOUCH_STATE_T touch_init(touch_struct_t *htouch, touch_init_t *touch_initstruct);
TOUCH_STATE_T touch_get_touch_state(touch_struct_t *htouch, uint8_t *is_touching);

TOUCH_STATE_T touch_time_callback(touch_struct_t *htouch);

#endif