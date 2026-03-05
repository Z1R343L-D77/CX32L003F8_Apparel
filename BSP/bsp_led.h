/**
 * @FilePath     : /MDKc:/window_apps/desktop/汇邦/smg_cx32/BSP/bsp_led.h
 * @Description  : LED驱动头文件（基于FD612数码管驱动芯片）
 * @Author       : Z1R343L 19816013818@163.com
 * @Version      : 1.0.0
 * @LastEditors  : Z1R343L 19816013818@163.com
 * @LastEditTime : 2026-03-03 22:00:00
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2026.
**/
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stdint.h"

/* LED段bit定义 */
#define LED_OUT_BIT     0x08        /* c段=bit3 → 加热灯 */
#define LED_AT_BIT      0x04        /* d段=bit2 → 信息灯 */
#define LED_ALM_BIT     0x01        /* a段=bit0 → 报警灯 */

/* LED位地址 */
#define LED_DIG_ADDR    0x00        /* SM2 DIG1 → FD612_DIG1_ADDR=0 */

/* LED类型枚举 */
typedef enum {
    LED_OUT = 0,  /* 加热灯（c段） */
    LED_AT  = 1,  /* 信息灯（d段） */
    LED_ALM = 2,  /* 报警灯（a段） */
    LED_MAX = 3
} LED_Type_t;

/* LED控制结构体 */
typedef struct {
    LED_Type_t led_type;        /* LED类型 */
    uint32_t period;            /* 闪烁周期（ms单位） */
    uint16_t total_times;       /* 预设闪烁总次数 */
    uint16_t cycle_cnt;         /* 单周期计数器（ms） */
    uint16_t completed_times;   /* 已完成闪烁次数 */
    uint8_t reset_flag;         /* 复位标志（1=复位，0=正常） */
    uint8_t finish_flag;        /* 完成标志（1=结束，0=运行） */
    float duty_cycle;           /* 占空比（点亮时间百分比） */
} LED_Control_t;

/* 函数声明 */
void LED_Init(void);
void LED_OUT_On(void);
void LED_OUT_Off(void);
void LED_AT_On(void);
void LED_AT_Off(void);
void LED_ALM_On(void);
void LED_ALM_Off(void);

void LED_Setup(LED_Type_t led_type, uint32_t period, float duty_cycle, uint16_t total_times);
void LED_Task_Proc(void);

#endif /* __BSP_LED_H */
