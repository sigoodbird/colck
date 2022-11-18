#include "ds3231.h"
#include "usart.h"
#include "delay.h"

DateTime RTCDateTime;

/*管脚方向设置*/
void DS3231_SDA_GPIO_OUT(void)//SDA:PB5  //IIC通信管脚管脚定义
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

/**************************实现函数********************************************
*函数原型:		void IIC_Start(void)
*功　　能:		产生IIC起始信号
*******************************************************************************/
int IIC_Start(void)
{
	DS3231_SDA_GPIO_OUT();     //sda线输出
	DS3231_SDA_H;
	if(!DS3231_SDA_READ)return 0;	
	DS3231_SCL_H;
	delay_us(1);
 	DS3231_SDA_L;//START:when CLK is high,DATA change form high to low 
	if(DS3231_SDA_READ)return 0;
	delay_us(1);
	DS3231_SCL_L;//钳住I2C总线，准备发送或接收数据 
	return 1;
}

/**************************实现函数********************************************
*函数原型:		void IIC_Stop(void)
*功　　能:	    //产生IIC停止信号
*******************************************************************************/	  
void IIC_Stop(void)
{
	DS3231_SDA_GPIO_OUT();//sda线输出
	DS3231_SCL_L;
	DS3231_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	DS3231_SCL_H; 
	DS3231_SDA_H;//发送I2C总线结束信号
	delay_us(1);							   	
}

/**************************实现函数********************************************
*函数原型:		uint8_t IIC_Wait_Ack(void)
*功　　能:	    等待应答信号到来 
//返回值：1，接收应答失败
//        0，接收应答成功
*******************************************************************************/
int IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	DS3231_SDA_GPIO_IN();      //SDA设置为输入  
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
			return 1;//失败
		}
	  delay_us(1);
	}
	DS3231_SCL_L;//时钟输出0 	   
	return 0;  
} 

/**************************实现函数********************************************
*函数原型:		void IIC_Ack(void)
*功　　能:	    产生ACK应答
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
	
/**************************实现函数********************************************
*函数原型:		void IIC_NAck(void)
*功　　能:	    产生NACK应答
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
/**************************实现函数********************************************
*函数原型:		void IIC_Send_Byte(uint8_t txd)
*功　　能:	    IIC发送一个字节
*******************************************************************************/		  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		DS3231_SDA_GPIO_OUT(); 	    
    DS3231_SCL_L;//拉低时钟开始数据传输
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
  
/**************************实现函数********************************************
*函数原型:		bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
*功　　能:		写函数
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
/**************************实现函数********************************************
*函数原型:		bool i2cRdad(uint8_t addr, uint8_t reg, uint8_t data)
*功　　能:		读函数
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


/**************************实现函数********************************************
*函数原型:		uint8_t IIC_Read_Byte(unsigned char ack)
*功　　能:	    //读1个字节，ack=1时，发送ACK，ack=0，发送nACK 
*******************************************************************************/  
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	DS3231_SDA_GPIO_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
		DS3231_SCL_L; 
		delay_us(2);
		DS3231_SCL_H;
		receive<<=1;
		if(DS3231_SDA_READ)receive++;   //读数据线高低位
		delay_us(2);
    }
    if (ack)
        IIC_Ack(); //发送ACK 
    else
        IIC_NAck();//发送nACK  
    return receive;
}

