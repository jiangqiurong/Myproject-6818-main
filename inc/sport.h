#ifndef __SPORT_H
#define __SPORT_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdbool.h>

bool sport();

#endif

// extern int init_touch_dev();
// extern void get_slide_event(int fd);
