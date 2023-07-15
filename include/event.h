#ifndef EVENT_H
#define EVENT_H

#include"LCD.h"

extern int ts_fd;
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

void init_ts(void);//打开触摸屏
int get_xy(int *x, int *y);//获取按下的坐标
void uninit_all(void);//关闭LCD,解除内存映射,关闭触摸屏

#endif