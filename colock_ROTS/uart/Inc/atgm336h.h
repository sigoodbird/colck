#ifndef __ATGM336H_H
#define __ATGM336H_H

//USART3
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收
#define false 0
#define true 1
//USART3

//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define UTCDate_Length 6
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

//GPS接收结构体
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char UTCDate[UTCDate_Length];//UTC日期
	char isUsefull;		//定位信息是否有效
} _SaveData;

extern unsigned short int point1;
extern _SaveData Save_Data;//声明变量

//GPS时间结构体
typedef struct GPSDataTime
{
	unsigned int GPS_year;									//GPS 年
	unsigned int GPS_month;									//GPS 月
	unsigned int GPS_day;										//GPS 日
	unsigned int GPS_hour;									//GPS 时
	unsigned int GPS_minute;								//GPS 分
	unsigned int GPS_second;								//GPS 秒
} _Data_Time;

extern _Data_Time _GPS_Data_Time;//声明变量

void errorLog(int num);
void parseGpsBuffer(void);
void printGpsBuffer(void);
unsigned char UTCToBeijing(unsigned int UTCyear,unsigned char UTCmonth,unsigned char UTCday,unsigned int UTChour,unsigned char UTCminute,unsigned char UTCsecond);
unsigned char get_intermediate(void);

#endif
