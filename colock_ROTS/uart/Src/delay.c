#include "delay.h"
#include "main.h"

/*us��ʱ����*/
void delay_us(unsigned int Delay){
	unsigned int cnt =Delay*16;//32Mhz  *8 ,����Ƶ����������
	unsigned int i=0;
	for(i=0;i<cnt;i++)__NOP();
}

/*ms��ʱ����*/
void delay_ms(unsigned int Delay){
	unsigned int cnt =Delay*16000;//32Mhz  *8 ,����Ƶ����������
	unsigned int i=0;
	for(i=0;i<cnt;i++)__NOP();
}

