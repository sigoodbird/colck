#include "Transaction.h"
#include "atgm336h.h"
#include "main.h"

//GPS模块停电
void GPS_power_OFF(){
	HAL_GPIO_WritePin(GPS_DY_GPIO_Port,GPS_DY_Pin,GPIO_PIN_SET);
	para.GPS_electric=0;
	//顺带关闭中断
}

//GPS模块供电
void GPS_power_ON(){
	HAL_GPIO_WritePin(GPS_DY_GPIO_Port,GPS_DY_Pin,GPIO_PIN_SET);
	//顺带开启中断
	para.GPS_electric=1;
}

/*
*校时程序
*/
void GPS_Timing(){
		int i,j;
		OLED_Clear();//清屏
		OLED_ShowString(15,23,"GPS");//显示"GPS"
		OLED_Showchinese(40,23,6,1);//"校"
		OLED_Showchinese(55,23,7,1);//"时"
		OLED_Showchinese(70,23,8,1);//"中"
		OLED_ShowString(86,23,".");//显示"..."
		OLED_Refresh_Gram();//刷新
		delay_us(25000000);
		for(j=0;j<=5;j++)
		{
			parseGpsBuffer();//解析GPS消息
			if(!get_intermediate())//分割失败
			{
				if(!UTCToBeijing(_GPS_Data_Time.GPS_year,_GPS_Data_Time.GPS_month,_GPS_Data_Time.GPS_day,_GPS_Data_Time.GPS_hour,_GPS_Data_Time.GPS_minute,_GPS_Data_Time.GPS_second))//校时失败
				{
				GPS_power_OFF();//关闭GPS
				EXTI->PR=1<<5; //清除LINE15上的中断标志位
				OLED_Clear();//清屏
				OLED_Showchinese(30,23,6,1);//"校"
				OLED_Showchinese(45,23,7,1);//"时"
				OLED_Showchinese(60,23,2,1);//"成"
				OLED_Showchinese(75,23,3,1);//"功"
				OLED_ShowString(90,23,".");//显示"."
				OLED_Refresh_Gram();//刷新
				delay_us(5000000);
				OLED_Clear();//清屏
				//printf("校时成功");
				return;
				}
			}
			OLED_ShowString(86+(4*(j+1)),23,".");//显示"..."
			OLED_Refresh_Gram();//刷新
			if(j<5)delay_us(10000000);//延时10秒，再进入下回
		}
		GPS_power_ON();//关闭GPS
		OLED_Clear();//清屏
		OLED_Showchinese(30,10,6,1);//"校"
		OLED_Showchinese(45,10,7,1);//"时"
		OLED_Showchinese(60,10,9,1);//"失"
		OLED_Showchinese(75,10,10,1);//"败"
		OLED_ShowString(90,10,"!");//显示"！"
		OLED_Showchinese(5,35,11,1);//"请"
		OLED_Showchinese(20,35,12,1);//"到"
		OLED_Showchinese(35,35,13,1);//"开"
		OLED_Showchinese(50,35,14,1);//"阔"
		OLED_Showchinese(65,35,15,1);//"处"
		OLED_Showchinese(83,35,16,1);//"重"
		OLED_Showchinese(98,35,17,1);//"试"
		OLED_ShowString(115,35,".");//显示"."
		OLED_Refresh_Gram();//刷新
		for(i=0;i<=10;i++)//没有跳出循环，表示校时失败
		{
			delay_us(10000);
		}
		delay_us(5000000);
		OLED_Clear();//清屏
}

/*
*关灯
*/
void light_OFF(){
	HAL_GPIO_WritePin(light_GPIO_Port,light_Pin,GPIO_PIN_RESET);
	para.light_state=0;
}

/*
*开灯
*/
void light_ON(){
	HAL_GPIO_WritePin(light_GPIO_Port,light_Pin,GPIO_PIN_SET);
	para.light_state=1;
}

