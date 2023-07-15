#ifndef lcd_h
#define lcd_h

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h> //输入事件头文件

extern int *p;
extern int lcd_fd;

/*bmp图片的文件头信息定义*/
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
#pragma pack(1) // 取消字节对齐
typedef struct
{ // bmp图片文件头信息封装
	// 位图文件头
	u8 bit_file_type[2]; // 位图文件类型:'BM'->0x4d42
	u32 file_size;		 // 整个文件大小
	u16 reserved1;		 // 保留
	u16 reserved2;		 // 保留
	u32 offset;			 // 文件头到位图数据之间的偏移量

	// 位图信息头
	u32 head_size;		// 位图信息头长度
	u32 width;			// 位图宽度
	u32 height;			// 位图高度
	u16 bit_planes;		// 位图位面数
	u16 bits_per_pixel; // 每个像素的位数
	u32 compression;	// 压缩说明
	u32 image_size;		// 位图数据大小
	u32 h_res;			// 水平分辨率
	u32 v_res;			// 垂直分辨率
	u32 color_palette;	// 位图使用的颜色索引数
	u32 vip_color;		// 重要的颜色索引数目
} bmp_head;
#pragma pack() // 恢复字节对齐

void init_lcd(void);									   // 初始化lcd,打开内存映射
void show_bmp(int offset_x, int offset_y, char *bmp_name); // 显示bmp图片
#endif