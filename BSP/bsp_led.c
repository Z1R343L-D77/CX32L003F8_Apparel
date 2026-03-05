/**
 * @FilePath     : /MDKc:/window_apps/desktop/汇邦/smg_cx32/BSP/bsp_led.c
 * @Description  : LED驱动实现（基于FD612，支持闪烁控制，10ms调用周期）
 * @Author       : Z1R343L 19816013818@163.com
 * @Version      : 1.1.0
 * @LastEditors  : Z1R343L 19816013818@163.com
 * @LastEditTime : 2026-03-04 00:00:00
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2026.
**/
#include "bsp_led.h"
#include "drv_fd612.h"
#include "stdio.h"

/* 全局变量 */
static uint8_t g_DigDat = 0x00;     /* DIG1的段数据缓存 */
static LED_Control_t led_ctrl[LED_MAX];  /* LED控制结构体数组 */

/**
 * @brief  LED硬件控制（底层操作，仅更新缓存，不触发刷新）
 * @param  led_type: LED类型
 * @param  state: 1=点亮，0=熄灭
 * @retval None
 */
static void LED_HW_Control(LED_Type_t led_type, uint8_t state)
{
    uint8_t bit_mask = 0;

    switch(led_type)
    {
        case LED_OUT: bit_mask = LED_OUT_BIT; break;
        case LED_AT:  bit_mask = LED_AT_BIT;  break;
        case LED_ALM: bit_mask = LED_ALM_BIT; break;
        default: return;
    }

    if(state != 0U)
        g_DigDat |= bit_mask;
    else
        g_DigDat &= ~bit_mask;
}

/**
 * @brief  LED控制状态机（10ms调用一次）
 * @param  p_ctrl: LED控制结构体指针
 * @retval None
 * @note   period单位为10ms，例如period=100表示1秒周期
 */
static void LED_Control_Work(LED_Control_t *p_ctrl)
{
    uint16_t on_time;

    /* 空指针检查 */
    if(p_ctrl == NULL)
    {
        return;
    }

    /* 复位逻辑 */
    if(p_ctrl->reset_flag == 1U)
    {
        p_ctrl->reset_flag = 0U;
        p_ctrl->cycle_cnt = 0U;
        p_ctrl->completed_times = 0U;
        p_ctrl->finish_flag = 0U;
    }

    /* 闪烁完成控制 */
    if(p_ctrl->completed_times >= p_ctrl->total_times)
    {
        p_ctrl->finish_flag = 1U;
        LED_HW_Control(p_ctrl->led_type, 0);  /* 完成后关闭 */
        return;
    }

    /* 周期计数（10ms递增） */
    p_ctrl->cycle_cnt++;

    /* 计算点亮时间（10ms单位） */
    on_time = (uint16_t)(p_ctrl->period * p_ctrl->duty_cycle);

    /* 点亮区间判断 */
    if(p_ctrl->cycle_cnt <= on_time)
    {
        LED_HW_Control(p_ctrl->led_type, 1);  /* 点亮 */
    }
    /* 熄灭区间判断 */
    else if(p_ctrl->cycle_cnt < p_ctrl->period)
    {
        LED_HW_Control(p_ctrl->led_type, 0);  /* 熄灭 */
    }
    /* 单周期完成，重置计数 */
    else
    {
        p_ctrl->cycle_cnt = 0U;
        p_ctrl->completed_times++;
    }
}

/**
 * @brief  LED参数动态配置
 * @param  led_type: LED类型
 * @param  period: 闪烁周期（10ms单位，例如100=1秒）
 * @param  duty_cycle: 占空比（0.0~1.0）
 * @param  total_times: 闪烁总次数（0=持续闪烁）
 * @retval None
 */
void LED_Setup(LED_Type_t led_type, uint32_t period, float duty_cycle, uint16_t total_times)
{
    if(led_type >= LED_MAX)
    {
        return;
    }

    led_ctrl[led_type].led_type    = led_type;
    led_ctrl[led_type].period      = period;
    led_ctrl[led_type].duty_cycle  = duty_cycle;
    led_ctrl[led_type].total_times = total_times;
    led_ctrl[led_type].reset_flag  = 1U;
}

/**
 * @brief  LED任务处理函数（10ms调用一次）
 * @retval None
 * @note   所有LED状态机更新完后统一刷新FD612一次，避免频繁刷新导致闪烁
 */
void LED_Task_Proc(void)
{
    uint8_t i;

    for(i = 0; i < LED_MAX; i++)
    {
        LED_Control_Work(&led_ctrl[i]);
    }

    /* 所有LED状态更新完毕，统一刷新一次 */
    FD612_PotiveTNage(LED_DIG_ADDR, g_DigDat);
    FD612_Refresh();
}

/* ===== 直接控制函数（保留兼容性，调用后需手动触发刷新或等待任务刷新） ===== */
void LED_OUT_On(void)  { LED_HW_Control(LED_OUT, 1); }
void LED_OUT_Off(void) { LED_HW_Control(LED_OUT, 0); }
void LED_AT_On(void)   { LED_HW_Control(LED_AT,  1); }
void LED_AT_Off(void)  { LED_HW_Control(LED_AT,  0); }
void LED_ALM_On(void)  { LED_HW_Control(LED_ALM, 1); }
void LED_ALM_Off(void) { LED_HW_Control(LED_ALM, 0); }

/**
 * @brief  LED初始化
 * @retval None
 */
void LED_Init(void)
{
    uint8_t i;

    /* FD612初始化 */
    FD612_DispBuff.DispSEG_MODE = FD612_8SEG_CMD;
    FD612_Command(FD612_8SEG_CMD);

    for(i = 0; i < 12; i++)
    {
        FD612_DispBuff.DispData[i] = 0x00;
    }

    FD612_DispStateWr(FD612_INTENS8 | FD612_DISP_ON);
    FD612_Refresh();

    /* 清空LED段数据 */
    g_DigDat = 0x00;
    FD612_PotiveTNage(LED_DIG_ADDR, g_DigDat);
    FD612_Refresh();

    /* 初始化LED控制结构体 */
    for(i = 0; i < LED_MAX; i++)
    {
        led_ctrl[i].led_type        = (LED_Type_t)i;
        led_ctrl[i].period          = 100U;   /* 默认1秒周期（100×10ms） */
        led_ctrl[i].duty_cycle      = 0.5f;
        led_ctrl[i].total_times     = 0U;
        led_ctrl[i].cycle_cnt       = 0U;
        led_ctrl[i].completed_times = 0U;
        led_ctrl[i].reset_flag      = 0U;
        led_ctrl[i].finish_flag     = 1U;
    }
}

