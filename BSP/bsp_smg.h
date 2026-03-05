/**
 * @FilePath     : bsp_smg.h
 * @Description  : 数码管驱动头文件（FD612，双三位数码管）
 * @Author       : Z1R343L 19816013818@163.com
 * @Version      : 2.0.0
 * @LastEditors  : Z1R343L 19816013818@163.com
 * @LastEditTime : 2026-03-04
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2026.
 *
 * 使用方式：
 *   1. 业务层调用 smg_disp_proc(g1_str, g2_str) 设置显示内容
 *   2. 调度器每 100ms 调用一次 task_smg_proc() 刷新硬件
 *
 * 字符串格式：
 *   每组最多3个字符，'.'紧跟在字符后表示小数点
 *   支持字符：0-9, -, A, b, C, d, E, F, H, L, P, r, n, ' '(空格)
 *   例如："12.3"  → 显示 1  2. 3
 *        " 25"   → 显示 空格 2 5（前导空格）
 *        "E  "   → 只显示E，其余熄灭
**/

#ifndef __BSP_SMG_H__
#define __BSP_SMG_H__

#include "drv_fd612.h"
#include <stdio.h>

/* ============================================
 * 类型定义
 * ============================================ */


/* ============================================
 * FD612 显示命令
 * ============================================ */
#define FD612_ADDR_INC_CMD      0x40U   /* 地址自动递增写模式 */
#define FD612_ADDR_BASE         0xC0U   /* 显示地址基址 */
#define FD612_DISP_ON_MAX       0x8FU   /* 开显示，最大亮度 */


/* ============================================
 * 显示缓冲区大小（FD612共12字节）
 * ============================================ */
#define FD612_BUF_SIZE          12U

/* ============================================
 * 位选值定义
 *
 * 硬件接线（实测）：
 *
 * 第一组数码管（3位，上排）
 *   位选引脚：D8 D7 D6 D5
 *   a~d 段选在 buf[8]~buf[11]，位选值：
 *     第1位=0x20, 第2位=0x40, 第3位=0x80
 *   e/f/g 段选在 buf[3]/buf[2]/buf[0]，位选值：
 *     第1位=0x02, 第2位=0x04, 第3位=0x08
 *   小数点(dp)：buf[1] |= 0x04（仅中间位）
 *
 * 第二组数码管（3位，下排）
 *   位选引脚：D9 D10 D11 D12
 *   a~d 段选在 buf[4]~buf[7]，位选值：
 *     第1位=0x10, 第2位=0x20, 第3位=0x40
 *   e/f/g 段选共用 buf[3]/buf[2]/buf[0]，位选值：
 *     第1位=0x10, 第2位=0x20, 第3位=0x40
 *   小数点(dp)：buf[1] |= 0x20（仅中间位）
 * ============================================ */

/* 第一组位选值（a/b/c/d段） */
#define G1_POS0     0x20U
#define G1_POS1     0x40U
#define G1_POS2     0x80U

/* 第一组位选值（e/f/g段） */
#define G1_EFG0     0x02U
#define G1_EFG1     0x04U
#define G1_EFG2     0x08U

/* 第二组位选值（a/b/c/d段） */
#define G2_POS0     0x10U
#define G2_POS1     0x20U
#define G2_POS2     0x40U

/* 第二组位选值（e/f/g段） */
#define G2_EFG0     0x10U
#define G2_EFG1     0x20U
#define G2_EFG2     0x40U

/* 小数点（dp） */
#define G1_DP_MID   0x04U   /* 第一组中间位小数点，写入 buf[1] */
#define G2_DP_MID   0x20U   /* 第二组中间位小数点，写入 buf[1] */

/* ============================================
 * FD612_ShowNumberEx 显示选项标志位
 * ============================================ */
#define DISP_NO_LEADING_ZERO    0x01U   /* 不显示前导零（用空格代替） */
#define DISP_G1_DP              0x02U   /* 显示第一组中间位小数点 */
#define DISP_G2_DP              0x04U   /* 显示第二组中间位小数点 */

/* ============================================
 * 函数声明
 * ============================================ */

/**
 * @brief  字符串转buf（类似51的disp_proc）
 * @param  g1_str: 第一组显示字符串，最多3个字符，'.'跟在字符后表示小数点
 * @param  g2_str: 第二组显示字符串，格式同上
 * @note   调用此函数后，task_smg_proc()刷新时生效
 *
 * 示例：
 *   smg_disp_proc("12.3", "456");   // 第一组12.3，第二组456
 *   smg_disp_proc("E  ", " 25");    // 第一组E，第二组25（前导空格）
 *   smg_disp_proc("888", "888");    // 全段测试
 *   smg_disp_proc("---", "---");    // 显示---
 */
void smg_disp_proc(const char *g1_str, const char *g2_str, uint8_t blink1, uint8_t blink2);

/**
 * @brief  FD612刷新任务（类似51的disp_scan，调度器定时调用）
 * @note   建议调用周期：100ms
 *         将 g_disp_buf 内容发送到FD612硬件
 */
void task_smg_scan_proc(void);

/**
 * @brief  关闭显示
 */
void FD612_DisplayOff(void);

/**
 * @brief  开启显示（最大亮度）
 */
void FD612_DisplayOn(void);

/**
 * @brief  清空所有显示
 */
void FD612_Clear(void);

#endif /* __BSP_SMG_H__ */

