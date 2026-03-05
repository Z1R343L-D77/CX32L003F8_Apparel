/**
 * @FilePath     : bsp_smg.c
 * @Description  : 数码管驱动实现（FD612，双三位数码管）
 * @Author       : Z1R343L 19816013818@163.com
 * @Version      : 2.0.0
 * @LastEditors  : Z1R343L 19816013818@163.com
 * @LastEditTime : 2026-03-04
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2026.
 *
**/
#include "bsp_smg.h"

static uint8_t blink_flag = 1;
static uint16_t blink_cnt = 0;

#define SMG_BLINK_PERIOD 50   //10*50=500

/* ============================================
 * 段码结构体
 * ============================================ */
typedef struct { INT8U a, b, c, d, e, f, g; } SegBits;

/* ============================================
 * 数字段码表 0~9
 * ============================================ */
static const SegBits SEG_NUM[10] = {
    {1,1,1,1,1,1,0}, /* 0 */
    {0,1,1,0,0,0,0}, /* 1 */
    {1,1,0,1,1,0,1}, /* 2 */
    {1,1,1,1,0,0,1}, /* 3 */
    {0,1,1,0,0,1,1}, /* 4 */
    {1,0,1,1,0,1,1}, /* 5 */
    {1,0,1,1,1,1,1}, /* 6 */
    {1,1,1,0,0,0,0}, /* 7 */
    {1,1,1,1,1,1,1}, /* 8 */
    {1,1,1,1,0,1,1}, /* 9 */
};

/* ============================================
 * 特殊字符段码表
 * ============================================ */
typedef struct { char ch; SegBits seg; } SpecChar_t;

static const SpecChar_t SEG_SPEC[] = {
    /* 字母 A~Z */
    {'A', {1,1,1,0,1,1,1}}, /* A */
    {'B', {0,0,1,1,1,1,1}}, /* b */
    {'C', {1,0,0,1,1,1,0}}, /* C */
    {'D', {0,1,1,1,1,0,1}}, /* d */
    {'E', {1,0,0,1,1,1,1}}, /* E */
    {'F', {1,0,0,0,1,1,1}}, /* F */
    {'G', {1,0,1,1,1,1,0}}, /* G */
    {'H', {0,1,1,0,1,1,1}}, /* H */
    {'I', {0,0,0,0,1,1,0}}, /* I (左两竖) */
    {'J', {0,1,1,1,1,0,0}}, /* J */
    {'K', {0,1,0,0,1,1,1}}, /* K 近似 */
    {'L', {0,0,0,1,1,1,0}}, /* L */
    {'M', {1,1,1,0,1,1,0}}, /* M 近似(同A去g) */
    {'N', {0,0,1,0,1,0,1}}, /* N 近似(同n) */
    {'O', {1,1,1,1,1,1,0}}, /* O 同0 */
    {'P', {1,1,0,0,1,1,1}}, /* P */
    {'Q', {1,1,1,0,0,1,1}}, /* Q 近似 */
    {'R', {0,0,0,0,1,0,1}}, /* R 近似(同r) */
    {'S', {1,0,1,1,0,1,1}}, /* S 同5 */
    {'T', {0,0,0,1,1,1,1}}, /* T 近似 */
    {'U', {0,1,1,1,1,1,0}}, /* U */
    {'V', {0,0,1,1,1,0,0}}, /* V 近似 */
    {'W', {0,1,1,1,1,1,0}}, /* W 近似(同U) */
    {'X', {0,1,1,0,1,1,1}}, /* X 近似(同H) */
    {'Y', {0,1,1,1,0,1,1}}, /* Y */
    {'Z', {1,1,0,1,1,0,1}}, /* Z 同2 */
    /* 小写 */
    {'a', {1,1,1,1,1,0,1}}, /* a */
    {'c', {0,0,0,1,1,0,1}}, /* c */
    {'e', {1,1,0,1,1,1,1}}, /* e */
    {'f', {1,0,0,0,1,1,1}}, /* f 同F */
    {'g', {1,1,1,1,0,1,1}}, /* g 同9 */
    {'h', {0,0,1,0,1,1,1}}, /* h */
    {'i', {0,0,0,0,1,0,0}}, /* i */
    {'j', {0,0,1,1,1,0,0}}, /* j */
    {'l', {0,0,0,1,1,1,0}}, /* l 同L */
    {'n', {0,0,1,0,1,0,1}}, /* n */
    {'o', {0,0,1,1,1,0,1}}, /* o */
    {'p', {1,1,0,0,1,1,1}}, /* p 同P */
    {'q', {1,1,1,0,0,1,1}}, /* q */
    {'r', {0,0,0,0,1,0,1}}, /* r */
    {'s', {1,0,1,1,0,1,1}}, /* s 同S */
    {'t', {0,0,0,1,1,1,1}}, /* t 同T */
    {'u', {0,0,1,1,1,0,0}}, /* u */
    {'y', {0,1,1,1,0,1,1}}, /* y 同Y */
    /* 特殊符号 */
    {'-', {0,0,0,0,0,0,1}}, /* - */
    {'_', {0,0,0,1,0,0,0}}, /* _ */
    {'=', {0,0,0,1,0,0,1}}, /* = */
    {'^', {1,0,0,0,0,1,0}}, /* ^ 顶横 */
    {'*', {0,1,1,0,0,1,1}}, /* * 近似度 */
    {'?', {1,1,0,0,0,1,1}}, /* ? */
    /* 空格（全灭，必须放最后作默认值） */
    {' ', {0,0,0,0,0,0,0}},
};

