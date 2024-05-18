#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "AD5293.h"
void AD5293_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTA时钟使能 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;  // PA3 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;  // PA4 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	AD5293_CS_HIGH();
	
	
	SPI1_Init();
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);
}

void AD5293_CS_HIGH(void)
{
  GPIO_SetBits(GPIOA, GPIO_Pin_3);
}
 
void AD5293_CS_LOW(void)
{
  GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}
 
void AD5293_Unlock(){
	
	
	AD5293_CS_LOW();
 
	SPI1_ReadWriteByte(unlock>>8);
	SPI1_ReadWriteByte(unlock&0xff);
 
	AD5293_CS_HIGH();
	
}
void AD5293_Write(u8 com, u16 data){
	
	AD5293_CS_LOW();
		
	SPI1_ReadWriteByte((com<<2)|((data>>8)&0x03));
	SPI1_ReadWriteByte((data&0xff));
	
	AD5293_CS_HIGH();
}

