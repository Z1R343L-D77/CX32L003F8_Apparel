/**
  ******************************************************************************
  * @file           : bsp_key.h
  * @author         : 19816
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/20
  ******************************************************************************
  */

#ifndef INC_002_G_BSP_KEY_H
#define INC_002_G_BSP_KEY_H


#include "drv_key.h"

/* FD612按键位定义 */
#define FD612_KEY_SET_BIT   0x00010000  /* bit16: KS2 - SET键 */
#define FD612_KEY_DEC_BIT   0x00000002  /* bit6:  KS5 - DEC键 */
#define FD612_KEY_ADD_BIT   0x00000040  /* bit1:  KS6 - ADD键 */

void button_init_all(void);


#endif //INC_002_G_BSP_KEY_H
