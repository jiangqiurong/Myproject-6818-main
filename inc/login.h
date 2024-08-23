#ifndef __LOGIN_H
#define __LOGIN_H

#include "bmp.h"
#include "font.h"
#include "log.h"
#include "touch.h"
#include "list.h"
#include "lcd.h"

// 当前登录的用户
char curUser[10];

// 获取用户信息 存到用户链表
KernelNode_t *login_userToList();

// 登录注册模块
bool login_main();

bool login_register(KernelNode_t *userList);

bool login_signIn(KernelNode_t *userList);

#endif