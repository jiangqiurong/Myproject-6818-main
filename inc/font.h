#ifndef __font_h__
#define __font_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/input.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "truetype.h"

// (2)、宏定义(函数、变量、常量)
// 1、字体处理相关宏定义
#define wchar s32
#define wchar_to_str(wc) ((char *)(wc))

#define color u32
#define getColor(a, b, c, d) (a | b << 8 | c << 16 | d << 24)
#define getA(c) ((c >> 0) & 0x000000ff)
#define getR(c) ((c >> 8) & 0x000000ff)
#define getG(c) ((c >> 16) & 0x000000ff)
#define getB(c) ((c >> 24) & 0x000000ff)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

// (3)、数据类型(结构体、联合体、枚举)
// 1、字体处理结构体(自己写的)
typedef struct
{
    stbtt_fontinfo *info;
    u8 *buffer;
    float scale;
} font;

typedef struct
{
    s32 fontSize;
    u32 bgWidth;
    u32 bgHeight;
    u32 bgColor;
    s32 offsetX;
    s32 offsetY;
    char str[256];
    u32 fontColor;
    int posX;
    int posY;
} Font_t;

// 2、画板(点阵图)结构体(自己写的)
typedef struct
{
    u32 height;
    u32 width;
    u32 byteperpixel;
    u8 *map;
} bitmap;

typedef enum
{
    FONT_BLACK = 0x00000000, // 黑色
    FONT_WHITE = 0xffffffff, // 白色
} Font_Color_t;

// 1.初始化字库
font *fontLoad(char *fontPath);

// 2.设置字体的大小
void fontSetSize(font *f, s32 pixels);

// 3.设置字体输出框的大小
bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);

// 可以指定输出框的颜色
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c);

// 4.把字体输出到输出框中
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth);

// 5.把输出框的所有信息显示到LCD屏幕中
//  bitmap.width
//  bitmap.height
//  bitmap.bytepixel
//  bitmap.map

// 关闭字体库
void fontUnload(font *f);

// 初始化字库
font *font_init();

// 关闭bitmap
bitmap *font_set(font *f, Font_t fontp);

void font_show(bitmap *bm, int posX, int posY);

// (4)、全局变量声明

// (5)、函数声明
extern font *fontLoad(char *fontPath);
extern void fontSetSize(font *f, s32 pixels);
extern bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);
extern void setPixel(bitmap *bm, u32 x, u32 y, color c);
extern bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c);
extern void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth);
extern void fontUnload(font *f);
extern void destroyBitmap(bitmap *bm);
extern color getPixel(bitmap *bm, u32 x, u32 y);
extern u32 wstrlen(wchar *ws);
extern wchar *utf8_to_ucs2(char *code);
extern bitmap *createBitmapWithInit_bmp(char *bmppath);

bitmap *char2rgb(char *text, int pixelSize, int outFrameWidth, int outFrameHeight, int outFrameColor, int fontPosX, int fontPosY, int fontColor);

// 关闭画板，释放资源
void destroytruetype(bitmap *bm);

// (6)、全局变量定义、静态函数定义

#endif
