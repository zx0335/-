#include "event.h"

int ts_fd;
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
// 打开触摸屏
void init_ts(void)
{
    // 模拟器：/dev/ubuntu_event
    // 开发板：/dev/input/event0
    ts_fd = open("/dev/input/event0", O_RDWR);

    if (ts_fd < 0)
    {
        perror("open ts failed");
        return;
    }
}

int get_xy(int *x, int *y)
{
    struct input_event buf;
    int x1, y1;       // 保存第一次的坐标
    int x2, y2;       // 保存最后一次的坐标
    int tmp_x, tmp_y; // 保存临时坐标
    x1 = y1 = x2 = y2 = -1;
    while (1)
    {
        read(ts_fd, &buf, sizeof(buf)); // 读取触摸屏数据
        if (buf.type == EV_ABS)         // 触摸屏事件
        {
            if (buf.code == ABS_X) // X轴
            {
                tmp_x = buf.value;
            }
            else if (buf.code == ABS_Y) // Y轴
            {
                tmp_y = buf.value;
            }
        }
        // 判断手指松开
        if (buf.type == EV_KEY && buf.code == BTN_TOUCH)
        {
            if (buf.value == 1) // 按下
            {
                x1 = tmp_x;
                y1 = tmp_y;
            }
            else if (buf.value == 0) // 松开,保存最后一次坐标
            {
                x2 = tmp_x;
                y2 = tmp_y;
                break;
            }
        }
    }
    int var_x = abs(x2 - x1); // x轴变化量
    int var_y = abs(y2 - y1); // y轴变化量
    // 判断是滑动还是点击
    if ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) >= 60 * 60) // 滑动
    {
        if (y1 > y2 && var_y > var_x) // 上划
        {
            return UP;
        }
        else if (y1 < y2 && var_y > var_x) // 下划
        {
            return DOWN;
        }
        else if (x1 > x2 && var_x > var_y) // 左
        {
            return LEFT;
        }
        else if (x1 < x2 && var_x > var_y) // 右
        {
            return RIGHT;
        }
    }
    else // 点击
    {
        *x = x2 * 800 / 1024; //*800/1024
        *y = y2 * 480 / 600;  //*480/600
        return 0;
    }
}

// 关闭所有文件
void uninit_all(void)
{
    // 解除映射
    munmap(p, 800 * 480 * 4);
    // 关闭lcd
    close(lcd_fd);
    // 关闭触摸屏
    close(ts_fd);
}