#define SEG_SPEC_COUNT  (sizeof(SEG_SPEC) / sizeof(SEG_SPEC[0]))

/* ============================================
 * 位选值表
 * ============================================ */
static const INT8U G1_POS[3] = {G1_POS0, G1_POS1, G1_POS2};
static const INT8U G1_EFG[3] = {G1_EFG0, G1_EFG1, G1_EFG2};
static const INT8U G2_POS[3] = {G2_POS0, G2_POS1, G2_POS2};
static const INT8U G2_EFG[3] = {G2_EFG0, G2_EFG1, G2_EFG2};

/* ============================================
 * 内部显示缓冲区
 * smg_disp_proc() 写入，task_smg_proc() 读取刷新
 * ============================================ */
static INT8U g_disp_buf[FD612_BUF_SIZE] = {0};

/* ============================================
 * 初始化标志（开显示命令只发一次）
 * ============================================ */
static INT8U g_disp_initialized = 0U;

/* ============================================
 * 内部函数：字符转段码指针
 * ============================================ */
static const SegBits* char_to_seg(char c)
{
    INT8U i;

    /* 数字 0~9 */
    if(c >= '0' && c <= '9')
        return &SEG_NUM[(INT8U)(c - '0')];

    /* 特殊字符查表 */
    for(i = 0U; i < (INT8U)SEG_SPEC_COUNT; i++)
    {
        if(SEG_SPEC[i].ch == c)
            return &SEG_SPEC[i].seg;
    }

    /* 未识别字符 → 空格（全灭），即表中最后一项 */
    return &SEG_SPEC[SEG_SPEC_COUNT - 1U].seg;
}

/* ============================================
 * 内部函数：将一个字符写入 buf 的指定位置
 * group : 0=第一组, 1=第二组
 * pos   : 0=第1位, 1=第2位, 2=第3位
 * c     : 显示字符
 * dp    : 1=点亮该位小数点，0=不点亮
 * ============================================ */
static void Write_Char(INT8U *buf, INT8U group, INT8U pos, char c, INT8U dp)
{
    const SegBits *s;
    INT8U v, ve;

    if(pos >= 3U) return;

    s = char_to_seg(c);

    if(group == 0U)
    {
        v  = G1_POS[pos];
        ve = G1_EFG[pos];

        if(s->a) buf[8]  |= v;
        if(s->b) buf[9]  |= v;
        if(s->c) buf[10] |= v;
        if(s->d) buf[11] |= v;
        if(s->e) buf[3]  |= ve;
        if(s->f) buf[2]  |= ve;
        if(s->g) buf[0]  |= ve;
        if(dp != 0U) buf[1] |= G1_DP_MID;
    }
    else
    {
        v  = G2_POS[pos];
        ve = G2_EFG[pos];

        if(s->a) buf[4]  |= v;
        if(s->b) buf[5]  |= v;
        if(s->c) buf[7]  |= v;   /* c→buf[7] */
        if(s->d) buf[6]  |= v;   /* d→buf[6] */
        if(s->e) buf[3]  |= ve;
        if(s->f) buf[2]  |= ve;
        if(s->g) buf[0]  |= ve;
        if(dp != 0U) buf[1] |= G2_DP_MID;
    }
}

/* ============================================
 * 内部函数：解析字符串，写入 buf 的一组（3位）
 * str : 显示字符串，'.'紧跟在字符后表示小数点
 * group: 0=第一组, 1=第二组
 * buf  : 目标缓冲区
 * ============================================ */
