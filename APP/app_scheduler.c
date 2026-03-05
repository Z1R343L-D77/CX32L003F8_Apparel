/**
  ******************************************************************************
  * @file           : scheduler.c
  * @author         : 19816
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/20
  ******************************************************************************
  */
#include "app_scheduler.h"
#include "app_menu.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_smg.h"

uint8_t task_num;//存储任务数量
uint32_t now_time;//当前时间

typedef struct {
    void (*task_func)(void);
    uint32_t rate_ms;
    uint32_t last_run;
    
} task_t;
// 静态任务数组，每个任务包含任务函数、执行周期（毫秒）和上次运行时间（毫秒）
static task_t scheduler_task[] ={
		{button_ticks,				5,			0},
		{LED_Task_Proc,				10,			0},
		{task_smg_scan_proc,	1,			0},
    {task_app_menu_proc ,	10,			0}
};
/**
 * @brief 调度器初始化函数
 * 计算任务数组的元素个数，并将结果存储在 task_num 中
 */
void scheduler_init(void){
    task_num = sizeof(scheduler_task) / sizeof(task_t);
}
/**
 * @brief 调度器运行函数
 * 遍历任务数组，检查是否有任务需要执行。如果当前时间已经超过任务的执行周期，则执行该任务并更新上次运行时间
 */
void scheduler_run(void){
		uint8_t i = 0;
    for ( i = 0; i < task_num; i++){
        now_time = TIMER_GetTick();
        if (now_time >= scheduler_task[i].rate_ms + scheduler_task[i].last_run){
            scheduler_task[i].last_run = now_time;
            scheduler_task[i].task_func();
        }
    }
}


