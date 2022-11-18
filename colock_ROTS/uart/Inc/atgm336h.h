#ifndef __ATGM336H_H
#define __ATGM336H_H

//USART3
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������3����
#define false 0
#define true 1
//USART3

//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define UTCDate_Length 6
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

//GPS���սṹ��
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char UTCDate[UTCDate_Length];//UTC����
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} _SaveData;

extern unsigned short int point1;
extern _SaveData Save_Data;//��������

//GPSʱ��ṹ��
typedef struct GPSDataTime
{
	unsigned int GPS_year;									//GPS ��
	unsigned int GPS_month;									//GPS ��
	unsigned int GPS_day;										//GPS ��
	unsigned int GPS_hour;									//GPS ʱ
	unsigned int GPS_minute;								//GPS ��
	unsigned int GPS_second;								//GPS ��
} _Data_Time;

extern _Data_Time _GPS_Data_Time;//��������

void errorLog(int num);
void parseGpsBuffer(void);
void printGpsBuffer(void);
unsigned char UTCToBeijing(unsigned int UTCyear,unsigned char UTCmonth,unsigned char UTCday,unsigned int UTChour,unsigned char UTCminute,unsigned char UTCsecond);
unsigned char get_intermediate(void);

#endif