static void Parse_Group(INT8U *buf, INT8U group, const char *str)
{
    INT8U pos = 0U;
    INT8U dp  = 0U;
    const char *p = str;
    /* 第二组位选物理顺序是右→左，需要反转逻辑位 */
    INT8U real_pos;

    if(str == (const char*)0) return;

    /* 先统计有效字符数，第二组需要镜像pos */
    while((*p != '\0') && (pos < 3U))
    {
        if(*p == '.') { p++; continue; }
        dp = ((*(p + 1)) == '.') ? 1U : 0U;

        real_pos = (group == 1U) ? (2U - pos) : pos;

        Write_Char(buf, group, real_pos, *p, dp);

        if(dp != 0U) p++;
        p++;
        pos++;
    }
}

/* ============================================
 * 字符串转buf
 *
 * @param g1_str: 第一组显示字符串（最多3字符，'.'跟在字符后表示小数点）
 * @param g2_str: 第二组显示字符串（格式同上）
 *
 * 示例：
 *   smg_disp_proc("12.3", "456");   // 第一组12.3，第二组456
 *   smg_disp_proc("E  ", " 25");    // 第一组E  ，第二组 25
 *   smg_disp_proc("888", "888");    // 全段测试
 *   smg_disp_proc("---", "---");    // 全部显示-
 * ============================================ */
//void smg_disp_proc(const char *g1_str, const char *g2_str)
//{
//    INT8U buf[FD612_BUF_SIZE] = {0};
//    INT8U i;

//    Parse_Group(buf, 0U, g1_str);
//    Parse_Group(buf, 1U, g2_str);
//		
//    for(i = 0U; i < FD612_BUF_SIZE; i++)
//        g_disp_buf[i] = buf[i];
//}
void smg_disp_proc(const char *g1_str, const char *g2_str, uint8_t blink1, uint8_t blink2)
{
    INT8U buf[FD612_BUF_SIZE] = {0};
    INT8U i;

    /* 第一组 */
    if(!(blink1 && !blink_flag))
        Parse_Group(buf, 0U, g1_str);

    /* 第二组 */
    if(!(blink2 && !blink_flag))
        Parse_Group(buf, 1U, g2_str);

    for(i = 0U; i < FD612_BUF_SIZE; i++)
        g_disp_buf[i] = buf[i];
}
/* ============================================
 * FD612刷新任务
 * 调度器定时调用，建议周期：1ms
 * 将 g_disp_buf 发送到FD612
 * ============================================ */
//void task_smg_scan_proc(void)
//{
//    INT8U i;

//    /* 发送地址自增写命令 */
//    FD612_Start();
//    FD612_WrByte(FD612_ADDR_INC_CMD);
//    FD612_Stop();

//    /* 从地址0x00开始写12字节显示数据 */
//    FD612_Start();
//    FD612_WrByte(FD612_ADDR_BASE);
//    for(i = 0U; i < FD612_BUF_SIZE; i++)
//        FD612_WrByte(g_disp_buf[i]);
//    FD612_Stop();

//    /* 开显示命令只在上电第一次发送 */
//    if(g_disp_initialized == 0U)
//    {
//        FD612_Start();
//        FD612_WrByte(FD612_DISP_ON_MAX);
//        FD612_Stop();
//        g_disp_initialized = 1U;
//    }
//}
void task_smg_scan_proc(void)
{
		INT8U i;
    if(++blink_cnt >= SMG_BLINK_PERIOD)
    {
        blink_cnt = 0;
        blink_flag ^= 1;
    }

    FD612_Start();
    FD612_WrByte(0x40);
    FD612_Stop();

    FD612_Start();
    FD612_WrByte(0xC0);

    for( i=0;i<FD612_BUF_SIZE;i++)
        FD612_WrByte(g_disp_buf[i]);

    FD612_Stop();
}
/* ============================================
 * 关闭显示
 * ============================================ */
void FD612_DisplayOff(void)
{
    FD612_Start();
    FD612_WrByte(FD612_DISP_OFF);
    FD612_Stop();
}

/* ============================================
 * 开启显示（最大亮度）
 * ============================================ */
void FD612_DisplayOn(void)
{
    FD612_Start();
    FD612_WrByte(FD612_DISP_ON_MAX);
    FD612_Stop();
    g_disp_initialized = 1U;
}

/* ============================================
 * 清空所有显示
 * ============================================ */
void FD612_Clear(void)
{
    INT8U i;
    for(i = 0U; i < FD612_BUF_SIZE; i++)
        g_disp_buf[i] = 0U;
}

