/************************************************************************************
//ע�⣺Kϵ��ģ�����������������Դ���κ��շ���Ϊ���ǵ���������Ϊ
//������(���ǵ��ӿƼ����Ա�����: https://shop149406681.taobao.com/?spm=a1z10.1-c.0.0.64cb457fKzgKea 
//�Ա����������ؼ��ʣ�ADS1115
//�� �� ��   : v2.0
//��    ��   : ZMD_001
//���ǵ��ӿƼ�QQ :1795190213   ΢�ţ�15770663680
//CSDN  ��������������Դ�����б����н���˵����   :��Ƭ��������˹�(�����ؼ��ʣ�K_A37_005)
//BILIBILI����ʾ��Ƶ�� :���ǵ��ӿƼ�(�����ؼ��ʣ�K_A37_005)
//��������   : 2023-6-21
//����޸�   : 2023-11-21
//������ ��STLINK ���ӣ�https://item.taobao.com/item.htm?ft=t&id=740772318848
//  ��������   : ADS1115����Գ���
//  ��������   : STM32F103C8T6   ����8M  ϵͳʱ��72M
����
ADS1115--------------------------------STM32F103C8T6
VCC-- ---------------------------------5V
GND- ----------------------------------GND
SCL- ----------------------------------PB_4 //SCL
SDA- ----------------------------------PB_3 //SDA	

OLED0.96
VCC-- ---------------------------------3.3V
GND- ----------------------------------GND
SCL- ----------------------------------PB_8 //SCL
SDA- ----------------------------------PB_9 //SDA	
*************************************************************************************/
#include "ADS1115_IIC.h"
#include "delay.h"

  //ADS1115 IIC ��ʱ����
void ADS1115_IIC_Delay(void)
{
	DelayUs(10);
}

//��ʼ��IIC
void ADS1115_IIC_Init(void)
{			
	
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB 
}
//����IIC��ʼ�ź�
void ADS1115_IIC_Start(void)
{
	ADS1115_SDA_OUT();     //sda�����
	ADS1115_IIC_SDA=1;	  	  
	ADS1115_IIC_SCL=1;
	ADS1115_IIC_Delay();
 	ADS1115_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	ADS1115_IIC_Delay();
}	  
//����IICֹͣ�ź�
void ADS1115_IIC_Stop(void)
{
	ADS1115_SDA_OUT();//sda�����
	ADS1115_IIC_SDA=0;//
	ADS1115_IIC_SCL=1;
 	ADS1115_IIC_Delay();
  
	ADS1115_IIC_SDA=1;//����I2C���߽����ź�
	ADS1115_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t ADS1115_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	ADS1115_SDA_IN();      //SDA����Ϊ����  
	ADS1115_IIC_SCL=0;
	ADS1115_IIC_Delay();	   
	ADS1115_IIC_SCL=1;
	ADS1115_IIC_Delay();	 
	while(ADS1115_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			ADS1115_IIC_Stop();
			return 1;
		}
	}
	ADS1115_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void ADS1115_IIC_Ack(void)
{

	ADS1115_SDA_OUT();
	ADS1115_IIC_SDA=0;
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=1;
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=0;
}
//������ACKӦ��		    
void ADS1115_IIC_NAck(void)
{

	ADS1115_SDA_OUT();
	ADS1115_IIC_SDA=1;
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=1;
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=0;
}					 				     


/****************************************************************
//��IIC����д��һ���ֽڵ�����
*****************************************************************/

void ADS1115_IIC_write_byte(uint8_t Data)
{

		uint8_t i;
		ADS1115_SDA_OUT();
		for(i=0;i<8;i++)//��λ����
		{
				if((Data & 0x80) == 0x80)
				{
						ADS1115_IIC_SDA = 1;
				}
				else
				{
						ADS1115_IIC_SDA = 0;
				}
				ADS1115_IIC_Delay();
				ADS1115_IIC_SCL = 1; //һ��ʱ���ź���������
				ADS1115_IIC_Delay();
				ADS1115_IIC_SCL = 0;
				ADS1115_IIC_Delay();
				Data = Data << 1;//��������һλ���Ѵθ�λ�������λ,Ϊд��θ�λ��׼��
		}
		
		ADS1115_IIC_SDA = 0;		//
		ADS1115_IIC_Delay();
}
/****************************************************************
//��IIC���߶�ȡһ���ֽڵ����ݺ���
*****************************************************************/

