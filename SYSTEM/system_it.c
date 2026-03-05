/**
 * @FilePath     : /MDKc:/window_apps/desktop/汇邦/smg_cx32/BSP/bsp_timer.c
 * @Description  : TIM1定时器驱动实现（1ms定时中断，4MHz系统时钟）
 * @Author       : Z1R343L 19816013818@163.com
 * @Version      : 1.0.0
 * @LastEditors  : Z1R343L 19816013818@163.com
 * @LastEditTime : 2026-03-03 21:00:00
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2026.
**/
#include "system_it.h"
#include "cx32l003_rcc.h"

/* 系统滴答计数器（1ms递增） */
static volatile uint32_t system_tick = 0;

/**
 * @brief  TIM1定时器初始化（1ms中断一次）
 * @param  None
 * @retval None
 * @note   系统时钟：4MHz (HIRC)
 *         定时器时钟：4MHz
 *         目标周期：1ms
 *         
 *         TIM1是16位高级定时器
 *         
 *         计算方案（预分频4-1=3）：
 *         4MHz / 4 = 1MHz
 *         1MHz * 0.001s = 1000
 *         配置：PSC=3, ARR=1000-1=999
 */
void TIMER_Init(void)
{
    /* 解锁寄存器 */
    RCC->UNLOCK = 0x55AAAA55;
    
    /* 使能TIM1时钟 (根据RCC文件，TIM1对应PWMCKEN) */
    RCC->PCLKEN |= (1 << 10);  /* RCC_APBPeriph_PWMCKEN bit10 */
    
    /* 锁定寄存器 */
    RCC->UNLOCK = 0x00000000;
    
    /* 配置TIM1基本参数 */
    TIM1->CR1 = 0;  /* 复位控制寄存器 */
    
    /* 设置预分频器：PSC = 3 (分频系数为4) */
    TIM1->PSC = 3;
    
    /* 设置自动重载值：ARR = 999 (1ms周期) */
    TIM1->ARR = 999;
    
    /* 清零计数器 */
    TIM1->CNT = 0;
    
    /* 配置CR1寄存器 */
    TIM1->CR1 |= (1 << 7);   /* TIM_CR1_ARPE: 使能自动重载预装载 */
    TIM1->CR1 &= ~(1 << 4);  /* TIM_CR1_DIR: 向上计数 */
    TIM1->CR1 &= ~(3 << 5);  /* TIM_CR1_CMS: 边沿对齐模式 */
    TIM1->CR1 &= ~(3 << 8);  /* TIM_CR1_CKD: 时钟分频因子=1 */
    
    /* 使能更新中断 */
    TIM1->DIER |= (1 << 0);  /* TIM_DIER_UIE */
    
    /* 生成更新事件，重载预分频器 */
    TIM1->EGR |= (1 << 0);   /* TIM_EGR_UG */
    
    /* 清除更新中断标志 */
    TIM1->SR &= ~(1 << 0);   /* TIM_SR_UIF */
    
    /* 配置NVIC */
    NVIC_SetPriority(TIM1_IRQn, 2);  /* 优先级2 */
    NVIC_EnableIRQ(TIM1_IRQn);       /* 使能中断 */
    
    /* 启动定时器 */
    TIM1->CR1 |= (1 << 0);   /* TIM_CR1_CEN */
}

/**
 * @brief  获取系统滴答计数（单位：ms）
 * @param  None
 * @retval 系统运行时间（ms）
 */
uint32_t TIMER_GetTick(void)
{
    return system_tick;
}
#include "bsp_led.h"
/**
 * @brief  TIM1中断服务函数
 * @param  None
 * @retval None
 * @note   每1ms进入一次
 */
void TIM1_IRQHandler(void)
{
    /* 检查更新中断标志 */
    if(TIM1->SR & (1 << 0))  /* TIM_SR_UIF */
    {
        /* 清除中断标志 */
        TIM1->SR &= ~(1 << 0);
        
        /* 系统滴答计数器递增 */
        system_tick++;
        /* 在这里添加1ms周期任务 */
    }
}
