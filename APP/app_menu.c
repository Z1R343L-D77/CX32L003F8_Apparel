/**
 * @file app_menu.c
 * @author {Z1R343L} (Z1R343L@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "app_menu.h"
#include "bsp_smg.H"

uint8_t menu_state = 0;
para_set_num para;

void app_menu_init(void)
{
		smg_disp_proc("88.8","88.8",0,0);	task_smg_scan_proc();delay_ms(500);
		smg_disp_proc("HB ","539",0,0);		task_smg_scan_proc();delay_ms(500);
		smg_disp_proc("999","-99",0,0);		task_smg_scan_proc();delay_ms(500);
		smg_disp_proc("G69","000",0,0);		task_smg_scan_proc();delay_ms(500);
}

void task_app_menu_proc(void)
{
    char s2[8] = {0};
    uint8_t idx;

    switch(menu_state)
    {
        case DEFAULT_ER:
						sprintf(s2, "%3u", para.SV_set);
            smg_disp_proc("ER.1", s2,0,0);  //上面闪烁
            break;

        case DEFAULT_SET:
            sprintf(s2, "%3u", para.SV_set);
            smg_disp_proc(" SV", s2,0,1);		//下面闪烁
            break;

        default:
            if(IS_PARA_VIEW(menu_state))
            {
                idx = PARA_IDX(menu_state);
								sprintf(s2, "%3u", *para_table[idx].value);
                smg_disp_proc(para_table[idx].name, s2,0,0);//无闪烁
            }
            else if(IS_PARA_SET(menu_state))
            {
                idx = PARA_IDX(TO_VIEW(menu_state));
                sprintf(s2, "%3u", *para_table[idx].value);
                smg_disp_proc(para_table[idx].name, s2,0,1);//下面闪烁
            }
            break;
    }
}
