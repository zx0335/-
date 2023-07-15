#include "event.h"
#include <pthread.h>
#include <stdbool.h>

#define MAXSIZE 4 // 存储图片的数量
int count = 0;
bool IsMove = false;

void *f(void *arg) // 实现每隔1s切换图片
{
    char(*p)[20] = (char(*)[20])arg;
    while (1)
    {
        if (IsMove) // 打断自动播放
        {
            sleep(4);
            IsMove = false;
        }
        show_bmp(0, 0, p[count]);
        sleep(1);
        if (count == 3)
            count = 0;
        else
            count++;
    }
}

int main(int argc ,char **argv)
{
    init_lcd();
    init_ts();
    // 取触摸屏数据
    struct input_event buf;
    int x, y;
    int ret;
    char str[MAXSIZE][20];
    for (int i = 0; i < MAXSIZE; i++)
    {
        sprintf(str[i], "./pic/%d.bmp", i + 1);
        printf("%s\n", str[i]);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, f, str);

    int a = 0;
    while (1)
    {
        ret = get_xy(&x, &y);
        IsMove = true;
        if (ret == 0)
        {
            printf("(%d,%d)\n", x, y);
            // 根据坐标实现对应的功能
            if (x > 0 && x < 400 && y > 0 && y < 240)
                printf("左上角\n");
            else if (x > 0 && x < 400 && y > 240 && y < 480)
                printf("左下角\n");
            else if (x > 400 && x < 800 && y > 0 && y < 240)
                printf("右上角\n");
            else if (x > 400 && x < 800 && y > 240 && y < 480)
                printf("右下角\n");
        }
        else if (ret == UP)
        {
            printf("上划\n");
        }
        else if (ret == DOWN)
        {
            printf("下划\n");
            a++;
            if (a == 2)
                break;
        }
        else if (ret == LEFT)
        {
            printf("左划\n");
            if (count > 0)
            {
                count--;
                show_bmp(0, 0, str[count]);
            }
            else if (count == 0)
            {
                count = MAXSIZE - 1;
                show_bmp(0, 0, str[count]);
            }
        }
        else if (ret == RIGHT)
        {
            printf("右划\n");
            if (count < (MAXSIZE - 1))
            {
                count++;
                show_bmp(0, 0, str[count]);
            }
            else if (count == (MAXSIZE - 1))
            {
                count = 0;
                show_bmp(0, 0, str[count]);
            }
        }
    }
    uninit_all();
}