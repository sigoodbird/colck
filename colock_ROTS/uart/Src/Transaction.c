#include "Transaction.h"
#include "atgm336h.h"
#include "main.h"

//GPSģ��ͣ��
void GPS_power_OFF(){
	HAL_GPIO_WritePin(GPS_DY_GPIO_Port,GPS_DY_Pin,GPIO_PIN_SET);
	para.GPS_electric=0;
	//˳���ر��ж�
}

//GPSģ�鹩��
void GPS_power_ON(){
	HAL_GPIO_WritePin(GPS_DY_GPIO_Port,GPS_DY_Pin,GPIO_PIN_SET);
	//˳�������ж�
	para.GPS_electric=1;
}

/*
*Уʱ����
*/
void GPS_Timing(){
		int i,j;
		OLED_Clear();//����
		OLED_ShowString(15,23,"GPS");//��ʾ"GPS"
		OLED_Showchinese(40,23,6,1);//"У"
		OLED_Showchinese(55,23,7,1);//"ʱ"
		OLED_Showchinese(70,23,8,1);//"��"
		OLED_ShowString(86,23,".");//��ʾ"..."
		OLED_Refresh_Gram();//ˢ��
		delay_us(25000000);
		for(j=0;j<=5;j++)
		{
			parseGpsBuffer();//����GPS��Ϣ
			if(!get_intermediate())//�ָ�ʧ��
			{
				if(!UTCToBeijing(_GPS_Data_Time.GPS_year,_GPS_Data_Time.GPS_month,_GPS_Data_Time.GPS_day,_GPS_Data_Time.GPS_hour,_GPS_Data_Time.GPS_minute,_GPS_Data_Time.GPS_second))//Уʱʧ��
				{
				GPS_power_OFF();//�ر�GPS
				EXTI->PR=1<<5; //���LINE15�ϵ��жϱ�־λ
				OLED_Clear();//����
				OLED_Showchinese(30,23,6,1);//"У"
				OLED_Showchinese(45,23,7,1);//"ʱ"
				OLED_Showchinese(60,23,2,1);//"��"
				OLED_Showchinese(75,23,3,1);//"��"
				OLED_ShowString(90,23,".");//��ʾ"."
				OLED_Refresh_Gram();//ˢ��
				delay_us(5000000);
				OLED_Clear();//����
				//printf("Уʱ�ɹ�");
				return;
				}
			}
			OLED_ShowString(86+(4*(j+1)),23,".");//��ʾ"..."
			OLED_Refresh_Gram();//ˢ��
			if(j<5)delay_us(10000000);//��ʱ10�룬�ٽ����»�
		}
		GPS_power_ON();//�ر�GPS
		OLED_Clear();//����
		OLED_Showchinese(30,10,6,1);//"У"
		OLED_Showchinese(45,10,7,1);//"ʱ"
		OLED_Showchinese(60,10,9,1);//"ʧ"
		OLED_Showchinese(75,10,10,1);//"��"
		OLED_ShowString(90,10,"!");//��ʾ"��"
		OLED_Showchinese(5,35,11,1);//"��"
		OLED_Showchinese(20,35,12,1);//"��"
		OLED_Showchinese(35,35,13,1);//"��"
		OLED_Showchinese(50,35,14,1);//"��"
		OLED_Showchinese(65,35,15,1);//"��"
		OLED_Showchinese(83,35,16,1);//"��"
		OLED_Showchinese(98,35,17,1);//"��"
		OLED_ShowString(115,35,".");//��ʾ"."
		OLED_Refresh_Gram();//ˢ��
		for(i=0;i<=10;i++)//û������ѭ������ʾУʱʧ��
		{
			delay_us(10000);
		}
		delay_us(5000000);
		OLED_Clear();//����
}

/*
*�ص�
*/
void light_OFF(){
	HAL_GPIO_WritePin(light_GPIO_Port,light_Pin,GPIO_PIN_RESET);
	para.light_state=0;
}

/*
*����
*/
void light_ON(){
	HAL_GPIO_WritePin(light_GPIO_Port,light_Pin,GPIO_PIN_SET);
	para.light_state=1;
}

