#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "kernel_list.h"

// 节点数据结构体
typedef struct
{
    char fileName[256];
    char user[10];
    char password[10];
} ElemType_t;

// 节点
typedef struct
{
    ElemType_t data;       // 数据域
    struct list_head list; // 指针域 小结构体
} KernelNode_t;

// 初始化链表
KernelNode_t *list_init();
// 从链表尾部插入数据
void list_insert_tail(KernelNode_t *head, ElemType_t data);
// 从链表头部插入数据
void list_insert_head(KernelNode_t *head, ElemType_t data);
// 删除链表节点  返回值：true--删除成功  false--删除失败
bool list_delete(KernelNode_t *head, ElemType_t data);
// 修改链表节点数据  返回值：true--修改成功  false--修改失败
bool list_update(KernelNode_t *head, ElemType_t srcData, ElemType_t newData);
// 查找数据  返回值：true--查找成功  false--查找失败
bool list_find(KernelNode_t *head, ElemType_t data);
// 顺序打印链表
void list_show(KernelNode_t *head);
// 逆序打印链表
void list_show_reverse(KernelNode_t *head);
// 销毁链表
void list_destroy(KernelNode_t *head);

#endif