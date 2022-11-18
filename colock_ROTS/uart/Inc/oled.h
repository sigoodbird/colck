#ifndef _OLED_H_
#define _OLED_H_

#include "main.h"

extern const unsigned char *astronaut[];

typedef unsigned char u8;
typedef unsigned int u32;

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/*
*列
*行
*偏移
*页码
*/
#define OLED_COLUMN_NUMBER 128 //列
#define OLED_LINE_NUMBER 64    //行
#define OLED_COLUMN_OFFSET 2   //列_偏移2
#define OLED_PAGE_NUMBER OLED_LINE_NUMBER/8 //页码

#define IIC_SDA_1	    (HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET))//管脚拉高拉低
#define IIC_SDA_0     (HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET))
#define IIC_SDA_READ 	(HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port,OLED_SDA_Pin))//读管脚

#define IIC_SCK_1		(HAL_GPIO_WritePin(OLED_SCK_GPIO_Port,OLED_SCK_Pin,GPIO_PIN_SET))
#define IIC_SCK_0		(HAL_GPIO_WritePin(OLED_SCK_GPIO_Port,OLED_SCK_Pin,GPIO_PIN_RESET))

/*
自己写的
*/
void OLED_Clear(void);//清屏函数
void OLED_Refresh_Gram(void);//刷新
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x,u8 y,const u8 *p);
void OLED_Showchinese(u8 x, u8 y,u8 num,u8 mode);//显示汉字

void OLED_send_data(unsigned char o_data);
void OLED_init(void);
void OLED_full(void);
void Picture_display(const unsigned char *ptr_pic);
void Picture_ReverseDisplay(const unsigned char *ptr_pic);

#endif
