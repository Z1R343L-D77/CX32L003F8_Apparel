/**
 * @file app_menu.h
 * @author {Z1R343L} (Z1R343L@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef _APP_MENU_H_
#define _APP_MENU_H_
 
#include "stdint.h"

typedef enum {
    DEFAULT_ER = 0,                 // 初始默认界面
    DEFAULT_SET,                // 默认界面参数设置
    PARA_PV,                     // 参数界面
    PARA_IV,                     // 参数界面
    PARA_DV,                     // 参数界面
    PARA_HC,                     // 参数界面
    PARA_IR,                     // 参数界面
    PARA_SP,                     // 参数界面
    PARA_PI,                     // 参数界面
    PARA_AH,                     // 参数界面    
    PARA_RV,                     // 参数界面
    PARA_BT,                     // 参数界面
    PARA_INV,                    // 参数界面   
    PARA_ATV,                    // 参数界面
    PARA_LKV,                    // 参数可写设置界面
		PARA_PV_SET,   
		PARA_IV_SET, 
		PARA_DV_SET, 
		PARA_HC_SET, 
		PARA_IR_SET,  
		PARA_SP_SET,    
		PARA_PI_SET,
		PARA_AH_SET,  
		PARA_RV_SET,
		PARA_BT_SET,
		PARA_INV_SET, 
		PARA_ATV_SET,    
    PARA_LKV_SET,             
}MenuEvent;

typedef struct para_set {
	uint8_t SV_set;
	uint8_t PV_set;
	uint8_t IV_set;
	uint8_t DV_set;
	uint8_t HC_set;
	uint8_t IR_set;
	uint8_t SP_set;
	uint8_t PI_set;
	uint8_t AH_set;
	uint8_t RV_set;
	uint8_t BT_set;
	uint8_t INV_set;
	uint8_t ATV_set;
	uint8_t LKV_set;
}para_set_num;
extern  para_set_num para;

/* ============================================
 * 参数表（顺序必须和MenuEvent的PARA_XX顺序一致）
 * ============================================ */
typedef struct {
    const char  *name;      /* 显示名称（上排） */
    uint8_t     *value;     /* 对应para结构体成员指针 */
    uint8_t      min;       /* 最小值 */
    uint8_t      max;       /* 最大值 */
} ParaItem_t;

static ParaItem_t para_table[] = {
    {" PV", &para.PV_set,  0, 255},  /* PARA_PV  */
    {" LV", &para.IV_set,  0, 255},  /* PARA_IV  */
    {" DV", &para.DV_set,  0, 255},  /* PARA_DV  */
    {" HC", &para.HC_set,  0, 255},  /* PARA_HC  */
    {" IR", &para.IR_set,  0, 255},  /* PARA_IR  */
    {" SP", &para.SP_set,  0, 255},  /* PARA_SP  */
    {" PI", &para.PI_set,  0, 255},  /* PARA_PI  */
    {" AH", &para.AH_set,  0, 255},  /* PARA_AH  */
    {" RV", &para.RV_set,  0, 255},  /* PARA_RV  */
    {" BT", &para.BT_set,  0, 255},  /* PARA_BT  */
    {"INV", &para.INV_set, 0, 255},  /* PARA_INV */
    {"ATV", &para.ATV_set, 0, 255},  /* PARA_ATV */
    {"LKV", &para.LKV_set, 0,   1},  /* PARA_LKV */
};

#define PARA_TABLE_SIZE  (sizeof(para_table) / sizeof(para_table[0]))
/* 获取当前参数表索引（PARA_PV=0, PARA_IV=1 ...） */
#define PARA_IDX(state)  ((state) - PARA_PV)
/* 判断是否在参数浏览态 */
#define IS_PARA_VIEW(s)  ((s) >= PARA_PV  && (s) <= PARA_LKV)
/* 判断是否在参数编辑态 */
#define IS_PARA_SET(s)   ((s) >= PARA_PV_SET && (s) <= PARA_LKV_SET)
/* 浏览态?编辑态互转 */
#define TO_SET(s)        ((s) - PARA_PV + PARA_PV_SET)
#define TO_VIEW(s)       ((s) - PARA_PV_SET + PARA_PV)

extern uint8_t menu_state;

void app_menu_init(void);
void task_app_menu_proc(void);




#endif
