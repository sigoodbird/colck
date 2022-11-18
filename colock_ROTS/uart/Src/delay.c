#include "delay.h"
#include "main.h"

/*us延时函数*/
void delay_us(unsigned int Delay){
	unsigned int cnt =Delay*16;//32Mhz  *8 ,其他频率其他倍数
	unsigned int i=0;
	for(i=0;i<cnt;i++)__NOP();
}

/*ms延时函数*/
void delay_ms(unsigned int Delay){
	unsigned int cnt =Delay*16000;//32Mhz  *8 ,其他频率其他倍数
	unsigned int i=0;
	for(i=0;i<cnt;i++)__NOP();
}

