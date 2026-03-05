/**
 * @file main.c
 * @author {Z1R343L} (Z1R343L@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-03
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "cx32l003.h"
#include "cx32l003_delay.h"

#include "system_gpio.h"
#include "system_it.h"

#include "app_scheduler.h"
#include "app_menu.h"


#include "bsp_led.H"
#include "bsp_key.h"
#include "bsp_smg.h"
#include "bsp_flash.h"

#include "drv_key.h"
#include "drv_fd612.H"

/* ===== main ===== */
int main(void)
{
    delay_init(4);
    GPIO_Init_FD612();
		FD612_Init();
		Para_Init();
		button_init_all();
		TIMER_Init();
		scheduler_init();
		app_menu_init();
    while(1)
		{
			scheduler_run();
    }
}






