/**************************实现函数********************************************
*函数原型:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	I2C_Addr  目标设备地址
		addr	   寄存器地址
返回   读出来的值
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();
	IIC_Send_Byte(I2C_Addr);//发送写命令
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;  //发送地址
	IIC_Wait_Ack();
	//IIC_Stop();//产生一个停止条件
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;      //进入接收模式			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);
  IIC_Stop();//产生一个停止条件

	return res;
}

uint8_t IIC_DS3231_ByteWrite(uint8_t WriteAddr , uint8_t date)//写函数
{
	IIC_Start();//开始条件
	IIC_Send_Byte(DS3231_ADDRESS_Write);//发送从机地址
	if(IIC_Wait_Ack()){
		//printf("发送从机地址失败\r\n");
		return 1;
	}
	IIC_Send_Byte(WriteAddr);//写地址,写地址失败
	if(IIC_Wait_Ack()){
		//printf("写地址失败\r\n");
		return 2;
	}
	IIC_Send_Byte(date);//写数据
	if(IIC_Wait_Ack()){
		//printf("写数据失败r\n");
		return 3;
	}
		
	IIC_Stop();
	return 0;
}

uint8_t IIC_DS3231_ByteRead(uint8_t ReadAddr,uint8_t* Receive)//读函数
{
	uint8_t data = 0;
	
	IIC_Start();													//产生起始位
	IIC_Send_Byte(DS3231_ADDRESS_Write); 	//发送从机地址（写模式）
	if(IIC_Wait_Ack())										//等待响应
		return 1;
	IIC_Send_Byte(ReadAddr);							//发送寄存器地址
	if(IIC_Wait_Ack())										//等待响应
		return 2;
	IIC_Start();													//重复起始位
	IIC_Send_Byte(DS3231_ADDRESS_Read);		//发送从机地址（读模式）
	if(IIC_Wait_Ack())										//等待响应
		return 3;
	data = IIC_Read_Byte(0);							//读取数据，参数设为0 --- NACK
	*Receive = data;											//将结果赋值给接收位
	IIC_Stop();
	return 0;
}

uint8_t DS3231_setDate(uint8_t year,uint8_t mon,uint8_t day)//设置年月日
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

static uint8_t bcdToDec(uint8_t byte)//2转10
{
	uint8_t temp_H , temp_L;
	temp_L = byte & 0x0f;
	temp_H = (byte & 0xf0) >> 4;
	return ( temp_H * 10 )+ temp_L;
}

uint8_t DS3231_gettime(DateTime* ans)//读时间
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

uint8_t DS3231_getdate(DateTime* ans)//读日期
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

//判断有多少位数
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

//将RCT时间结构体放入写包里
unsigned char RCTdatatoWBlockData(unsigned char *datain)
{	
	int i;
	datain[2]=(RTCDateTime.year%100)/10;
	datain[3]=(RTCDateTime.year%10);

	for(i=3;i<15;i++)
	{
		switch(i)
		{
			case 4:if(fun(RTCDateTime.month)<2){datain[i]=0;}else{datain[i]=RTCDateTime.month/10;};break;//月
			case 5:if(fun(RTCDateTime.month)<2){datain[i]=RTCDateTime.month;}else{datain[i]=RTCDateTime.month%10;};break;//
			
			case 6:if(fun(RTCDateTime.dayofmonth)<2){datain[i]=0;}else{datain[i]=RTCDateTime.dayofmonth/10;};break;//日
			case 7:if(fun(RTCDateTime.dayofmonth)<2){datain[i]=RTCDateTime.dayofmonth;}else{datain[i]=RTCDateTime.dayofmonth%10;};break;//
			
			case 8:if(fun(RTCDateTime.hour)<2){datain[i]=0;}else{datain[i]=RTCDateTime.hour/10;};break;//时
			case 9:if(fun(RTCDateTime.hour)<2){datain[i]=RTCDateTime.hour;}else{datain[i]=RTCDateTime.hour%10;};break;//
			
			case 10:if(fun(RTCDateTime.minute)<2){datain[i]=0;}else{datain[i]=RTCDateTime.minute/10;};break;//分
			case 11:if(fun(RTCDateTime.minute)<2){datain[i]=RTCDateTime.minute;}else{datain[i]=RTCDateTime.minute%10;};break;//
			
			case 12:if(fun(RTCDateTime.second)<2){datain[i]=0;}else{datain[i]=RTCDateTime.second/10;};break;//秒
			case 13:if(fun(RTCDateTime.second)<2){datain[i]=RTCDateTime.second;}else{datain[i]=RTCDateTime.second%10;};break;//
			
			default:break;
		}
	}
	return 0;
}

