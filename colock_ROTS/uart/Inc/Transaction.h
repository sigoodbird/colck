#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

//��������
void GPS_power_OFF(void);
void GPS_power_ON(void);
void light_OFF(void);
void light_OFF(void);
void Buzzer_OFF(void);
void Buzzer_OFF(void);

void GPS_Timing(void);//Уʱ����
void Screen_saver(void);/*������̫����*/
void Screen_time(void);//��ʾʱ��
void Transaction_Process(void);//�ۺ��¼�����


/*�༶�˵�*/

//�༶�˵��ṹ��
typedef struct
{
	unsigned char current;
	unsigned char up;//���Ϸ�������
	unsigned char down;//���·�������
	unsigned char enter;//ȷ��������
	void (*current_operation)();
} key_table;

extern key_table table[30];
extern signed short int last_index,func_index;
void menu(void);


#endif
