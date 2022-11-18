#include "flash.h"
//flash���

#define u32 unsigned long
#define u8 unsigned char
unsigned int  paraBuf[64];
unsigned int  paraBufWrite[64];//д������ݰ�
unsigned char flash_wirteflag;
unsigned int flash_writeCount;
unsigned int flash_error;

unsigned int  readWord(u8 position)//����,���ض�Ӧ������
{
    return paraBuf[position];
}

void writeWord(u8 position,u32 data)//�������
{
    paraBuf[position] = data;
}

void Init_Para(void)//��ȡ����
{
  unsigned char Start_L,Start_H;
	unsigned int temp;
	u8 CAT_Val;
	Flash_ReadPara();
	Start_L = readWord(0);//����ȡ��
	Start_H = readWord(1);

	if((Start_L == 0x5C)&&(Start_L == (u8)(~Start_H)))//flash��������
	{
	    /*
        para.alarm_temperature_up = readWord(ALR_TempUpper_A);//��paraBuf[]�������ȡ��  //��Ҫ��
        para.alarm_temperature_low = readWord(ALR_TempLower_A);
        para.alarm_humi_up = readWord(ALR_HumiUpper_A);
        para.alarm_humi_low = readWord(ALR_HumiLower_A);
        para.alarm_voc = readWord(ALR_VOC_A);
        para.alarm_hcho = readWord(ALR_HCHO_A);
        para.alarm_pm25 = readWord(ALR_PM25_A);
        para.alarm_pm1 = readWord(ALR_PM1_A);
        para.alarm_pm10 = readWord(ALR_PM10_A);
        para.alarm_co2 = readWord(ALR_CO2_A);
        para.alarm_enable = readWord(ALR_EN_A);
        para.CFFlag = readWord(CFswitch_A);
        para.CAL_Temperature = readWord(CAL_TEMPERATURE_A);
        para.CAL_Humi = readWord(CAL_HUMI_A);
        para.CAL_VOC = readWord(CAL_VOC_A);
        para.CAL_HCHO = readWord(CAL_HCHO_A);
        para.updataTime = readWord(UPDATATIME_A);
        para.address_type = readWord(ADDRESSTYPE_A);
        para.address_flash = readWord(ADDRESS_A);
        */

        //para.humi=readWord(ALR_Humi_A);
        //para.temperature=readWord(ALR_Temp_A);
        para.Model=readWord(MODEL_A);
        para.Edition=readWord(Edition_A);
        para.ID_UP=readWord(IDUpper_A);
        para.ID_LOW=readWord(IDLower_A);
        para.alarm_temperature_up=readWord(alarm_temperature_up_A);
        para.alarm_temperature_up_flag=readWord(alarm_temperature_up_flag_A);
        para.alarm_temperature_low=readWord(alarm_temperature_low_A);
        para.alarm_temperature_low_flag=readWord(alarm_temperature_low_flag_A);
        para.alarm_humi_up=readWord(alarm_humi_up_A);
        para.alarm_humi_up_flag=readWord(alarm_humi_up_flag_A);
        para.alarm_humi_low=readWord(alarm_humi_low_A);
        para.alarm_humi_low_flag=readWord(alarm_humi_low_flag_A);
        para.CAL_Temperature=readWord(CAL_TEMPERATURE_A);
        para.CAL_Humi=readWord(CAL_HUMI_A);
				
				
				para.address_flash=readWord(ADDRESS_FLASH_A);//��ַ
				RS485.Address=para.address_flash;
				
				//������
				para.baud_rate=readWord(BAUD_RATE_A);
				
				if((para.baud_rate>5)||(para.baud_rate<1))
				para.baud_rate=3;
				
				switch (para.baud_rate)
        {
            case 0:
                RS485.BAUD = 1200;
                break;
            case 1:
                RS485.BAUD = 2400;
                break;
            case 2:
                RS485.BAUD = 4800;
                break;
            case 3:
                RS485.BAUD = 9600;
                break;
            case 4:
                RS485.BAUD = 19200;
                break;
            default:
                RS485.BAUD = 9600;
                break;
        }
				MX_USART1_UART_Init();
				
				
	}
	else
	{
		RecoverParam();
		Update_PARA();
	}
}

