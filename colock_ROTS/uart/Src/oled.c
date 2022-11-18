#include "OLED.h"
#include "oledfont.h"

u8 OLED_GRAM[128][64];	 

unsigned char ACK=0;

/*̫����ͼ*/
const unsigned char *astronaut[]={&gImage_1[0],&gImage_2[0],&gImage_3[0],&gImage_4[0],&gImage_5[0],&gImage_6[0],&gImage_7[0],&gImage_8[0],&gImage_9[0],
&gImage_10[0],&gImage_11[0]};

/*
����оƬ��SH1106
*/
const unsigned char  OLED_init_cmd[25]=			//SH1106
{
  
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//�ر���ʾ
	
       0xD5,//����ʱ�ӷ�Ƶ����,��Ƶ��
       0x80,  //[3:0],��Ƶ����;[7:4],��Ƶ��

       0xA8,//��������·��
       0X3F,//Ĭ��(1/64)
	
       0xD3,//������ʾƫ��
       0X00,//Ĭ��Ϊ0
	
       0x40,//������ʾ��ʼ�� [5:0],����.
	
       0x8D,//��ɱ�����
       0x14,//bit2������/�ر�
       0x20,//�����ڴ��ַģʽ
       0x02,//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
       0xA1,//���ض�������,bit0:0,0->0;1,0->127;  A1
	
       0xC8,//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� (C0 ��ת��ʾ) C8
	   
       0xDA,//����COMӲ����������
       0x12,//[5:4]����  
	   
       0x81,//�Աȶ�����
       0x66,//1~255;Ĭ��0X7F (��������,Խ��Խ��)
	   
       0xD9,//����Ԥ�������
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
	   
       0xDB,//����VCOMH ��ѹ����
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	   
       0xA4,//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	   
       0xA6,//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ 
       
       0xAF,//������ʾ     
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

/**************************IICģ�鷢�ͺ���************************************************

 *************************************************************************/
//д��  ���SDA���ߣ��Եȴ����豸����Ӧ��
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
//	����ҪӦ��
//	if (READ_SDA==0)
//		ACK = 1;
//	else ACK =0;
	delay_us(1);
	IIC_SCK_0;
	delay_us(1);
}

//�����ź�
//SCL�ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��͵�ƽ�ı仯����Ϊ�����ź�
void IIC_start()
{
	IIC_SDA_1;
	delay_us(1);
	IIC_SCK_1;
	delay_us(1);				   //���в��������ͷ�SCL	
	IIC_SDA_0;
	delay_us(3);
	IIC_SCK_0;
	IIC_write(0x78);  
}

//ֹͣ�ź�
//SCL�ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ߵ�ƽ�ı仯����Ϊֹͣ�ź�
void IIC_stop()
{
	IIC_SDA_0;
	delay_us(1);
	IIC_SCK_1;
	delay_us(3);
	IIC_SDA_1;
}

//��������
void OLED_send_cmd(unsigned char o_command)
{
	IIC_start();
	IIC_write(0x00); 
	IIC_write(o_command);
	IIC_stop();
}

//��������
void OLED_send_data(unsigned char o_data)
{ 
	IIC_start();
	IIC_write(0x40);
	IIC_write(o_data);
	IIC_stop();
}

//������
void Column_set(unsigned char column)
{
	column+=OLED_COLUMN_OFFSET;
	OLED_send_cmd(0x10|(column>>4));    //�����е�ַ��λ
	OLED_send_cmd(0x00|(column&0x0f));   //�����е�ַ��λ  
}

//ҳ ����
void Page_set(unsigned char page)
{
	OLED_send_cmd(0xb0+page);
}

//����
void OLED_full(void)
{
	unsigned char page,column;
	for(page=0;page<OLED_PAGE_NUMBER;page++)             //page loop ��
	{ 
		Page_set(page);
		Column_set(0);	  
		for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop ��
		{
			OLED_send_data(0xff);
		}
	}
}

//��ʼ��
void OLED_init(void)
{
	unsigned char i;
	for(i=0;i<25;i++)
	{
		OLED_send_cmd(OLED_init_cmd[i]);
	}
}

//ͼ����ʾ
void Picture_display(const unsigned char *ptr_pic)
{
	unsigned char page,column;
	for(page=0;page<(OLED_LINE_NUMBER/8);page++)        //page loop
	{
		Page_set(page);//ҳ ����
		Column_set(0); //������
		for(column=0;column<OLED_COLUMN_NUMBER;column++)	//column loop
		{
			OLED_send_data(*ptr_pic++);
		}
	}
}

//ͼƬ������ʾ
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
			OLED_send_data(data);//��������
		}
	}
}

//����
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
	for(t=0;t<size;t++)
	{
	if(size==12)temp=oled_asc2_1206[chr][t];//����1206����
	else temp=oled_asc2_1608[chr][t];//����1608����
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

//m^n����
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
	OLED_Refresh_Gram();//������ʾ
}

void OLED_Refresh_Gram(void)
{
	u8 i=0,n=0;		    
	for(i=0;i<8;i++)  
	{
		Page_set(i);    //����ҳ��ַ��0~8��
		Column_set(0);   
		for(n=0;n<128;n++)OLED_send_data(OLED_GRAM[n][i]); 
	}   
}

//��ʾ����
//��ʾ����
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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

//��ʾ�ַ���
//x,y:�������
//*p:�ַ�����ʼ��ַ
//��12����
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
/* ��ʾ���� x,y:λ�� num :���ı�� mode:��ʾ��ģʽ*/ 
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
