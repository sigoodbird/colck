#include "OLED.h"
#include "oledfont.h"

u8 OLED_GRAM[128][64];	 

unsigned char ACK=0;

/*太空人图*/
const unsigned char *astronaut[]={&gImage_1[0],&gImage_2[0],&gImage_3[0],&gImage_4[0],&gImage_5[0],&gImage_6[0],&gImage_7[0],&gImage_8[0],&gImage_9[0],
&gImage_10[0],&gImage_11[0]};

/*
驱动芯片：SH1106
*/
const unsigned char  OLED_init_cmd[25]=			//SH1106
{
  
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//关闭显示
	
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率

       0xA8,//设置驱动路数
       0X3F,//默认(1/64)
	
       0xD3,//设置显示偏移
       0X00,//默认为0
	
       0x40,//设置显示开始行 [5:0],行数.
	
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;  A1
	
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 (C0 翻转显示) C8
	   
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置  
	   
       0x81,//对比度设置
       0x66,//1~255;默认0X7F (亮度设置,越大越亮)
	   
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
	   
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	   
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	   
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示 
       
       0xAF,//开启显示     
};

//void delay_us(unsigned int _us_time)
//{       
//  unsigned char x=0;
//  for(;_us_time>0;_us_time--)
//  {
//    x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
//	  x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;x++;
//	  
//  }
//}

//void delay_ms(unsigned int _ms_time)
//{
//	unsigned int i,j;
//	for(i=0;i<_ms_time;i++)
//	{
//	for(j=0;j<900;j++)
//		{;}
//	}
//}

/**************************IIC模块发送函数************************************************

 *************************************************************************/
//写入  最后将SDA拉高，以等待从设备产生应答
void IIC_write(unsigned char date)
{
	unsigned char i, temp;
	temp = date;
			
	for(i=0; i<8; i++)
	{	IIC_SCK_0;
		
		if ((temp&0x80)==0)
				IIC_SDA_0;
		else IIC_SDA_1;
		temp = temp << 1;
		delay_us(1);			  
		IIC_SCK_1;
		delay_us(1);
		
	}
	IIC_SCK_0;
	delay_us(1);
	IIC_SDA_1;
	delay_us(1);
	IIC_SCK_1;
//	不需要应答
//	if (READ_SDA==0)
//		ACK = 1;
//	else ACK =0;
	delay_us(1);
	IIC_SCK_0;
	delay_us(1);
}

//启动信号
//SCL在高电平期间，SDA由高电平向低电平的变化定义为启动信号
void IIC_start()
{
	IIC_SDA_1;
	delay_us(1);
	IIC_SCK_1;
	delay_us(1);				   //所有操作结束释放SCL	
	IIC_SDA_0;
	delay_us(3);
	IIC_SCK_0;
	IIC_write(0x78);  
}

//停止信号
//SCL在高电平期间，SDA由低电平向高电平的变化定义为停止信号
void IIC_stop()
{
	IIC_SDA_0;
	delay_us(1);
	IIC_SCK_1;
	delay_us(3);
	IIC_SDA_1;
}

//发送命令
void OLED_send_cmd(unsigned char o_command)
{
	IIC_start();
	IIC_write(0x00); 
	IIC_write(o_command);
	IIC_stop();
}

//发送数据
void OLED_send_data(unsigned char o_data)
{ 
	IIC_start();
	IIC_write(0x40);
	IIC_write(o_data);
	IIC_stop();
}

//列设置
void Column_set(unsigned char column)
{
	column+=OLED_COLUMN_OFFSET;
	OLED_send_cmd(0x10|(column>>4));    //设置列地址高位
	OLED_send_cmd(0x00|(column&0x0f));   //设置列地址低位  
}

//页 设置
void Page_set(unsigned char page)
{
	OLED_send_cmd(0xb0+page);
}

//满的
void OLED_full(void)
{
	unsigned char page,column;
	for(page=0;page<OLED_PAGE_NUMBER;page++)             //page loop 行
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop 列
		{
			OLED_send_data(0xff);
		}
	}
}

//初始化
void OLED_init(void)
{
	unsigned char i;
	for(i=0;i<25;i++)
	{
		OLED_send_cmd(OLED_init_cmd[i]);
	}
}

//图像显示
void Picture_display(const unsigned char *ptr_pic)
{
	unsigned char page,column;
	for(page=0;page<(OLED_LINE_NUMBER/8);page++)        //page loop
	{
		Page_set(page);//页 设置
		Column_set(0); //列设置
		for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
		{
			OLED_send_data(*ptr_pic++);
		}
	}
}

//图片反向显示
void Picture_ReverseDisplay(const unsigned char *ptr_pic)
{
	unsigned char page,column,data;
	for(page=0;page<(OLED_LINE_NUMBER/8);page++)        //page loop
	{
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
		{
			data=*ptr_pic++;
			data=~data;
			OLED_send_data(data);//发送数据
		}
	}
}

//画点
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//得到偏移后的值				   
	for(t=0;t<size;t++)
	{
	if(size==12)temp=oled_asc2_1206[chr][t];//调用1206字体
	else temp=oled_asc2_1608[chr][t];//调用1608字体
	for(t1=0;t1<8;t1++)
	{
		if(temp&0x80)OLED_DrawPoint(x,y,mode);
		else OLED_DrawPoint(x,y,!mode);
		temp<<=1;
		y++;
		if((y-y0)==size)
		{
			y=y0;
			x++;
			break;
		}
	}  	 
	}          
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void OLED_Clear(void)  
{
	u8 i,n;
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
	OLED_Refresh_Gram();//更新显示
}

void OLED_Refresh_Gram(void)
{
	u8 i=0,n=0;		    
	for(i=0;i<8;i++)  
	{
		Page_set(i);    //设置页地址（0~8）
		Column_set(0);   
		for(n=0;n<128;n++)OLED_send_data(OLED_GRAM[n][i]); 
	}   
}

//显示数字
//显示数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size)
{
	u8 t=0,temp=0;
	u8 enshow=0;				   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
}

//显示字符串
//x,y:起点坐标
//*p:字符串起始地址
//用12字体
void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
		#define MAX_CHAR_POSX 122
		#define MAX_CHAR_POSY 58          
		while(*p!='\0')
		{
				if(x>MAX_CHAR_POSX){x=0;y+=16;}
				if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
				OLED_ShowChar(x,y,*p,16,1);
				x+=8;
				p++;
		}
}

/*************************************************************************/
/* 显示中文 x,y:位置 num :中文编号 mode:显示的模式*/ 
/*************************************************************************/
void OLED_Showchinese(u8 x, u8 y,u8 num,u8 mode)
{
		u8 temp,t,t1;
		u8 y0=y;
		u8 size = 16;
		u8 csize=(size/8 + ((size%8)?1:0)) * size;    
		
		for(t=0;t<csize;t++)
		{
				temp = chinese2x16[num][t];  
				for(t1=0;t1<8;t1++)
				{
						if(temp&0x80)OLED_DrawPoint(x,y,mode);
						else OLED_DrawPoint(x,y,!mode);
						temp<<=1;
						y++;
						if((y-y0)==size)
						{
								y=y0;
								x++;
								break;
						}
				}    
		} 	
}
