#include "ds3231.h"
#include "usart.h"
#include "delay.h"

DateTime RTCDateTime;

/*�ܽŷ�������*/
void DS3231_SDA_GPIO_OUT(void)//SDA:PB5  //IICͨ�ŹܽŹܽŶ���
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SDA_GPIO, &GPIO_InitStruct);
}

void DS3231_SDA_GPIO_IN(void)//SDA:PB12
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SDA_GPIO, &GPIO_InitStruct);
}

void DS3231_SCL_GPIO_OUT(void)//SCL:PB13
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCL_GPIO, &GPIO_InitStruct);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
int IIC_Start(void)
{
	DS3231_SDA_GPIO_OUT();     //sda�����
	DS3231_SDA_H;
	if(!DS3231_SDA_READ)return 0;	
	DS3231_SCL_H;
	delay_us(1);
 	DS3231_SDA_L;//START:when CLK is high,DATA change form high to low 
	if(DS3231_SDA_READ)return 0;
	delay_us(1);
	DS3231_SCL_L;//ǯסI2C���ߣ�׼�����ͻ�������� 
	return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
void IIC_Stop(void)
{
	DS3231_SDA_GPIO_OUT();//sda�����
	DS3231_SCL_L;
	DS3231_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	DS3231_SCL_H; 
	DS3231_SDA_H;//����I2C���߽����ź�
	delay_us(1);							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
int IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	DS3231_SDA_GPIO_IN();      //SDA����Ϊ����  
	DS3231_SDA_H;
	delay_us(1);	   
	DS3231_SCL_H;
	delay_us(1);	 
	while(DS3231_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;//ʧ��
		}
	  delay_us(1);
	}
	DS3231_SCL_L;//ʱ�����0 	   
	return 0;  
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
void IIC_Ack(void)
{
	DS3231_SCL_L;
	DS3231_SDA_GPIO_OUT();
	DS3231_SDA_L;
	delay_us(1);
	DS3231_SCL_H;
	delay_us(1);
	DS3231_SCL_L;
}
	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
void IIC_NAck(void)
{
	DS3231_SCL_L;
	DS3231_SDA_GPIO_OUT();
	DS3231_SDA_H;
	delay_us(1);
	DS3231_SCL_H;
	delay_us(1);
	DS3231_SCL_L;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Send_Byte(uint8_t txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/		  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		DS3231_SDA_GPIO_OUT(); 	    
    DS3231_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
			if(1==(txd&0x80)>>7){
				DS3231_SDA_H;
			}else {
				DS3231_SDA_L;
			}
		txd<<=1; 	  
		delay_us(1);   
		DS3231_SCL_H;
		delay_us(1); 
		DS3231_SCL_L;	
		delay_us(1);
    }	 
} 	 
  
/**************************ʵ�ֺ���********************************************
*����ԭ��:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
*��������:		д����
*******************************************************************************/
int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
		int i;
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1 );
    if (!IIC_Wait_Ack()){
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
		for (i = 0; i < len; i++) {
        IIC_Send_Byte(data[i]);
        if (!IIC_Wait_Ack()) {
            IIC_Stop();
            return 0;
        }
    }
    IIC_Stop();
    return 0;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		bool i2cRdad(uint8_t addr, uint8_t reg, uint8_t data)
*��������:		������
*******************************************************************************/
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!IIC_Start())
        return 1;
    IIC_Send_Byte(addr << 1);
    if (!IIC_Wait_Ack()){
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((addr << 1)+1);
    IIC_Wait_Ack();
    while(len){
		if (len == 1)
				*buf = IIC_Read_Byte(0);
		else
				*buf = IIC_Read_Byte(1);
		buf++;
		len--;
    }
    IIC_Stop();
    return 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	DS3231_SDA_GPIO_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
	{
		DS3231_SCL_L; 
		delay_us(2);
		DS3231_SCL_H;
		receive<<=1;
		if(DS3231_SDA_READ)receive++;   //�������߸ߵ�λ
		delay_us(2);
    }
    if (ack)
        IIC_Ack(); //����ACK 
    else
        IIC_NAck();//����nACK  
    return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();
	IIC_Send_Byte(I2C_Addr);//����д����
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;  //���͵�ַ
	IIC_Wait_Ack();
	//IIC_Stop();//����һ��ֹͣ����
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;      //�������ģʽ			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);
  IIC_Stop();//����һ��ֹͣ����

	return res;
}

