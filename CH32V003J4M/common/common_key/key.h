#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>

typedef enum
{
    KEY_OK = 0,      // 操作成功
    KEY_INVAL_VALUE, // 无效参数
    KEY_DEBOUNCING,  // 消抖中
} KEY_STATE_T;

/// HOLD、DOWN、UP :在任何时刻，只要检测到对应的事件，就会置标志位。
/// SINGLE、DOUBLE、LONG/REPEAT :三者互斥，一次完整的按键流程，只会置其中一类标志位。
/// HOLD 自动置1和清0 :其余标志位在检测到指定事件的时刻置1，读后清0。
typedef enum
{
    KEY_HOLD = 0x01,   // (按住不放) 按键按住不放时置1，按键松开时置0
    KEY_DOWN = 0x02,   // (按下时刻) 按键按下的时刻置1
    KEY_UP = 0x04,     // (松开时刻) 按键松开的时刻置1
    KEY_SINGLE = 0x08, // (单击) 按键按下松开后，没有再次按下，超过双击时间阈值的时刻置1
    KEY_DOUBLE = 0x10, // (双击) 按键按下松开后，在双击时间阈值内再次按下的时刻置1
    KEY_LONG = 0x20,   // (长按) 按键按住不放，超过长按时间阈值的时刻置1
    KEY_REPEAT = 0x40  // (重复) 按键长按后，每隔重复时间阈值置一次1，直到按键松开
} KEY_FLAG_T;

typedef uint8_t (*key_func_t)(void);

typedef struct
{
    uint16_t time_double; // 双击最大间隔（ms）
    uint16_t time_long;   // 长按阈值（ms）
    uint16_t time_repeat; // 长按重复间隔（ms）

    key_func_t get_state; /*@brief 获取按键状态
                           *@param void
                           *@retval 0未按下
                           *@retval 1已按下
                           */

} key_init_t;

typedef struct
{
    volatile uint8_t key_flag;      // 按键的状态标志
    volatile uint8_t curr_state;    // 按键当前的状态
    volatile uint8_t prev_state;    // 按键之前的状态
    volatile uint8_t sta_mac_state; // 状态机的状态
    volatile uint8_t debounce_cnt;  // 消抖计数
    volatile uint16_t pressed_time; // 按键按下的时间

    uint16_t time_double; // 双击最大间隔（ms，最小20ms）
    uint16_t time_long;   // 长按阈值（ms，最小20ms）
    uint16_t time_repeat; // 长按重复间隔（ms，最小20ms）

    key_func_t get_state; // 获取按键状态的回调函数

} key_struct_t;

KEY_STATE_T key_init(key_struct_t *hkey, key_init_t *key_initstruct);
KEY_STATE_T key_check_flag(key_struct_t *hkey, KEY_FLAG_T flag, uint8_t *p_is_set);
KEY_STATE_T key_scan(key_struct_t *hkey);

#endif
