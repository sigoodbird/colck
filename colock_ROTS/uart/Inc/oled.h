#ifndef _OLED_H_
#define _OLED_H_

#include "main.h"

extern const unsigned char *astronaut[];

typedef unsigned char u8;
typedef unsigned int u32;

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

/*
*��
*��
*ƫ��
*ҳ��
*/
#define OLED_COLUMN_NUMBER 128 //��
#define OLED_LINE_NUMBER 64    //��
#define OLED_COLUMN_OFFSET 2   //��_ƫ��2
#define OLED_PAGE_NUMBER OLED_LINE_NUMBER/8 //ҳ��

#define IIC_SDA_1	    (HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET))//�ܽ���������
#define IIC_SDA_0     (HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET))
#define IIC_SDA_READ 	(HAL_GPIO_ReadPin(OLED_SDA_GPIO_Port,OLED_SDA_Pin))//���ܽ�

#define IIC_SCK_1		(HAL_GPIO_WritePin(OLED_SCK_GPIO_Port,OLED_SCK_Pin,GPIO_PIN_SET))
#define IIC_SCK_0		(HAL_GPIO_WritePin(OLED_SCK_GPIO_Port,OLED_SCK_Pin,GPIO_PIN_RESET))

/*
�Լ�д��
*/
void OLED_Clear(void);//��������
void OLED_Refresh_Gram(void);//ˢ��
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x,u8 y,const u8 *p);
void OLED_Showchinese(u8 x, u8 y,u8 num,u8 mode);//��ʾ����

void OLED_send_data(unsigned char o_data);
void OLED_init(void);
void OLED_full(void);
void Picture_display(const unsigned char *ptr_pic);
void Picture_ReverseDisplay(const unsigned char *ptr_pic);

#endif
