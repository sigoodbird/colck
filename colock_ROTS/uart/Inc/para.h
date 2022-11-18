#ifndef _PARA_H_
#define _PARA_H_
/**
  ******************************************************************************
  * @file    para.h
  * @brief   �ڴ˶����豸����ز����Ľṹ�����ͣ������������main.h�� #include "para.h"
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
	unsigned char light_state;//�մɵ�״̬
	
	//OLED
	unsigned char display_astronaut_state;//̫����������ʾ״̬
	
	//GPS
	unsigned GPS_electric;//GPSģ�鹩��״̬
	
	//RTCʱ��
	
	//�����˸�
	
	//������  Buzzer
	unsigned char buzzer;
	
	//OTA��������
	unsigned char ota_flag;
	unsigned char ota_version;
	unsigned int ota_size;
	unsigned int ota_sum;

	unsigned char ota_receive;
	unsigned char ota_timeOut;

	unsigned int connectFlag;
	unsigned char address_type;//
	unsigned char address_flash;//��ַ
	unsigned char baud_rate;//������
	
	unsigned char RFS_flag;//�ָ���������

	unsigned char Model;//�豸�ͺ�
	unsigned char Edition;//�汾��
	unsigned char ID_UP;//�豸ID��16λ
	unsigned char ID_LOW;//�豸ID��16λ

}PARA_Struct;

extern PARA_Struct para;

#endif

