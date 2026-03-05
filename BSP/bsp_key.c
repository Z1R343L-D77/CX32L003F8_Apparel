/**
  ******************************************************************************
  * @file           : bsp_key.c
  * @author         : 19816
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/20
  ******************************************************************************
  */
#include "bsp_key.h"
#include "bsp_led.h"
#include "drv_fd612.h"
#include "app_menu.h"
#include "bsp_flash.h"
static uint8_t key_set_long_cnt = 0;
static uint8_t key_set_lock = 1;//1上锁
static para_set_num para_backup;//保存备份值

struct Button btn1;
struct Button btn2;
struct Button btn3;

enum {
    GPIO_LOW_KEY = 0,  // 低电平
    GPIO_HIGH_KEY 	   // 高电平
};

enum Button_IDs {
    btn1_id=1,
    btn2_id,
    btn3_id,
};
/**
  * @brief  read_button_GPIO 按键读取函数
  * @note   None
  * @param  button_id 
  * @retval None
  */
//uint8_t read_button_GPIO(uint8_t button_id)
//{
//    // you can share the GPIO read function with multiple Buttons
//    switch(button_id)
//    {
//        case btn1_id:
//            return gpio_input_data_bit_read(GPIOA, GPIO_PINS_0);

//        default:
//            return 0;
//    }
//}
/**
 * @brief  读取按键GPIO状态（用于MultiButton库）
 * @param  button_id: 按键ID
 * @retval 1=按下, 0=释放
 * @note   FD612按键通过I2C读取，不是直接GPIO
 */
uint8_t read_button_GPIO(uint8_t button_id)
{
    static INT32U last_key_data = 0;
    INT32U key_data;
    
    /* 读取FD612按键数据 */
    key_data = FD612_RdKey();
    last_key_data = key_data;
    
    /* 根据button_id返回对应按键状态 */
    switch(button_id)
    {
        case 1:  /* SET键 */	
            return (key_data & FD612_KEY_SET_BIT) ? 1 : 0;  /* 按下返回0 */
            
        case 2:  /* DEC键 */
            return (key_data & FD612_KEY_DEC_BIT) ? 1 : 0;
            
        case 3:  /* ADD键 */
            return (key_data & FD612_KEY_ADD_BIT) ? 1 : 0;
            
        default:
            return 0;  /* 默认返回释放状态 */
    }
}
/**
  * @brief  button_callback 按键控制回调函数
  * @note   _btn->event 按键状态 _btn->buttun_id 按键编号
  * @param  void* btn 回调句柄
  * @retval None
  */
