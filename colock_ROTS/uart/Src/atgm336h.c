#include "atgm336h.h"
#include "ds3231.h"//导入RTC的函数，设置日期和时间
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

//解析GPS数据
void parseGpsBuffer()
{
	char *subString;//指针
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
					errorLog(1);	//解析错误
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
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取状态
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W
						case 7:memcpy(Not_used_data, subString, subStringNext - subString);break;//速度
						case 8:memcpy(Not_used_data, subString, subStringNext - subString);break;//方位角
						case 9:memcpy(Save_Data.UTCDate, subString, subStringNext - subString);break;//UTCdate  UTC日期
						//memcpy(目的地址，目标，大小);

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')//将是否定位成功放如Save_Data类。
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
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
		printf("解析成功！");
		printf("\r\n");
		
		printf("Save_Data.UTCTime = ");//printf时间
		printf(Save_Data.UTCTime);
		//printf("时:%2d 分:%2d 秒:%2d",Save_Data.UTCTime[2]);
		printf("\r\n");
		
		printf("Save_Data.UTCDate = ");//printf日期
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

/*分割GPS:day/time得到可转换的结构体*/
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
	if (strlen(Save_Data.UTCDate))//判断字符数组长度，判断是否解析到时间字符数组。
	{
		//分割
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
		
		//添加复制字符，减少影响。未明原因。？？？？？
		strncpy(hour2,hour, 2);
		strncpy(minute2,minute, 2);
		strncpy(second2,second, 2);
		
		//进行char转int
		_GPS_Data_Time.GPS_year=atoi(year)+2000;
		_GPS_Data_Time.GPS_month=atoi(month);
		_GPS_Data_Time.GPS_day=atoi(day);
		_GPS_Data_Time.GPS_hour=atoi(hour2);
		_GPS_Data_Time.GPS_minute=atoi(minute2);
		_GPS_Data_Time.GPS_second=atoi(second2);
		//printf("_GPS_Data_Time.GPS_hour%d",_GPS_Data_Time.GPS_hour);
		
		//清空数组
		memset(Save_Data.UTCDate,'\0',sizeof(Save_Data.UTCDate));//清空日期数组
		memset(Save_Data.UTCTime,'\0',sizeof(Save_Data.UTCTime));//清空时间数组
		//printf("分割完成！");
		return 0;//分割成功，返回0.
		//printf("%d年%d月%d日%d时%d分%d秒",_GPS_Data_Time.GPS_year,_GPS_Data_Time.GPS_month,_GPS_Data_Time.GPS_day,_GPS_Data_Time.GPS_hour,_GPS_Data_Time.GPS_minute,_GPS_Data_Time.GPS_second);
	}
	//printf("分割失败！");
	return 1;//分割不成功，返回1.
}

/*UTC时间转北京时间*/
uint8_t UTCToBeijing(unsigned int UTCyear,unsigned char UTCmonth,unsigned char UTCday,unsigned int UTChour,unsigned char UTCminute,unsigned char UTCsecond)
{
		int year=0,month=0,day=0,hour=0;
    int lastday = 0;			// 月的最后一天
    //int lastlastday = 0;			//上个月的最后一天
		int minute;
		int seconcond;
	
	  year=UTCyear;
		month=UTCmonth;
	  day=UTCday;
	  hour=UTChour+8;//UTC+8			//北京东八区所以+8
		minute=UTCminute;
		seconcond=UTCsecond;
	
	  if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
		{
        lastday = 31;
//        if(month == 3)
//				{
//            if((year%400 == 0)||(year%4 == 0 && year%100 != 0))//判断是否是闰年
//                lastlastday = 29;//闰年2月29天
//            else
//                lastlastday = 28;//平年2月28天
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
            lastday = 29;//闰年2月29天
        else
            lastday = 28;//平年2月28天
    }
		if(hour >= 24)//小时大于24:00,减去24:00,日期加1
		{
				hour -= 24;
				day += 1; 
				if(day > lastday)//当前日期大于该月的最后一天,减去这个月的天数,月份加1
				{ 
						day -= lastday;
						month += 1;

						if(month > 12)//月份大于12,减去12,年份加1
						{
								month -= 12;
								year += 1;
						}
				}
		}
		DS3231_setDate(year-2000,month,day);//设置年月日 //在此设置时间不利于模块化设计，但方便。(-2000,是因为只写22进就行了)
		//设置时分秒，设置成功返回0.
		//printf("北京时间%2d/%02d/%02d,%02d:%02d:%02d\r\n",year,month,day,hour,minute,seconcond);
		if(DS3231_setTime(hour,minute,seconcond))return 1;else return 0;
		

}
