/**
 * @file system_gpio.c
 * @author {Z1R343L} (Z1R343L@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-03
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "cx32l003_gpio.h"
#include "cx32l003_rcc.h"
#include "system_gpio.h"

/**
 * @brief FD612µÄGPIO³õÊ¼»¯
 * 
 */
void GPIO_Init_FD612(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(RCC, RCC_AHBPeriph_GPIOAEN, ENABLE);

    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

