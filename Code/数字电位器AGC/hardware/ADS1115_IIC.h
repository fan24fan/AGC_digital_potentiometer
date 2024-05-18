#ifndef __ADS1115_IIC_H
#define __ADS1115_IIC_H
#include "sys.h"
	   		    
//IO方向设置
#define ADS1115_SDA_IN()   {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=8<<12;}
#define ADS1115_SDA_OUT()  {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=3<<12;}

//IO操作函数	 
#define ADS1115_IIC_SCL     PBout(4) //SCL
#define ADS1115_IIC_SDA     PBout(3) //SDA	
#define ADS1115_READ_SDA   	PBin(3) //SDA	

void ADS1115_IIC_Init(void);
//IIC所有操作函数
//uint8_t writeRegister(uint8_t ads1115_I2CAddress, uint8_t reg, uint16_t value);
void IIC_WriteReg(uint8_t i2c_addr, uint8_t reg_addr, uint8_t reg_dat);
void IIC_Write_addr(uint8_t i2c_addr, uint8_t reg_addr);
void IIC_WriteBuf(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *Pdat, uint8_t len);
void IIC_ReadData(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *pdat, uint8_t len);
int16_t ADS1115_ReadAdsReg(uint8_t i2cAddress, uint8_t reg);
void ADS1115_WriteAdsReg(uint8_t i2cAddress, uint8_t reg, uint16_t value);  
#endif
















