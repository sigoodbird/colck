#ifndef _PARA_H_
#define _PARA_H_
/**
  ******************************************************************************
  * @file    para.h
  * @brief   在此定义设备的相关参数的结构体类型，方便调用请在main.h里 #include "para.h"
  ******************************************************************************
  * @attention
  *
  * Copyright 2022-8-6 LSQ
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
 ******************************************************************************
*/

typedef struct {
	//LED
	unsigned char light_state;//陶瓷灯状态
	
	//OLED
	unsigned char display_astronaut_state;//太空人屏保显示状态
	
	//GPS
	unsigned GPS_electric;//GPS模块供电状态
	
	//RTC时钟
	
	//增加人感
	
	//蜂鸣器  Buzzer
	unsigned char buzzer;
	
	//OTA在线升级
	unsigned char ota_flag;
	unsigned char ota_version;
	unsigned int ota_size;
	unsigned int ota_sum;

	unsigned char ota_receive;
	unsigned char ota_timeOut;

	unsigned int connectFlag;
	unsigned char address_type;//
	unsigned char address_flash;//地址
	unsigned char baud_rate;//波特率
	
	unsigned char RFS_flag;//恢复出厂设置

	unsigned char Model;//设备型号
	unsigned char Edition;//版本号
	unsigned char ID_UP;//设备ID高16位
	unsigned char ID_LOW;//设备ID低16位

}PARA_Struct;

extern PARA_Struct para;

#endif