uint8_t ADS1115_IIC_read_byte(void)
{

		uint8_t i;
		uint8_t Data = 0;       //����һ������Ĵ�����
		ADS1115_SDA_OUT();
		ADS1115_IIC_SCL = 0;	//������ʱ����
		ADS1115_IIC_SDA = 1;	//������������
		ADS1115_IIC_Delay();
		ADS1115_SDA_IN();//SDA����Ϊ����
		for(i = 0;i < 8;i++)
		{
				Data = Data<<1;		//�������ֽڵ���������һλ��׼����ȡ���ݡ�
				ADS1115_IIC_SCL = 1;				//����ʱ���ߣ���ʼ��ȡ��һλ����
				if(ADS1115_READ_SDA)		//���������Ϊ��ƽ��ƽ��
				{
						Data = Data|0x01;	//��������ֽڵ����λд1��
				}
				ADS1115_IIC_Delay();
				ADS1115_IIC_SCL = 0;				//����ʱ���ߣ�һλ��ȡ���
				ADS1115_IIC_Delay();
		}
		ADS1115_IIC_SDA = 0;		//
		ADS1115_IIC_Delay();
		return Data;					//���ض�ȡ��һ���ֽ����ݡ�



}


/***************************************************************************************************************
* I2C write data
****************************************************************************************************************/
void IIC_WriteReg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t reg_dat)
{
    ADS1115_IIC_Start();
    ADS1115_IIC_write_byte((i2c_addr<<1)|0x00);
    ADS1115_IIC_Wait_Ack(); 
    ADS1115_IIC_write_byte(reg_addr);
    ADS1115_IIC_Wait_Ack();
    ADS1115_IIC_write_byte(reg_dat);
    ADS1115_IIC_Wait_Ack();
    ADS1115_IIC_Stop();
}
/***************************************************************************************************************
* I2C write data
****************************************************************************************************************/

void IIC_Write_addr(uint8_t i2c_addr, uint8_t reg_addr)
{
    ADS1115_IIC_Start();
    ADS1115_IIC_write_byte((i2c_addr<<1)|0x00);
    ADS1115_IIC_Wait_Ack(); 
    ADS1115_IIC_write_byte(reg_addr);
    ADS1115_IIC_Wait_Ack();
    
    ADS1115_IIC_Stop();
}
/***************************************************************************************************************
* I2C write data
****************************************************************************************************************/

void IIC_WriteBuf(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *Pdat, uint8_t len)
{
	uint8_t i;
    ADS1115_IIC_Start();
    ADS1115_IIC_write_byte((i2c_addr<<1)|0x00);
    ADS1115_IIC_Wait_Ack(); 
	
	  ADS1115_IIC_write_byte(reg_addr);
    ADS1115_IIC_Wait_Ack();
	
		for(i=0;i<len;i++)
		{
				ADS1115_IIC_write_byte(*(Pdat+i));
				ADS1115_IIC_Wait_Ack();
			
		}
    ADS1115_IIC_Stop();
}
/***************************************************************************************************************
*receive one bytes to 'pdata'
****************************************************************************************************************/

void IIC_ReadData(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *pdat, uint8_t len)
{
		uint8_t j;
    ADS1115_IIC_Start();
    ADS1115_IIC_write_byte((i2c_addr<<1)|0x00);
    ADS1115_IIC_Wait_Ack();
    ADS1115_IIC_write_byte(reg_addr);
    ADS1115_IIC_Wait_Ack();
    ADS1115_IIC_Start();
    ADS1115_IIC_write_byte((i2c_addr<<1)|0x01);
    ADS1115_IIC_Wait_Ack();
	
	    for(j = 0; j <len; j++)
    {
				*pdat=ADS1115_IIC_read_byte();
        if(len==(j+1))
             ADS1115_IIC_NAck();
        else
           
						ADS1115_IIC_Ack();
        pdat++;
    }

    ADS1115_IIC_Stop();
}


int16_t ADS1115_ReadAdsReg(uint8_t i2cAddress, uint8_t reg)
{
    uint8_t buf[2] = {0};
    
    IIC_ReadData(i2cAddress, reg, buf, sizeof(buf));
    return (buf[0] << 8) | buf[1];
}

void ADS1115_WriteAdsReg(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
		uint8_t buffer[2]={0};
     buffer[0] = (value >> 8); 
		 buffer[1] = value & 0xff;
    IIC_WriteBuf(i2cAddress, reg, buffer, 2);
}

















