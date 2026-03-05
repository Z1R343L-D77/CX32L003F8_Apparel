#ifndef _DRV_FLASH_H
#define _DRV_FLASH_H

#include "cx32l003_flash.h"
#include "stdint.h"

/* Flash最后一个扇区保存参数 */
#define PARA_FLASH_ADDR   0x0000FC00


/* API */
void Para_Init(void);
void Para_Save(void);

#endif
