#ifndef __DRAW_H
#define __DRAW_H

#include "lcd.h"
#include "touch.h"
#include "bmp.h"
#include "log.h"
#include "list.h"
#include "login.h"
#include "dir.h"
#include<sys/types.h>
#include<dirent.h>

//非画布区域的范围
#define DRAW_NON_DRAWABLE_AREA ((x >= 0 && x < 100 && y >= 0 && y < 480) ||     \
                                (x >= 700 && x < 800 && y >= 0 && y < 480) ||   \
                                (x >= 100 && x < 700 && y >= 380 && y < 480))

//画布区域的范围
#define DRAW_DRAWABLE_AREA (x >= 100 && x < 700  && y >= 0 && y < 380)

//设置画笔属性
#define draw_set_draw_pen(mode,color,shape,pixel)   do   \
                                                    {   \
                                                        drawPen.drawMode = mode;    \
                                                        drawPen.drawColor = color;  \
                                                        drawPen.drawStretchShape = shape;   \
                                                        drawPen.drawPixel = pixel;  \
                                                    } while (0);

//画圆
#define draw_create_circle(x,y,r)   do  \
                                    {   \
                                        if(x >= 100 && x < 700 && y >= 0 && y < 380)    \
                                        {   \
                                            for(int drawY = y - r; drawY < y + r; drawY++)  \
                                            {   \
                                                for(int drawX = x - r; drawX < x + r; drawX++)  \
                                                {   \
                                                    if((drawX - x) * (drawX - x) + (drawY - y) * (drawY - y) <= r * r)  \
                                                    {   \
                                                        if(drawX < 100 || drawX >= 700 || drawY > 380)  \
                                                        {   \
                                                            continue;   \
                                                        }   \
                                                        lcdBuf[drawY][drawX] = drawPen.drawColor;   \
                                                    }   \
                                                }   \
                                            }   \
                                        }   \
                                    } while (0);

//画矩形
#define draw_create_rectangle(x,y,width,height) do  \
                                                {   \
                                                    if(x >= 100 && x < 700 && y >= 0 && y < 380)    \
                                                    {   \
                                                        for(int drawY = y - height / 2; drawY < y + height / 2; drawY++)  \
                                                        {   \
                                                            for(int drawX = x - width / 2; drawX < x + width / 2; drawX++)  \
                                                            {   \
                                                                if(drawX < 100 || drawX >= 700 || drawY > 380)  \
                                                                {   \
                                                                    continue;   \
                                                                }   \
                                                                lcdBuf[drawY][drawX] = drawPen.drawColor;   \
                                                            }   \
                                                        }   \
                                                    }   \
                                                } while (0);

//画笔颜色
typedef enum 
{
    PEN_COLOR_BLACK = 0x00000000, //黑色
    PEN_COLOR_WHITE = 0x00ffffff, //白色
    PEN_COLOR_RED = 0x00ff0000,   //红色
    PEN_COLOR_GREEN = 0x0000ff00, //绿色
    PEN_COLOR_BLUE = 0x000000ff   //蓝色
} Pen_Color_t;

//画笔大小
typedef enum
{
    PEN_PIXEL_BIG = 34,       //大画笔
    PEN_PIXEL_MIDDLE = 24,    //中画笔
    PEN_PIXEL_SMALL = 12      //小画笔
} Pen_Pixel_t;

//非画布区域的按钮
typedef enum
{
    BTN_EXIT = 10,      //退出
    BTN_SAVE,           //保存
    BTN_BLUE,           //蓝色
    BTN_GREEN,          //绿色
    BTN_RED,            //红色
    BTN_BLACK,          //黑色
    BTN_ERASER,         //橡皮擦
    BTN_BIG_PEN,        //大画笔
    BTN_MIDDLE_PEN,     //中画笔
    BTN_LITTLE_PEN,     //小画笔
    BTN_DRAWABLE_AREA,  //画布区域
    BTN_CIRCLE,         //圆形
    BTN_RECTANGLE,      //矩形
    BTN_TRIANGLE        //三角形
} Btn_NonDrawableArea_t;

//绘画模式
typedef enum
{
    PEN_MODE_BRUSH,    //笔刷模式
    PEN_MODE_STRETCH   //拉伸模式
} Pen_Mode_t;

//拉伸模式的分类
typedef enum
{
    PEN_STRETCH_SHAPE_CIRCLE,       //拉伸模式圆形
    PEN_STRETCH_SHAPE_RECTANGLE,    //拉伸模式矩形
    PEN_STRETCH_SHAPE_TRIANGLE      //拉伸模式三角形
} Pen_Stretch_Shape_t;

//画笔属性
typedef struct 
{
    Pen_Mode_t drawMode;                //绘画模式
    Pen_Color_t drawColor;              //画笔颜色
    Pen_Stretch_Shape_t drawStretchShape;   //拉伸模式形状
    int drawPixel;                      //画笔大小
} Draw_Pen_t;

Draw_Pen_t drawPen; //画笔变量
char drawCanvasName[256];   //画布文件名

//画板主界面模块
void draw_main();

//画图模块
bool draw_layout();

//新建存储用户图片的链表
KernelNode_t* draw_userBmpToList();

//查看模块
bool draw_check();

//画布区域
bool draw_drawableArea();

//保存图片模块
int draw_saveToBmp();

//非画布区域
Btn_NonDrawableArea_t draw_nonDrawableArea();

//非画布区域按键处理模块
Btn_NonDrawableArea_t draw_nonDrawableAreaBtn(int *x,int *y);

#endif