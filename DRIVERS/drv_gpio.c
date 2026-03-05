/**
  ******************************************************************************
  * @file           : drv_gpio.c
  * @author         : 19816
  * @brief          : GPIO驱动（LED控制）实现文件
  * @attention      : 符合MISRA-C 2012规范，适配AT32F402
  * @date           : 2025/1/20
  ******************************************************************************
  */
#include "drv_gpio.h"
#include "string.h"

/* MISRA规则：全局变量加static限制作用域，禁止裸全局暴露 */
static GPIO_Control_t led2;
static GPIO_Control_t led3;

/**
 * @brief  GPIO初始化（LED控制结构体初始化）
 * @note   符合MISRA，消除魔法数字，字段语义清晰
 * @retval None
 */
void drv_gpio_init(void)
{
    /* LED2初始化 - 保留原有参数，字段名规范化 */
    led2.port = GPIOF;
    led2.pin = GPIO_PINS_4;
    led2.active_level = GPIO_LOW;
    led2.period = 40U;                // MISRA：无符号后缀U
    led2.duty_cycle = 0.5f;
    led2.total_times = 5U;
    led2.reset_flag = 1U;

    /* LED3初始化 */
    led3.port = GPIOF;
    led3.pin = GPIO_PINS_5;
    led3.active_level = GPIO_LOW;
    led3.period = 40U;
    led3.duty_cycle = 0.5f;
    led3.total_times = 5U;
    led3.reset_flag = 1U;
}

/**
 * @brief  GPIO控制状态机（5ms调用一次）
 * @note   符合MISRA：空指针检查、消除语法错误、逻辑无歧义
 * @param  p_ctrl: GPIO控制结构体指针（MISRA：指针参数加p_前缀）
 * @retval None
 */
static void gpio_control_work(GPIO_Control_t *p_ctrl)
{
    /* MISRA规则：空指针检查（必加） */
    if (p_ctrl == NULL) {
        return;
    }

    /* 复位逻辑 */
    if (p_ctrl->reset_flag == 1U) {
        p_ctrl->reset_flag = 0U;
        p_ctrl->cycle_cnt = 0U;
        p_ctrl->completed_times = 0U;
        p_ctrl->finish_flag = 0U;
    }

    /* 闪烁完成控制 */
    if (p_ctrl->completed_times >= p_ctrl->total_times) {
        p_ctrl->finish_flag = 1U;
        /* MISRA：补充分号，逻辑对齐，消除注释混乱 */
        if (p_ctrl->active_level == GPIO_HIGH) {
            GPIO_OFF(p_ctrl);  // 高电平点亮 → 完成后关闭
        } else {
            GPIO_ON(p_ctrl);   // 低电平点亮 → 完成后关闭
        }
        return;
    }

    /* 周期计数（5ms递增） */
    p_ctrl->cycle_cnt++;

    /* 点亮区间判断 - MISRA：括号包裹表达式，避免优先级问题 */
    if (p_ctrl->cycle_cnt <= (uint16_t)(p_ctrl->period * p_ctrl->duty_cycle)) {
        if (p_ctrl->active_level == GPIO_HIGH) {
            GPIO_ON(p_ctrl);   // 高电平点亮
        } else {
            GPIO_OFF(p_ctrl);  // 低电平点亮
        }
    }
    /* 熄灭区间判断 */
    else if (p_ctrl->cycle_cnt < p_ctrl->period) {
        if (p_ctrl->active_level == GPIO_HIGH) {
            GPIO_OFF(p_ctrl);  // 高电平点亮 → 熄灭
        } else {
            GPIO_ON(p_ctrl);   // 低电平点亮 → 熄灭
        }
    }
    /* 单周期完成，重置计数 */
    else {
        p_ctrl->cycle_cnt = 0U;
        p_ctrl->completed_times++;
    }
}

/**
 * @brief  LED2参数动态配置
 * @note   符合MISRA：参数命名语义化，无隐式转换
 * @param  period: 闪烁周期（5ms单位）
 * @param  duty_cycle: 占空比（0~1）
 * @param  total_times: 闪烁总次数
 * @retval None
 */
void led2_setup(uint32_t period, float duty_cycle, uint16_t total_times)
{
    led2.period = period;
    led2.duty_cycle = duty_cycle;
    led2.total_times = total_times;
    led2.reset_flag = 1U;
}

/**
 * @brief  LED3参数动态配置
 * @note   同LED2，符合MISRA规范
 * @param  period: 闪烁周期
 * @param  duty_cycle: 占空比
 * @param  total_times: 闪烁总次数
 * @retval None
 */
void led3_setup(uint32_t period, float duty_cycle, uint16_t total_times)
{
    led3.period = period;
    led3.duty_cycle = duty_cycle;
    led3.total_times = total_times;
    led3.reset_flag = 1U;
}

/**
 * @brief  LED任务处理函数（5ms调用）
 * @note   符合MISRA：函数体简洁，仅调用核心逻辑
 * @retval None
 */
void task_led_proc(void)
{
    gpio_control_work(&led2);
    gpio_control_work(&led3);
}