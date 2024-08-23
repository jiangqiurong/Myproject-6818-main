#ifndef __TOUCH_H
#define __TOUCH_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include "log.h"
#include "font.h"
#include "bmp.h"

#define touch_get_coordinate(x, y)                        \
    do                                                    \
    {                                                     \
        if (touchInfo.type == EV_ABS)                     \
        {                                                 \
            if (touchInfo.code == ABS_X)                  \
            {                                             \
                x = (touchInfo.value * 800 * 1.0) / 1024; \
            }                                             \
            else if (touchInfo.code == ABS_Y)             \
            {                                             \
                y = (touchInfo.value * 480 * 1.0) / 600;  \
            }                                             \
        }                                                 \
    } while (0);

// 键盘输入类型
typedef enum
{
    KEYBOARD_ACCCOUNT_IN, // 输入账号
    KEYBOARD_PASSWORD_IN, // 输入密码
    KEYBOARD_FILENAME_IN  // 输入文件名
} Keyboard_Flag_t;

// 键盘输入内容是否可视
#define INPUT_VISIBLE(flag) (flag == KEYBOARD_ACCCOUNT_IN || flag == KEYBOARD_FILENAME_IN)

int touchFd;
struct input_event touchInfo;

// 触摸屏初始化
void touch_init();

// 触摸屏销毁
void touch_destroy();

// 返回点击坐标
void touch_click(int *posX, int *posY);

// 获取键盘数字
int touch_keyboardGetNum(char *str, Keyboard_Flag_t flag);

#endif