void Update_PARA(void)//��������
{
    char i;

    writeWord(0,0x5C);
    writeWord(1,~0x5C);

    /*
    writeWord(ALR_TempUpper_A,para.alarm_temperature_up);//д����  ��Ҫ��
    writeWord(ALR_TempLower_A,para.alarm_temperature_low);
    writeWord(ALR_HumiUpper_A,para.alarm_humi_up);
    writeWord(ALR_HumiLower_A,para.alarm_humi_low);
    writeWord(ALR_VOC_A,para.alarm_voc);
    writeWord(ALR_HCHO_A,para.alarm_hcho);
    writeWord(ALR_PM25_A,para.alarm_pm25);//pm2.5
    writeWord(ALR_PM1_A,para.alarm_pm1);
    writeWord(ALR_PM10_A,para.alarm_pm10);
    writeWord(ALR_CO2_A,para.alarm_co2);//co2
    writeWord(ALR_EN_A,para.alarm_enable);
    writeWord(CAL_TEMPERATURE_A,para.CAL_Temperature);//�¶�
    writeWord(CAL_HUMI_A,para.CAL_Humi);//ʪ��
    writeWord(CFswitch_A,para.CFFlag);
    writeWord(CAL_HCHO_A,para.CAL_HCHO);
    writeWord(CAL_VOC_A,para.CAL_VOC);
    writeWord(UPDATATIME_A,para.updataTime);
    writeWord(ADDRESSTYPE_A,para.address_type);
    writeWord(ADDRESS_A,para.address_flash);
    */

    //writeWord(ALR_Humi_A,para.humi);
    //writeWord(ALR_Temp_A,para.temperature);
    writeWord(MODEL_A,para.Model);
    writeWord(Edition_A,para.Edition);
    writeWord(IDUpper_A,para.ID_UP);
    writeWord(IDLower_A,para.ID_LOW);
    writeWord(alarm_temperature_up_A,para.alarm_temperature_up);
    writeWord(alarm_temperature_up_flag_A,para.alarm_temperature_up_flag);
    writeWord(alarm_temperature_low_A,para.alarm_temperature_low);
    writeWord(alarm_temperature_low_flag_A,para.alarm_temperature_low_flag);
    writeWord(alarm_humi_up_A,para.alarm_humi_up);
    writeWord(alarm_humi_up_flag_A,para.alarm_humi_up_flag);
    writeWord(alarm_humi_low_A,para.alarm_humi_low);
    writeWord(alarm_humi_low_flag_A,para.alarm_humi_low_flag);
    writeWord(CAL_HUMI_A,para.CAL_Humi);
    writeWord(CAL_TEMPERATURE_A,para.CAL_Temperature);
		
		
		//��ַ�Ͳ�����
		writeWord(ADDRESS_FLASH_A,para.address_flash);
		
		if(BAUD_RATE_A != para.baud_rate)	 
		{
			writeWord(BAUD_RATE_A,para.baud_rate);//������
			MX_USART1_UART_Init();//���¶���485uart
		}
    for (i = 0;i < 64;i++)
    {
      paraBufWrite[i] = paraBuf[i];
    }
    Flash_WritePara();//flash д����

}

void Flash_ReadPara(void)//flash������
{
    char i;
    for (i = 0;i < 64;i++)
    {
        paraBuf[i] = *(__IO uint32_t*)(START_ADDRESS + i * 4);//���ݿռ��С��4
    }
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t GetPage(uint32_t Addr)//��ȡ��ַҳ��
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}
//uint32_t GetPage(uint32_t Addr)
//{
//  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
//}