/*
*�ط�����
*/
void Buzzer_OFF(){
	HAL_GPIO_WritePin(BUZZ_GPIO_Port,BUZZ_Pin,GPIO_PIN_RESET);
	para.buzzer=0;
}

/*
*��������
*/
void Buzzer_ON(){
	HAL_GPIO_WritePin(BUZZ_GPIO_Port,BUZZ_Pin,GPIO_PIN_SET);
	para.buzzer=1;
}

unsigned int i=0;

/*������̫����*/
void Screen_saver(){
	//��ʾͼ��
	if(i>10)i=0;
	Picture_display(astronaut[i]);
	i++;
}

//��ʾʱ��
void Screen_time(){
	
		DS3231_getdate(&RTCDateTime);//�����ڡ������Ż�
		DS3231_gettime(&RTCDateTime);//��ʱ��
		
		/*��ʾʱ��*/
		OLED_ShowNumber(0,50,RTCDateTime.year-2000,2,12);//��Ļ�����
		OLED_ShowString(15,47,"-");//��ʾ"-"
		OLED_ShowNumber(25,50,RTCDateTime.month,2,12);//��Ļ�����
		OLED_ShowString(37,47,"-");//��ʾ"-"
		OLED_ShowNumber(48,50,RTCDateTime.dayofmonth,2,12);//��Ļ�����
		
		OLED_ShowNumber(73,47,RTCDateTime.hour,2,16);//��Ļ���ʱ
		OLED_ShowString(88,47,":");//��ʾ":"
		OLED_ShowNumber(93,47,RTCDateTime.minute,2,16);//��Ļ�����
		OLED_ShowString(108,47,":");//��ʾ":"
		OLED_ShowNumber(113,47,RTCDateTime.second,2,16);//��Ļ�����
		OLED_Refresh_Gram();//ˢ��
}

//�༶�˵�

void fun_0() {
	//��������
}

//���Ų˵� ��������  ʹ�õ�PWM
//


void fun_a1() {
	OLED_ShowString(15,10,">");//��ʾ"<"
	OLED_ShowNumber(25,10,1,2,16);
	OLED_ShowNumber(25,25,2,2,16);
	OLED_ShowNumber(25,40,3,2,16);
}

void fun_b1() {
	OLED_ShowString(15,25,">");//��ʾ"<"
	OLED_ShowNumber(25,10,1,2,16);
	OLED_ShowNumber(25,25,2,2,16);
	OLED_ShowNumber(25,40,3,2,16);
}

void fun_c1() {
	OLED_ShowString(15,40,">");//��ʾ"<"
	OLED_ShowNumber(25,10,1,2,16);
	OLED_ShowNumber(25,25,2,2,16);
	OLED_ShowNumber(25,40,3,2,16);
}
void fun_a21(){
	//�����˵�
}
void fun_a22(){

}

//�˵��Ļ�����
key_table table[30] =
{
	//��0��
	{0,0,0,1,(*fun_0)},

	//��1��
	{1,3,2,5,(*fun_a1)},
	{2,1,3,6,(*fun_b1)},
	{3,2,1,0,(*fun_c1)},

	//��2��
	{5,6,6,1,(*fun_a21)},
	{6,5,5,1,(*fun_a22)},
};

//�˵���ʾ
int16_t last_index=0,func_index=1;
void (*current_operation_index)(); //����һ������ָ��
void menu(){
	current_operation_index = table[func_index].current_operation;
	OLED_Clear();
	(*current_operation_index)();//ִ�е�ǰ��������
	OLED_Refresh_Gram();
	last_index = func_index;
	OLED_Refresh_Gram();//ˢ��
}

//��ʾ����ʱ��
void light_time(){
	
	//��ǰʱ��-��¼ʱ��
	
}

//����Ƿ�����
void infrared_testing(){
	
}

//������ѧϰ����
void remind_not_studying(){
	//������ʱ������X��������ȥ�档
}

//�Զ���������
void Adjust_brightness(){
	//���ݻ��������Ⱥ��л�ϰ�ߵ�������
}

/*
*�ۺ��¼�����
*/
void Transaction_Process(){//������

}

