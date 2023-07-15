#include"lcd.h"

int *p;
int lcd_fd;
//初始化lcd
void init_lcd(void)
{
     //打开显示屏
    int lcd_fd = open("/dev/fb0",O_RDWR);
    
    if(lcd_fd < 0)      //打开失败
    {
        perror("open lcd failed");
        return;
    }

    //进行内存映射
    p = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd_fd,0);
    if(p == NULL)
    {
        perror("mmap() failed");
        return;
    }
}

//显示bmp图片
void show_bmp(int offset_x,int offset_y,char *bmp_name)
{
    //打开bmp图片
    int bmp_fd = open(bmp_name,O_RDWR);
    if(bmp_fd < 0)
    {
        perror("open bmp failed");
        return;
    }

    //读取头信息,读完光标偏移56字节
    bmp_head info;
    read(bmp_fd,&info,sizeof(info));
    
    //读取图片数据
    unsigned char bmp_buf[info.width*info.height*3];
    read(bmp_fd,bmp_buf,sizeof(bmp_buf));
    
    int x,y;
    unsigned char r,g,b;
    int color;
    int i = 0;
    for(y=0;y<info.height;y++)
    {
        for(x=0;x<info.width;x++)
        {
            b = bmp_buf[i++];
            g = bmp_buf[i++];
            r = bmp_buf[i++];
            color = b | g << 8 | r << 16;          //每个像素点的颜色
            *(p+800*(info.height-1-y+offset_y)+(x+offset_x)) = color;
        }
    }
}