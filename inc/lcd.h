#ifndef __LCD_H // 定义以防止递归包含
#define __LCD_H

// (1)、头文件
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "log.h"
#include <errno.h>
#include <string.h>
#include <linux/input.h>
#include <termios.h>
#include "font.h"

// (2)、宏定义(常量、变量、函数)
#define LCD_DEVPATH "/dev/fb0"

int lcdFd;
int (*lcdBuf)[800];     // 用于映射lcd到内存中
int (*lcdTempBuf)[800]; // 用于保存记录lcd关键点显示的内容
int (*lcdOverBuf)[800]; // 用于关闭程序时恢复打开程序前lcd显示的内容
int lcdWidth;
int lcdHeight;
struct fb_var_screeninfo lcdVarInfo;

// (3)、自定义类型(结构体、联合体、枚举类型)
// 1、BMP格式头规范
struct bitmap_header
{
    int16_t type;
    int32_t size; // 图像文件大小
    int16_t reserved1;
    int16_t reserved2;
    int32_t offbits; // bmp图像数据偏移量
} __attribute__((packed));

struct bitmap_info
{
    int32_t size;   // 本结构大小
    int32_t width;  // 图像宽
    int32_t height; // 图像高
    int16_t planes;

    int16_t bit_count; // 色深
    int32_t compression;
    int32_t size_img; // bmp数据大小，必须是4的整数倍
    int32_t X_pel;
    int32_t Y_pel;
    int32_t clrused;
    int32_t clrImportant;
} __attribute__((packed));

// 2、显示屏信息结构体
typedef struct
{
    const char *devpath; // 显示屏的位置

    int fd;              // 显示屏的文件描述符
    unsigned int *p;     // 显示屏的映射函数返回的虚拟内存指针
    unsigned int *tmp_p; // 显示屏的映射函数返回的虚拟内存指针的备份

} lcd_msg;

// 设计一个lcd结构体
typedef struct LcdDevice
{
    int fd;
    unsigned int (*lcd_p)[800];
    int lw, lh;
    int pertype;
    struct fb_var_screeninfo varinfo;
    int area;         // 0表示A区 1表示B区
    unsigned int *mp; // 保存映射首地址
} LcdDevice;

// 将lcd的内容显示为prev的内容
// 将 prev 数组对应位置的元素值赋给 to 数组的对应位置
#define lcd_from_prev_to(prev, to)             \
    do                                         \
    {                                          \
        for (int y = 0; y < lcdHeight; y++)    \
        {                                      \
            for (int x = 0; x < lcdWidth; x++) \
            {                                  \
                to[y][x] = prev[y][x];         \
            }                                  \
        }                                      \
    } while (0);

// (4)、全局变量声明

// (5)、函数声明
extern lcd_msg *Lcd_Init(void);
extern void Lcd_ShowColor(lcd_msg *lcd, int color, int lcd_x, int lcd_y, int color_w, int color_h, int mode);
extern int Lcd_ShowBmp(lcd_msg *lcd, const char *bmppath, int lcd_x, int lcd_y, int mode);
extern int Lcd_ShowBmp_move(lcd_msg *lcd, const char *bmppath, int mode);
extern void Lcd_Release(lcd_msg *lcd);
extern int Lcd_ShowFontBmpToLcd(lcd_msg *lcd, bitmap *bm, int lcd_x, int lcd_y, int mode);
extern void Lcd_ShowFont(lcd_msg *lcd, int f_s, int bm_w, int bm_h, color bm_c, int f_x, int f_y, char *font_text, color f_c, int f_lw, int lcd_x, int lcd_y, int lcd_effect);
extern void Lcd_ShowFont_bmp(lcd_msg *lcd, int f_s, char *bmp_path, int f_x, int f_y, char *font_text, color f_c, int f_lw, int lcd_x, int lcd_y, int lcd_effect);

// 初始化液晶屏
void lcd_init();

// 液晶屏销毁
void lcd_destroy();

// 打开屏幕
void lcd_open(void);
// 关闭屏幕
void lcd_close(void);
// 描点
void lcd_draw_point(int x, int y, int color);
// 画矩形
void lcd_draw_rectanle(int x0, int y0, int w, int h, int color);
// 清屏
void lcd_clear_screen(int color);

void lcd_draw_word(char *ch, int len, int w, int color, int x0, int y0);
void lcd_draw_rect(uint x0, uint y0, uint w, uint h, int color);
void lcd_draw_digit(int value, int x0, int y0, int color);

void draw_lcd_font(LcdDevice *lcd, int x, int y, bitmap *font);

LcdDevice *lcd_init1();

#endif