void button_callback(void* btn) {
    struct Button *_btn = btn;

    switch (_btn->event) {
        case PRESS_DOWN:
            break;
        case PRESS_UP:
            switch (_btn->button_id) {
                case btn1_id:
											LED_Setup(LED_AT,5u,1.0f,1);
											switch(menu_state)
											{
													case DEFAULT_ER:   menu_state = DEFAULT_SET; break;
													case DEFAULT_SET:  menu_state = DEFAULT_ER; Para_Save(); break;
													case PARA_PV:   
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_PV_SET;
																} 
													break;
													case PARA_IV:   
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_IV_SET;
																} 
													break;
													case PARA_DV:     
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_DV_SET;
																} 
													break;
													case PARA_HC:    
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_HC_SET;
																} 
													break;
													case PARA_IR:     
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_IR_SET;
																} 
													break;
													case PARA_SP:    
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_SP_SET;
																} 
													break;
													case PARA_PI:   
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_PI_SET;
																} 													
													break;
													case PARA_AH:   
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_AH_SET;
																} 
													break;
													case PARA_RV:   
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_RV_SET;
																} 
													break;
													case PARA_BT:    
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_BT_SET;
																} 
													break;
													case PARA_INV:  
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_INV_SET;
																} 
													break;
													case PARA_ATV:  
																if(para.LKV_set){
																	para_backup = para;
																	menu_state = PARA_ATV_SET;
																} 
													break;
													case PARA_LKV:
																	para_backup = para;
																	menu_state = PARA_LKV_SET;
																break;
													case PARA_PV_SET:  menu_state = PARA_PV; Para_Save(); break;
													case PARA_IV_SET:  menu_state = PARA_IV; Para_Save(); break;
													case PARA_DV_SET:  menu_state = PARA_DV; Para_Save(); break;
													case PARA_HC_SET:  menu_state = PARA_HC; Para_Save(); break;
													case PARA_IR_SET:  menu_state = PARA_IR; Para_Save(); break;
													case PARA_SP_SET:  menu_state = PARA_SP; Para_Save(); break;
													case PARA_PI_SET:  menu_state = PARA_PI; Para_Save(); break;
													case PARA_AH_SET:  menu_state = PARA_AH; Para_Save(); break;
													case PARA_RV_SET:  menu_state = PARA_RV; Para_Save(); break;
													case PARA_BT_SET:  menu_state = PARA_BT; Para_Save(); break;
													case PARA_INV_SET: menu_state = PARA_INV;Para_Save(); break;
													case PARA_ATV_SET: menu_state = PARA_ATV;Para_Save(); break;
													case PARA_LKV_SET: menu_state = PARA_LKV;Para_Save(); break;

													default:
															break;
											}
											break;
										/* ============================================
										 * btn2 = DEC键
										 * 默认界面：SV-1
										 * 参数浏览：上一个参数
										 * 参数编辑：当前参数值-1
										 * ============================================ */
										case btn2_id:
												LED_Setup(LED_AT, 5u, 1.0f, 1);
												switch(menu_state)
												{
														case DEFAULT_SET:
																if(para.SV_set > 0) para.SV_set--;
																break;
														default:
																if(IS_PARA_VIEW(menu_state))
																{
																		/* 参数浏览：上一个 */
																		if(menu_state > PARA_PV) menu_state--;
																}
																else if(IS_PARA_SET(menu_state))
																{
																		/* 参数编辑：值-1 */
																		uint8_t idx = PARA_IDX(TO_VIEW(menu_state));
																		if(*para_table[idx].value > para_table[idx].min)
																				(*para_table[idx].value)--;
																}
																break;
												}
												break;

										/* ============================================
										 * btn3 = ADD键
										 * 默认界面：SV+1
										 * 参数浏览：下一个参数
										 * 参数编辑：当前参数值+1
										 * ============================================ */
										case btn3_id:
												LED_Setup(LED_AT, 5u, 1.0f, 1);
												switch(menu_state)
												{
														case DEFAULT_SET:
																if(para.SV_set < 255) para.SV_set++;
																break;

														default:
																if(IS_PARA_VIEW(menu_state))
																{
																		/* 参数浏览：下一个 */
																		if(menu_state < PARA_LKV) menu_state++;
																}
																else if(IS_PARA_SET(menu_state))
																{
																		/* 参数编辑：值+1 */
																		uint8_t idx = PARA_IDX(TO_VIEW(menu_state));
																		if(*para_table[idx].value < para_table[idx].max)
																				(*para_table[idx].value)++;
																}
																break;
												}
												break;
                default:
                    // 处理无效按键情况，例如记录日志或返回
                    break;
            }
            break;
        case PRESS_REPEAT:
            break;
        case SINGLE_CLICK:
            break;
        case DOUBLE_CLICK:
            break;
        case LONG_PRESS_START:
            switch (_btn->button_id) {
									case btn1_id:
											key_set_long_cnt = 0;
											key_set_lock = 0;//解锁
									break;
							default:
								break;
						}
            break;
        case LONG_PRESS_HOLD:
					  switch (_btn->button_id) {
                case btn1_id://按键1长按切换默认模式与参数模式
									if(!key_set_lock)
									{
										if(key_set_long_cnt++>=44)//5ms*44=2200ms+800ms = 3s
										{
												LED_Setup(LED_AT,5u,1.0f,1);
												/* 如果在参数编辑界面 */
												if(IS_PARA_SET(menu_state))
												{
														para = para_backup;              //恢复原参数
														menu_state -= (PARA_PV_SET-PARA_PV); //返回浏览界面
												}
												else
												{
														switch(menu_state)
														{
																case DEFAULT_ER:
																case DEFAULT_SET:
																		menu_state = PARA_PV;
																		break;

																default:
																		menu_state = DEFAULT_ER;
																		break;
														}
												}
												key_set_long_cnt = 0;
												key_set_lock = 1;
										}
									}

								break;
										/* ============================================
										 * btn2 = DEC键
										 * 默认界面：SV-1
										 * 参数浏览：上一个参数
										 * 参数编辑：当前参数值-1
										 * ============================================ */
										case btn2_id:
												LED_Setup(LED_AT, 5u, 1.0f, 1);
												switch(menu_state)
												{
														case DEFAULT_SET:
																if(para.SV_set > 0) para.SV_set--;
																break;
														default:
																if(IS_PARA_SET(menu_state))
																{
																		/* 参数编辑：值-1 */
																		uint8_t idx = PARA_IDX(TO_VIEW(menu_state));
																		if(*para_table[idx].value > para_table[idx].min)
																				(*para_table[idx].value)--;
																}
																break;
												}
												break;
										/* ============================================
										 * btn3 = ADD键
										 * 默认界面：SV+1
										 * 参数浏览：下一个参数
										 * 参数编辑：当前参数值+1
										 * ============================================ */
										case btn3_id:
												LED_Setup(LED_AT, 5u, 1.0f, 1);
												switch(menu_state)
												{
														case DEFAULT_SET:
																if(para.SV_set < 255) para.SV_set++;
																break;

														default:
																if(IS_PARA_SET(menu_state))
																{
																		/* 参数编辑：值+1 */
																		uint8_t idx = PARA_IDX(TO_VIEW(menu_state));
																		if(*para_table[idx].value < para_table[idx].max)
																				(*para_table[idx].value)++;
																}
																break;
												}
												break;
            }
            break;
    }
}

