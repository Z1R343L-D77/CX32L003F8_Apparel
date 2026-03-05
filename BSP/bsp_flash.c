#include "bsp_flash.h"
#include "app_menu.h"

/* 默认参数 */
static void Para_Default(void)
{
	para.SV_set  = 33;
	para.PV_set  = 0;
	para.IV_set  = 0;
	para.DV_set  = 0;
	para.HC_set  = 0;
	para.IR_set  = 0;
	para.SP_set  = 0;
	para.PI_set  = 0;
	para.AH_set  = 0;
	para.RV_set  = 0;
	para.BT_set  = 0;
	para.INV_set = 0;
	para.ATV_set = 0;
	para.LKV_set = 1;
}


/* 从Flash读取参数 */
static void Para_Load(void)
{
	int i ;
	uint32_t *src = (uint32_t*)PARA_FLASH_ADDR;
	uint32_t *dst = (uint32_t*)&para;

	for( i=0;i<(sizeof(para)+3)/4;i++)
	{
		dst[i] = src[i];
	}
}


/* 保存参数到Flash */
void Para_Save(void)
{
	int i;
	uint32_t addr = PARA_FLASH_ADDR;
	uint32_t *src = (uint32_t*)&para;

	/* 擦除扇区 */
	FLASH_EraseSector(addr);

	/* 写入数据 */
	for( i=0;i<(sizeof(para)+3)/4;i++)
	{
		FLASH_ProgramWord(addr, src[i]);
		addr += 4;
	}
}


/* 参数初始化 */
void Para_Init(void)
{
	Para_Load();

	/* 判断Flash是否未初始化 */
	if(para.LKV_set > 1)
	{
		Para_Default();
		Para_Save();
	}
}

