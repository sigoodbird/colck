#ifndef _FLASH_H_
#define _FLASH_H_
#include "main.h"

#define START_ADDRESS   0x08007C00  //起始地址

/****************************************************************/

void Init_Para(void);
void Flash_ReadPara(void);
void RecoverParam(void);
HAL_StatusTypeDef Flash_WritePara(void);
void Update_PARA(void);

/*
#define ALR_TempUpper_A		(21)		//数据打包的位置
#define ALR_TempLower_A		(22)
#define ALR_HumiUpper_A		(23)
#define ALR_HumiLower_A		(24)
#define ALR_VOC_A           (7)
#define ALR_HCHO_A          (8)	//写入的位置
#define ALR_PM25_A          (9)
#define ALR_PM1_A           (10)
#define ALR_PM10_A          (11)
#define ALR_CO2_A           (12)
#define ALR_EN_A			(13)
#define CFswitch_A			(14)
#define CAL_TEMPERATURE_A	(3)//校准值
#define CAL_HUMI_A			(4)
#define CAL_VOC_A	        (5)
#define CAL_HCHO_A			(6)
#define UPDATATIME_A			(25)

#define ADDRESSTYPE_A       (26)
#define ADDRESS_A           (27)
*/

/*flash内部储存地址可以跟rs485地址不同*/
//校验位

//#define ALR_Humi_A		    (2)
//#define ALR_Temp_A         (3)

#define MODEL_A                 (8)//设备型号
#define Edition_A               (9)//版本号
#define IDUpper_A               (10)
#define IDLower_A               (11)
#define alarm_temperature_up_A  (12)
#define alarm_temperature_up_flag_A  (13)
#define alarm_temperature_low_A (14)
#define alarm_temperature_low_flag_A (15)
#define alarm_humi_up_A  (16)
#define alarm_humi_up_flag_A  (17)
#define alarm_humi_low_A (18)
#define alarm_humi_low_flag_A (19)

#define CAL_TEMPERATURE_A   (29)
#define CAL_HUMI_A  (30)

#define ADDRESS_FLASH_A   (32)//地址
#define BAUD_RATE_A       (33)//波特率


uint32_t GetPage(uint32_t Addr);
void OTG_Save(unsigned long start,unsigned char *buf,unsigned char length);
extern unsigned char flash_wirteflag;
extern unsigned int flash_writeCount;
extern unsigned int flash_error;
#endif

