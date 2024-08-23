#ifndef __AD_H
#define __AD_H

/*存放头文件*/
#include <stdio.h>  //printf函数、scanf函数
#include <error.h>  //标准出错头文件
#include <string.h> //memset函数、strlen函数的头文件
#include <stdlib.h>
#include <unistd.h> //read、write函数,sleep的
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>    //mmap函数
#include <linux/input.h> //Linux下的输入子系
#include <pthread.h>     //线程API接口头文件
#include <unistd.h>

bool advertising(); // 广告

#endif