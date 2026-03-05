/**
  *****************************************************************************
  * @file    cx32l003_exit.h
  * @author  MCU Application Team
  * @version V0.0.0
  * @date    27-July-2018
  * @brief   This file contains all the functions prototypes for the EXTI 
	           firmware  library.          
  *****************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CX32l003_EXTI__H
#define __CX32l003_EXTI__H

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
/* Includes ------------------------------------------------------------------*/
#include "cx32l003.h"
#include "cx32l003_conf.h"

/** @defgroup GPI0_IRQ_InitTypeDef
  * @{
  */ 
typedef struct
{
  uint8_t GPI0_IRQ_Pin_Enable;         /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPI0_IRQ_Pin_Enable */

  uint8_t GPI0_IRQ_Pin_Clear;          /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Clear */

  uint8_t GPI0_IRQ_Pin_Type;           /*!< Specifies the speed for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Type */

  uint8_t GPI0_IRQ_Pin_Polarity;       /*!< Specifies the operating output type for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Polarity */

  uint8_t GPI0_IRQ_Pin_Edge;           /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Edge */
}GPI0_IRQ_InitTypeDef;


/** @defgroup EXTI_Exported_Functions
  * @{
  */

void GPIO_EXTILineConfig(GPIO_TypeDef* GPIOx,GPI0_IRQ_InitTypeDef* GPIO_IRQ_InitStructure, uint8_t GPIO_Pin);


#ifdef __cplusplus
}
#endif

#endif /*__CX32l003_EXTI__H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************(C)COPYRIGHT CX*********END OF FILE***************************************/
