#include "atgm336h.h"
#include "ds3231.h"//����RTC�ĺ������������ں�ʱ��
#include <string.h>

_Data_Time _GPS_Data_Time;
_SaveData Save_Data;

/*GPS*/
void errorLog(int num)
{
	
	while (1)
	{
	  	//printf("ERROR%d\r\n",num);
	}
}

//����GPS����
void parseGpsBuffer()
{
	char *subString;//ָ��
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		//printf("**************\r\n");
		//printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 9 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2];
					char Not_used_data[2];
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡ״̬
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W
						case 7:memcpy(Not_used_data, subString, subStringNext - subString);break;//�ٶ�
						case 8:memcpy(Not_used_data, subString, subStringNext - subString);break;//��λ��
						case 9:memcpy(Save_Data.UTCDate, subString, subStringNext - subString);break;//UTCdate  UTC����
						//memcpy(Ŀ�ĵ�ַ��Ŀ�꣬��С);

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')//���Ƿ�λ�ɹ�����Save_Data�ࡣ
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		printf("�����ɹ���");
		printf("\r\n");
		
		printf("Save_Data.UTCTime = ");//printfʱ��
		printf(Save_Data.UTCTime);
		//printf("ʱ:%2d ��:%2d ��:%2d",Save_Data.UTCTime[2]);
		printf("\r\n");
		
		printf("Save_Data.UTCDate = ");//printf����
		printf(Save_Data.UTCDate);
		printf("\r\n");
		
		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			
			
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}

/*�ָ�GPS:day/time�õ���ת���Ľṹ��*/
uint8_t get_intermediate()
{
	char year[2];
	char month[2];
	char day[2];
	char hour[2];
	char minute[2];
	char second[2];
	
	char hour2[2];
	char minute2[2];
	char second2[2];
	if (strlen(Save_Data.UTCDate))//�ж��ַ����鳤�ȣ��ж��Ƿ������ʱ���ַ����顣
	{
		//�ָ�
		year[0]=Save_Data.UTCDate[4];
		year[1]=Save_Data.UTCDate[5];
		month[0]=Save_Data.UTCDate[2];
		month[1]=Save_Data.UTCDate[3];
		day[0]=Save_Data.UTCDate[0];
		day[1]=Save_Data.UTCDate[1];
		
		hour[0]=Save_Data.UTCTime[0];
		hour[1]=Save_Data.UTCTime[1];
		minute[0]=Save_Data.UTCTime[2];
		minute[1]=Save_Data.UTCTime[3];
		second[0]=Save_Data.UTCTime[4];
		second[1]=Save_Data.UTCTime[5];
		
		//��Ӹ����ַ�������Ӱ�졣δ��ԭ�򡣣���������
		strncpy(hour2,hour, 2);
		strncpy(minute2,minute, 2);
		strncpy(second2,second, 2);
		
		//����charתint
		_GPS_Data_Time.GPS_year=atoi(year)+2000;
		_GPS_Data_Time.GPS_month=atoi(month);
		_GPS_Data_Time.GPS_day=atoi(day);
		_GPS_Data_Time.GPS_hour=atoi(hour2);
		_GPS_Data_Time.GPS_minute=atoi(minute2);
		_GPS_Data_Time.GPS_second=atoi(second2);
		//printf("_GPS_Data_Time.GPS_hour%d",_GPS_Data_Time.GPS_hour);
		
		//�������
		memset(Save_Data.UTCDate,'\0',sizeof(Save_Data.UTCDate));//�����������
		memset(Save_Data.UTCTime,'\0',sizeof(Save_Data.UTCTime));//���ʱ������
		//printf("�ָ���ɣ�");
		return 0;//�ָ�ɹ�������0.
		//printf("%d��%d��%d��%dʱ%d��%d��",_GPS_Data_Time.GPS_year,_GPS_Data_Time.GPS_month,_GPS_Data_Time.GPS_day,_GPS_Data_Time.GPS_hour,_GPS_Data_Time.GPS_minute,_GPS_Data_Time.GPS_second);
	}
	//printf("�ָ�ʧ�ܣ�");
	return 1;//�ָ�ɹ�������1.
}

/*UTCʱ��ת����ʱ��*/
uint8_t UTCToBeijing(unsigned int UTCyear,unsigned char UTCmonth,unsigned char UTCday,unsigned int UTChour,unsigned char UTCminute,unsigned char UTCsecond)
{
		int year=0,month=0,day=0,hour=0;
    int lastday = 0;			// �µ����һ��
    //int lastlastday = 0;			//�ϸ��µ����һ��
		int minute;
		int seconcond;
	
	  year=UTCyear;
		month=UTCmonth;
	  day=UTCday;
	  hour=UTChour+8;//UTC+8			//��������������+8
		minute=UTCminute;
		seconcond=UTCsecond;
	
	  if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
		{
        lastday = 31;
//        if(month == 3)
//				{
//            if((year%400 == 0)||(year%4 == 0 && year%100 != 0))//�ж��Ƿ�������
//                lastlastday = 29;//����2��29��
//            else
//                lastlastday = 28;//ƽ��2��28��
//        }
//        if(month == 8)
//            lastlastday = 31;
    }
    else 
		if(month == 4 || month == 6 || month == 9 || month == 11)
		{
        lastday = 30;
        //lastlastday = 31;
    }
    else
		{
        //lastlastday = 31;
        if((year%400 == 0)||(year%4 == 0 && year%100 != 0))
            lastday = 29;//����2��29��
        else
            lastday = 28;//ƽ��2��28��
    }
		if(hour >= 24)//Сʱ����24:00,��ȥ24:00,���ڼ�1
		{
				hour -= 24;
				day += 1; 
				if(day > lastday)//��ǰ���ڴ��ڸ��µ����һ��,��ȥ����µ�����,�·ݼ�1
				{ 
						day -= lastday;
						month += 1;

						if(month > 12)//�·ݴ���12,��ȥ12,��ݼ�1
						{
								month -= 12;
								year += 1;
						}
				}
		}
		DS3231_setDate(year-2000,month,day);//���������� //�ڴ�����ʱ�䲻����ģ�黯��ƣ������㡣(-2000,����Ϊֻд22��������)
		//����ʱ���룬���óɹ�����0.
		//printf("����ʱ��%2d/%02d/%02d,%02d:%02d:%02d\r\n",year,month,day,hour,minute,seconcond);
		if(DS3231_setTime(hour,minute,seconcond))return 1;else return 0;
		

}