uint8_t IIC_DS3231_ByteWrite(uint8_t WriteAddr , uint8_t date)//д����
{
	IIC_Start();//��ʼ����
	IIC_Send_Byte(DS3231_ADDRESS_Write);//���ʹӻ���ַ
	if(IIC_Wait_Ack()){
		//printf("���ʹӻ���ַʧ��\r\n");
		return 1;
	}
	IIC_Send_Byte(WriteAddr);//д��ַ,д��ַʧ��
	if(IIC_Wait_Ack()){
		//printf("д��ַʧ��\r\n");
		return 2;
	}
	IIC_Send_Byte(date);//д����
	if(IIC_Wait_Ack()){
		//printf("д����ʧ�ܦ\r\n");
		return 3;
	}
		
	IIC_Stop();
	return 0;
}

uint8_t IIC_DS3231_ByteRead(uint8_t ReadAddr,uint8_t* Receive)//������
{
	uint8_t data = 0;
	
	IIC_Start();													//������ʼλ
	IIC_Send_Byte(DS3231_ADDRESS_Write); 	//���ʹӻ���ַ��дģʽ��
	if(IIC_Wait_Ack())										//�ȴ���Ӧ
		return 1;
	IIC_Send_Byte(ReadAddr);							//���ͼĴ�����ַ
	if(IIC_Wait_Ack())										//�ȴ���Ӧ
		return 2;
	IIC_Start();													//�ظ���ʼλ
	IIC_Send_Byte(DS3231_ADDRESS_Read);		//���ʹӻ���ַ����ģʽ��
	if(IIC_Wait_Ack())										//�ȴ���Ӧ
		return 3;
	data = IIC_Read_Byte(0);							//��ȡ���ݣ�������Ϊ0 --- NACK
	*Receive = data;											//�������ֵ������λ
	IIC_Stop();
	return 0;
}

uint8_t DS3231_setDate(uint8_t year,uint8_t mon,uint8_t day)//����������
{
	uint8_t temp_H , temp_L;
	temp_L = year%10;
	temp_H = year/10;
	year = (temp_H << 4) + temp_L;
	if(IIC_DS3231_ByteWrite(DS3231_YEAR_REG,year)) //set year
	{
			//printf("set year error\r\n");
			return 1;
	}	
	temp_L = mon%10;
	temp_H = mon/10;
	mon = (temp_H << 4) + temp_L;	
	if(IIC_DS3231_ByteWrite(DS3231_MONTH_REG,mon)) //set mon
	{
		//printf("set month error\r\n");
		return 2;
	}
	temp_L = day%10;
	temp_H = day/10;
	day = (temp_H << 4) + temp_L;		
	if(IIC_DS3231_ByteWrite(DS3231_MDAY_REG,day)) //set day
	{
		//printf("set day error\r\n");
		return 3;
	}
	return 0;
}


uint8_t DS3231_setTime(uint8_t hour , uint8_t min , uint8_t sec)
{
	uint8_t temp_H , temp_L;
	temp_L = hour%10;
	temp_H = hour/10;
	hour = (temp_H << 4) + temp_L;
	if(IIC_DS3231_ByteWrite(DS3231_HOUR_REG,hour)) //set hour
		return 1;
	temp_L = min%10;
	temp_H = min/10;
	min = (temp_H << 4) + temp_L;
	if(IIC_DS3231_ByteWrite(DS3231_MIN_REG,min)) //SET min
		return 2;
	temp_L = sec%10;
	temp_H = sec/10;
	sec = (temp_H << 4) + temp_L;	
	if(IIC_DS3231_ByteWrite(DS3231_SEC_REG,sec))		//SET sec
		return 3;
	return 0;
}

