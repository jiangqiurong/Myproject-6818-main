#ifndef __DIR_H
#define __DIR_H

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include "list.h"
#include "log.h"

//查找目标后缀名文件并生成链表
KernelNode_t* dir_fileToList(const char *dir,const char *suffix);

#endif