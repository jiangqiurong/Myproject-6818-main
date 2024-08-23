#ifndef __2048_H__
#define __2048_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "lcd.h"
#include "ts.h"
// 棋盘左上角坐标
#define startx0 50
#define statry0 200
// 棋盘棋子宽度
#define Size 80
// 棋子间隙
#define space 5
// 棋子数量
#define number 4

// 游戏主循环
bool game_2048();

// 游戏主界面
bool game();

#endif
