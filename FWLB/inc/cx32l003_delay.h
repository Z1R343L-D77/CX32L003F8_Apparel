#ifndef __CX32L003_DELAY_H
#define __CX32L003_DELAY_H			   
	  
/**
  ******************************************************************************
  * @file    cx32l003_delay.h
  * @author  MCU Application Team
  * @version V0.0.0
  * @date    09-17-2019
  * @brief   This file contains all the functions prototypes for the delay firmware 
  *          library.
  ******************************************************************************
  */
  
#include "cx32l003.h"
#include "cx32l003_conf.h"
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(uint32_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif





