static uint8_t bcdToDec(uint8_t byte)//2ת10
{
	uint8_t temp_H , temp_L;
	temp_L = byte & 0x0f;
	temp_H = (byte & 0xf0) >> 4;
	return ( temp_H * 10 )+ temp_L;
}

uint8_t DS3231_gettime(DateTime* ans)//��ʱ��
{
	uint8_t receive = 0;
	if(IIC_DS3231_ByteRead(DS3231_HOUR_REG,&receive)){
		//printf("Read hour error\r\n");
		return 1;
	}
	ans->hour = bcdToDec(receive);
	if(IIC_DS3231_ByteRead(DS3231_MIN_REG,&receive)){
		//printf("Read min error\r\n");
		return 2;
	}
	ans->minute = bcdToDec(receive);
	if(IIC_DS3231_ByteRead(DS3231_SEC_REG,&receive)){
		//printf("Read sec error\r\n");
		return 3;
	}
	ans->second = bcdToDec(receive);
	return 0;
}

uint8_t DS3231_getdate(DateTime* ans)//������
{
	uint8_t receive = 0;
	if(IIC_DS3231_ByteRead(DS3231_YEAR_REG,&receive))
		return 1;
	ans->year = bcdToDec(receive) + 2000;
	if(IIC_DS3231_ByteRead(DS3231_MONTH_REG,&receive))
		return 2;
	ans->month = bcdToDec(receive);
	if(IIC_DS3231_ByteRead(DS3231_MDAY_REG,&receive))
		return 3;
	ans->dayofmonth = bcdToDec(receive);
	return 0;
}

//�ж��ж���λ��
int  fun(int n)
{
 int count=1;//????????
 while(n/10) //?????0
 {
  count++; //??0,???1
  n=n/10; //??n??10,????
 }
 return count;
}

//��RCTʱ��ṹ�����д����
unsigned char RCTdatatoWBlockData(unsigned char *datain)
{	
	int i;
	datain[2]=(RTCDateTime.year%100)/10;
	datain[3]=(RTCDateTime.year%10);

	for(i=3;i<15;i++)
	{
		switch(i)
		{
			case 4:if(fun(RTCDateTime.month)<2){datain[i]=0;}else{datain[i]=RTCDateTime.month/10;};break;//��
			case 5:if(fun(RTCDateTime.month)<2){datain[i]=RTCDateTime.month;}else{datain[i]=RTCDateTime.month%10;};break;//
			
			case 6:if(fun(RTCDateTime.dayofmonth)<2){datain[i]=0;}else{datain[i]=RTCDateTime.dayofmonth/10;};break;//��
			case 7:if(fun(RTCDateTime.dayofmonth)<2){datain[i]=RTCDateTime.dayofmonth;}else{datain[i]=RTCDateTime.dayofmonth%10;};break;//
			
			case 8:if(fun(RTCDateTime.hour)<2){datain[i]=0;}else{datain[i]=RTCDateTime.hour/10;};break;//ʱ
			case 9:if(fun(RTCDateTime.hour)<2){datain[i]=RTCDateTime.hour;}else{datain[i]=RTCDateTime.hour%10;};break;//
			
			case 10:if(fun(RTCDateTime.minute)<2){datain[i]=0;}else{datain[i]=RTCDateTime.minute/10;};break;//��
			case 11:if(fun(RTCDateTime.minute)<2){datain[i]=RTCDateTime.minute;}else{datain[i]=RTCDateTime.minute%10;};break;//
			
			case 12:if(fun(RTCDateTime.second)<2){datain[i]=0;}else{datain[i]=RTCDateTime.second/10;};break;//��
			case 13:if(fun(RTCDateTime.second)<2){datain[i]=RTCDateTime.second;}else{datain[i]=RTCDateTime.second%10;};break;//
			
			default:break;
		}
	}
	return 0;
}