//unsigned int para2buf[64];
HAL_StatusTypeDef status = HAL_OK;
uint32_t error = 0;
HAL_StatusTypeDef Flash_WritePara(void)//д�����ݵ�
{
    char i = 0;

    FLASH_EraseInitTypeDef f;//������ʼ��

    HAL_FLASH_Unlock();//����
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.Page = GetPage(START_ADDRESS);//�����ĸ�����
    f.Banks = FLASH_BANK_SIZE;//��С
    f.NbPages = 1;//ҳ��
    i = 0;
		status = HAL_ERROR;
		//flash ���ҳ
    while(status != HAL_OK)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY | FLASH_FLAG_OPTVERR);
        status = HAL_FLASHEx_Erase(&f,&error);//����
        flash_error = error + 4000;
        if (status == HAL_OK)
        {
            //break;
        }
        else
        {
            i++;
            if (i >= 4)
                return status;
        }
        HAL_Delay(50);
    }
		//flash д��
    for (i = 0;i < 64;i+=2)
    {
				//��FLASH��־λ���
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY | FLASH_FLAG_OPTVERR);
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD , START_ADDRESS + i * 4, paraBufWrite[i] + paraBufWrite[i+1]*0x100000000);//д�����ݵ�flash
        if (status == HAL_OK)
        {
            status = HAL_OK;
            flash_error = i + 2000;
        }
        else
        {
            flash_error = i+3000;
            return status;
        }
    }
    HAL_FLASH_Lock();//����
    return status;
}

void RecoverParam(void)//����ʼֵ����
{
		para.baud_rate = 3;//������
		RS485.BAUD = 9600;
		RS485.Address=0x01;
	
    para.alarm_temperature_up = 800;
    para.alarm_temperature_low = 0;
    para.alarm_enable = 0;
    para.alarm_humi_up = 999;
    para.alarm_humi_low = 0;
    para.alarm_voc = 1626;
    para.alarm_hcho = 200;
    para.alarm_pm25 = 75;
    para.alarm_pm1 = 75;
    para.alarm_pm10 = 75;
    para.alarm_co2 = 2000;
    para.CFFlag = 0;
    para.CAL_Humi = 0;
    para.CAL_Temperature = 0;
    para.CAL_HCHO = 0;
    para.CAL_VOC = 0;

    para.updataTime = 60;

    para.address_type = 0;//��ַѡ��
    para.address_flash =0x01;//485��ַ
		para.RFS_flag=0;
		
		//��ʪ�ȱ���ʹ��
		para.alarm_temperature_up_flag=0;
		para.alarm_temperature_low_flag=0;
		para.alarm_humi_up_flag=0;
		para.alarm_humi_up_flag=0;

    para.Model=1;//�ͺ�
    para.Edition=1;
    para.ID_UP=1;
    para.ID_LOW=1;

}
uint64_t ss;
void OTG_Save(unsigned long start,unsigned char *buf,unsigned char length)
{
    unsigned char i;
    __disable_irq();
    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    for (i = 0;i < length;)
    {
        ss = (uint64_t)buf[i] + (uint64_t)buf[i+1]*0x100 + (uint64_t)buf[i+2]*0x10000 + (uint64_t)buf[i+3]*0x1000000 + (uint64_t)buf[i+4]*0x100000000 + (uint64_t)buf[i+5]*0x10000000000 + (uint64_t)buf[i+6]*0x1000000000000 +(uint64_t)buf[i+7]*0x100000000000000;
				//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR | FLASH_FLAG_BSY | FLASH_FLAG_OPTVERR);
        //status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD , start + BACKADDRESS + i, ss);//ota��������û�õ�����ע�͵�
        if (status == HAL_OK)
        {
            status = HAL_OK;
            flash_error = i + 2000;
            i+=8;
        }

        else
        {
 //         Call_Buzzer(100);
            flash_error = i+3000;
            i+=8;
        }
    }
    HAL_FLASH_Lock();
    __enable_irq();
}
