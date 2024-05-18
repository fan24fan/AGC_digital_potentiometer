/************************************************************************************
//注意：K系列模块驱动程序是免费资源，任何收费行为都是盗用资料行为
//店名：(咱们电子科技）淘宝链接: https://shop149406681.taobao.com/?spm=a1z10.1-c.0.0.64cb457fKzgKea 
//淘宝店内搜索关键词：ADS1115
//版 本 号   : v2.0
//作    者   : ZMD_001
//咱们电子科技QQ :1795190213   微信：15770663680
//CSDN  （基本参数，资源更新列表，所有接线说明）   :单片机代码搬运工(搜索关键词：K_A37_005)
//BILIBILI（演示视频） :咱们电子科技(搜索关键词：K_A37_005)
//生成日期   : 2023-6-21
//最近修改   : 2023-11-21
//下载器 ：STLINK 链接：https://item.taobao.com/item.htm?ft=t&id=740772318848
//  功能描述   : ADS1115块测试程序
//  测试条件   : STM32F103C8T6   晶振8M  系统时钟72M
接线
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

  //ADS1115 IIC 延时函数
void ADS1115_IIC_Delay(void)
{
	DelayUs(10);
}

//初始化IIC
void ADS1115_IIC_Init(void)
{			
	
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB 
}
//产生IIC起始信号
void ADS1115_IIC_Start(void)
{
	ADS1115_SDA_OUT();     //sda线输出
	ADS1115_IIC_SDA=1;	  	  
	ADS1115_IIC_SCL=1;
	ADS1115_IIC_Delay();
 	ADS1115_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	ADS1115_IIC_Delay();
}	  
//产生IIC停止信号
void ADS1115_IIC_Stop(void)
{
	ADS1115_SDA_OUT();//sda线输出
	ADS1115_IIC_SDA=0;//
	ADS1115_IIC_SCL=1;
 	ADS1115_IIC_Delay();
  
	ADS1115_IIC_SDA=1;//发送I2C总线结束信号
	ADS1115_IIC_Delay();							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t ADS1115_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	ADS1115_SDA_IN();      //SDA设置为输入  
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
	ADS1115_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void ADS1115_IIC_Ack(void)
{

	ADS1115_SDA_OUT();
	ADS1115_IIC_SDA=0;
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=1;
	ADS1115_IIC_Delay();
	ADS1115_IIC_SCL=0;
}
//不产生ACK应答		    
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
//向IIC总线写入一个字节的数据
*****************************************************************/

void ADS1115_IIC_write_byte(uint8_t Data)
{

		uint8_t i;
		ADS1115_SDA_OUT();
		for(i=0;i<8;i++)//八位数据
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
				ADS1115_IIC_SCL = 1; //一个时钟信号送入数据
				ADS1115_IIC_Delay();
				ADS1115_IIC_SCL = 0;
				ADS1115_IIC_Delay();
				Data = Data << 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
		}
		
		ADS1115_IIC_SDA = 0;		//
		ADS1115_IIC_Delay();
}
/****************************************************************
//从IIC总线读取一个字节的数据函数
*****************************************************************/

uint8_t ADS1115_IIC_read_byte(void)
{

		uint8_t i;
		uint8_t Data = 0;       //定义一个缓冲寄存器。
		ADS1115_SDA_OUT();
		ADS1115_IIC_SCL = 0;	//先拉低时钟线
		ADS1115_IIC_SDA = 1;	//再拉高数据线
		ADS1115_IIC_Delay();
		ADS1115_SDA_IN();//SDA设置为输入
		for(i = 0;i < 8;i++)
		{
				Data = Data<<1;		//将缓冲字节的数据左移一位，准备读取数据。
				ADS1115_IIC_SCL = 1;				//拉高时钟线，开始读取下一位数据
				if(ADS1115_READ_SDA)		//如果数据线为高平电平。
				{
						Data = Data|0x01;	//则给缓冲字节的最低位写1。
				}
				ADS1115_IIC_Delay();
				ADS1115_IIC_SCL = 0;				//拉低时钟线，一位读取完成
				ADS1115_IIC_Delay();
		}
		ADS1115_IIC_SDA = 0;		//
		ADS1115_IIC_Delay();
		return Data;					//返回读取的一个字节数据。



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

