/*
*关蜂鸣器
*/
void Buzzer_OFF(){
	HAL_GPIO_WritePin(BUZZ_GPIO_Port,BUZZ_Pin,GPIO_PIN_RESET);
	para.buzzer=0;
}

/*
*开蜂鸣器
*/
void Buzzer_ON(){
	HAL_GPIO_WritePin(BUZZ_GPIO_Port,BUZZ_Pin,GPIO_PIN_SET);
	para.buzzer=1;
}

unsigned int i=0;

/*屏保—太空人*/
void Screen_saver(){
	//显示图像
	if(i>10)i=0;
	Picture_display(astronaut[i]);
	i++;
}

//显示时间
void Screen_time(){
	
		DS3231_getdate(&RTCDateTime);//读日期——可优化
		DS3231_gettime(&RTCDateTime);//读时间
		
		/*显示时间*/
		OLED_ShowNumber(0,50,RTCDateTime.year-2000,2,12);//屏幕输出年
		OLED_ShowString(15,47,"-");//显示"-"
		OLED_ShowNumber(25,50,RTCDateTime.month,2,12);//屏幕输出月
		OLED_ShowString(37,47,"-");//显示"-"
		OLED_ShowNumber(48,50,RTCDateTime.dayofmonth,2,12);//屏幕输出日
		
		OLED_ShowNumber(73,47,RTCDateTime.hour,2,16);//屏幕输出时
		OLED_ShowString(88,47,":");//显示":"
		OLED_ShowNumber(93,47,RTCDateTime.minute,2,16);//屏幕输出分
		OLED_ShowString(108,47,":");//显示":"
		OLED_ShowNumber(113,47,RTCDateTime.second,2,16);//屏幕输出秒
		OLED_Refresh_Gram();//刷新
}

//多级菜单

void fun_0() {
	//开机画面
}

//安排菜单 调节亮度  使用到PWM
//


void fun_a1() {
	OLED_ShowString(15,10,">");//显示"<"
	OLED_ShowNumber(25,10,1,2,16);
	OLED_ShowNumber(25,25,2,2,16);
	OLED_ShowNumber(25,40,3,2,16);
}

void fun_b1() {
	OLED_ShowString(15,25,">");//显示"<"
	OLED_ShowNumber(25,10,1,2,16);
	OLED_ShowNumber(25,25,2,2,16);
	OLED_ShowNumber(25,40,3,2,16);
}

void fun_c1() {
	OLED_ShowString(15,40,">");//显示"<"
	OLED_ShowNumber(25,10,1,2,16);
	OLED_ShowNumber(25,25,2,2,16);
	OLED_ShowNumber(25,40,3,2,16);
}
void fun_a21(){
	//二级菜单
}
void fun_a22(){

}

//菜单的换规则
key_table table[30] =
{
	//第0层
	{0,0,0,1,(*fun_0)},

	//第1层
	{1,3,2,5,(*fun_a1)},
	{2,1,3,6,(*fun_b1)},
	{3,2,1,0,(*fun_c1)},

	//第2层
	{5,6,6,1,(*fun_a21)},
	{6,5,5,1,(*fun_a22)},
};

//菜单显示
int16_t last_index=0,func_index=1;
void (*current_operation_index)(); //定义一个函数指针
void menu(){
	current_operation_index = table[func_index].current_operation;
	OLED_Clear();
	(*current_operation_index)();//执行当前操作函数
	OLED_Refresh_Gram();
	last_index = func_index;
	OLED_Refresh_Gram();//刷新
}

//显示开灯时长
void light_time(){
	
	//当前时间-记录时间
	
}

//检测是否有人
void infrared_testing(){
	
}

//防沉迷学习提醒
void remind_not_studying(){
	//当开灯时长大于X分钟提醒去玩。
}

//自动调节亮度
void Adjust_brightness(){
	//根据环境光亮度和有户习惯调节亮度
}

/*
*综合事件处理
*/
void Transaction_Process(){//事务处理

}