/**
  * @brief  button_init_all 按键句柄初始化集合
  * @note   None
  * @param  None
  * @retval None
  */
void button_init_all(void)
{
    button_init(&btn1, read_button_GPIO, GPIO_HIGH_KEY, btn1_id);
//    button_attach(&btn1, PRESS_DOWN,       button_callback);
    button_attach(&btn1, PRESS_UP,         button_callback);
//	button_attach(&btn1, PRESS_REPEAT,     button_callback);
//	button_attach(&btn1, SINGLE_CLICK,     button_callback);
//	button_attach(&btn1, DOUBLE_CLICK,     button_callback);
		button_attach(&btn1, LONG_PRESS_START, button_callback);
		button_attach(&btn1, LONG_PRESS_HOLD, button_callback);
    button_start(&btn1);
	
    button_init(&btn2, read_button_GPIO, GPIO_HIGH_KEY, btn2_id);
    button_attach(&btn2, PRESS_UP,       button_callback);
		button_attach(&btn2, LONG_PRESS_HOLD, button_callback);
	  button_start(&btn2);

    button_init(&btn3, read_button_GPIO, GPIO_HIGH_KEY, btn3_id);
    button_attach(&btn3, PRESS_UP,       button_callback);
		button_attach(&btn3, LONG_PRESS_HOLD, button_callback);
	  button_start(&btn3);
	
	
//		button_init(&btn2, read_button_GPIO, GPIO_LOW_KEY, btn2_id);
//		button_attach(&btn2, PRESS_DOWN,       button_callback);
//		button_attach(&btn2, PRESS_UP,         button_callback);
//		button_attach(&btn2, PRESS_REPEAT,     button_callback);
//		button_attach(&btn2, SINGLE_CLICK,     button_callback);
//		button_attach(&btn2, DOUBLE_CLICK,     button_callback);
//		button_attach(&btn2, LONG_PRESS_START, button_callback);
//		button_start(&btn2);

//    //for example
//    button_attach(&btn2, PRESS_DOWN,       BTN2_PRESS_DOWN_Handler);
//    button_attach(&btn2, PRESS_UP,         BTN2_PRESS_UP_Handler);
//    button_attach(&btn2, PRESS_REPEAT,     BTN2_PRESS_REPEAT_Handler);
//    button_attach(&btn2, SINGLE_CLICK,     BTN2_SINGLE_Click_Handler);
//    button_attach(&btn2, DOUBLE_CLICK,     BTN2_DOUBLE_Click_Handler);
//    button_attach(&btn2, LONG_PRESS_START, BTN2_LONG_PRESS_START_Handler);
//    button_attach(&btn2, LONG_PRESS_HOLD,  BTN2_LONG_PRESS_HOLD_Handler);
}
