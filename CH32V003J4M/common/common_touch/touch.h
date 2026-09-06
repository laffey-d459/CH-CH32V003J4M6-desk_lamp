#ifndef __TOUCH_H
#define __TOUCH_H

#include <stdint.h>

typedef enum
{
    TOUCH_OK = 0,      // 操作成功
    TOUCH_INVAL_VALUE, // 无效参数
    TOUCH_BUSY,        // 正在忙
    TOUCH_TIMEOUT,     // 超时
} TOUCH_STATE_T;

typedef uint32_t (*touch_func_t)(uint8_t data);

typedef struct
{
    uint32_t d_threa; // 差值阈值，当前值与基准值之差超过该值认为发生触摸事件

    touch_func_t conf_tim_sr; /*@brief 设置定时器开关
                               *@param data 定时器状态（0关，1开）
                               *@return void
                               *@note 1.关闭时需清除定时器的count寄存器
                               **/

    touch_func_t get_tim_cvr; /*@brief 获取定时器捕获值
                               *@param void
                               *@return 定时器捕获值
                               **/

    touch_func_t conf_pin; /*@brief 配置gpio状态，准备/结束测量
                            *@param data 准备/结束测量 （0结束，1准备）
                            *@return void
                            *@note 1.结束测量：先关闭电容充电电源（如果有电源控制），
                            *                 再将定时器输入捕获引脚配置成推挽输出，并拉低
                            *                 最后再延时一段时间，确保电容完全放电，通常是us级的。
                            *      2.准备测量：先将定时器输入捕获引脚配置成浮空输入，
                            *                 再开启电容充电电源（如果有电源控制）
                            *      3.如果测量结果很飘，可以增大延时
                            **/

} touch_init_t;

typedef struct
{
    volatile uint8_t is_measuring; // 是否正在测量

    uint32_t d_threa;  // 差值阈值
    uint32_t baseline; // 未触摸时的基准CVR值

    touch_func_t conf_tim_sr;
    touch_func_t get_tim_cvr;
    touch_func_t conf_pin;
} touch_t;

TOUCH_STATE_T touch_init(touch_t *htouch, touch_init_t *touch_initstruch);
TOUCH_STATE_T touch_measure_touching(touch_t *htouch, uint8_t *is_touching);

TOUCH_STATE_T touch_measure_callback(touch_t *htouch);

#endif
