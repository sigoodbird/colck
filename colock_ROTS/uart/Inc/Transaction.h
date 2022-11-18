#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

//操作函数
void GPS_power_OFF(void);
void GPS_power_ON(void);
void light_OFF(void);
void light_OFF(void);
void Buzzer_OFF(void);
void Buzzer_OFF(void);

void GPS_Timing(void);//校时函数
void Screen_saver(void);/*屏保—太空人*/
void Screen_time(void);//显示时间
void Transaction_Process(void);//综合事件处理


/*多级菜单*/

//多级菜单结构体
typedef struct
{
	unsigned char current;
	unsigned char up;//向上翻索引号
	unsigned char down;//向下翻索引号
	unsigned char enter;//确认索引号
	void (*current_operation)();
} key_table;

extern key_table table[30];
extern signed short int last_index,func_index;
void menu(void);


#endif
