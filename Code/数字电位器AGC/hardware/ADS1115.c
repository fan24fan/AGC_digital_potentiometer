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
#include "ADS1115.h"
#include "sys.h"
#include "delay.h"
 

#include "ADS1115.h"
#include "Delay.h"

float coefficient = 0.0625;
  /**
   * @fn setGain
   * @brief Sets the gain and input voltage range.This configures the programmable gain amplifier
   * @param value  The enum variable struct of  eADSGain_t
   * @n GAIN_TWOTHIRDS      ADC Range: 0~6.144V (1 bit = 0.1875mV)
   * @n GAIN_ONE            ADC Range: 0~4.096V (1 bit = 0.125mV)
   * @n GAIN_TWO            ADC Range: 0~2.048V (1 bit = 0.0625mV)
   * @n GAIN_FOUR           ADC Range: 0~1.024V (1 bit = 0.03125mV)
   * @n GAIN_EIGHT          ADC Range: 0~0.512V (1 bit = 0.015625mV)
   * @n GAIN_SIXTEEN        ADC Range: 0~0.256V (1 bit = 0.0078125mV)
   */
void ADS1115_SetGain(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS, ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x07 << 9);
    conf |= (uint16_t) value << 9;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS, ADS1115_POINTER_CONFIG, conf);
    switch(value){
        case GAIN_TWO:
            coefficient = 0.0625;
            break;
        case GAIN_TWOTHIRDS:
            coefficient = 0.1875;
            break;
        case GAIN_ONE:
            coefficient = 0.125;
            break;
        case GAIN_FOUR:
            coefficient = 0.03125;
            break;
        case GAIN_EIGHT:
            coefficient = 0.015625;
            break;
        case GAIN_SIXTEEN:
            coefficient = 0.0078125;
            break;
        default:
            coefficient = 0.0625;
            break;
    }
}
  /**
   * @fn setMode
   * @brief Sets the Device operating mode. This controls the current operational mode of the ADS1115
   * @param value  The enum variable struct of  eADSMode_t 
   * @n MODE_CONTIN
   * @n MODE_SINGLE  
   */
void ADS1115_SetMode(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x01 << 8);
    conf |= (uint16_t) value << 8;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}
  /**
   * @fn setRate
   * @brief Sets the Date Rate. This controls the data rate setting.
   * @param value The enum variable struct of  eADSRate_t
   * @n RATE_8     8 SPS
   * @n RATE_16    16 SPS
   * @n RATE_32    32 SPS
   * @n RATE_64    64 SPS
   * @n RATE_128   128 SPS (default) 
   * @n RATE_250   250 SPS
   * @n RATE_475   475 SPS
   * @n RATE_860   860 SPS
   */
void ADS1115_SetRate(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x07 << 5);
    conf |= (uint16_t) value << 5;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}
  /**
   * @fn setOSMode
   * @brief Sets the Operational status/single-shot conversion to start.
   * @n This determines the operational status of the device.
   * @param value  The enum variable struct of  eADSOSMode_t   
   * @n OSMODE_SINGLE
   * @n OSMODE_BUSY
   * @n OSMODE_NOTBUSY
   */
void ADS1115_SetOSMode(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x01 << 15);
    conf |= (uint16_t) value << 15;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}
  /**
   * @fn setCompQue
   * @brief  Sets the Comparator queue and disable .
   * @n This perform two functions. 
   * @n It can disable the comparator function and put the ALERT/RDY pin into a high state. 
   * @n It also can control the number of successive conversions exceeding the upper
   * @n or lower thresholds required before asserting the ALERT/RDY pin.
   * @param value  The enum variable struct of  eADSCompLat_t
   * @n COMPQUE_ONE
   * @n COMPQUE_TWO
   * @n COMPQUE_FOUR
   * @n COMPQUE_NONE
   */
void ADS1115_SetCompQue(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x03);
    conf |= (uint16_t) value;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}
  /**
   * @fn setCompLat
   * @brief  Sets the Latching comparator .
   * @n This controls whether the ALERT/RDY pin latches once asserted 
   * @n or clears when conversions are within the margin of the upper 
   * @n and lower threshold values.
   * @param value  The enum variable struct of  eADSCompLat_t
   * @n COMPLAT_NONLAT
   * @n COMPLAT_LATCH  
   */
void ADS1115_SetCompLat(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x01 << 2);
    conf |= (uint16_t) value << 2;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}
  /**
   * @fn setCompMode
   * @brief  Sets the Comparator polarity. This controls the polarity of the ALERT/RDY pin
   * @param value  The enum variable struct of  eADSCompPol_t
   * @n COMPPOL_LOW
   * @n COMPPOL_HIGH  
   */
void DADS1115_SetCompPol(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x01 << 3);
    conf |= (uint16_t) value << 3;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}
  /**
   * @fn setCompMode
   * @brief Sets the Comparator mode.This controls the comparator mode of operation.
   * @param value  The enum variable struct of  eADSCompMode_t
   * @n COMPMODE_TRAD
   * @n COMPMODE_WINDOW  
   */
void ADS1115_SetCompMode(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x01 << 4);
    conf |= (uint16_t) value << 4;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}

void ADS1115_SetMux(uint8_t value)
{
    int16_t    conf;
    conf = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    conf &= ~((uint16_t) 0x07 << 12);
    conf |= (uint16_t) value << 12;
    ADS1115_WriteAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG, conf);
}

/*
 *Reads the conversion results, measuring the voltage
 *for a single-ended ADC reading from the specified channel
 *Negative voltages cannot be applied to this circuit because the
 *ADS1115 can only accept positive voltages
 */
uint16_t ADS1115_ReadVoltage(uint8_t channel)
{
		uint16_t config=0;
    int16_t Voltage = 0;
    if (channel > 3)
        return 0;
    // Start with default config values
    ADS1115_SetCompQue(COMPQUE_NONE);
    ADS1115_SetCompLat(COMPLAT_NONLAT);
    DADS1115_SetCompPol(COMPPOL_LOW);
    ADS1115_SetCompMode(COMPMODE_TRAD);
    if (channel == 0)
        ADS1115_SetMux(ADSMUX_5);
    else if (channel == 1)
        ADS1115_SetMux(ADSMUX_6);
    else if (channel == 2)
        ADS1115_SetMux(ADSMUX_7);
    else if (channel == 3)
        ADS1115_SetMux(ADSMUX_8);
    // Wait for the conversion to complete

    config = ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONFIG);
    DelayMs(10);
    // Read the conversion results
    // 16-bit unsigned results for the ADS1115

    Voltage = (int16_t)ADS1115_ReadAdsReg(ADS1115_ADDRESSS,  ADS1115_POINTER_CONVERT)*coefficient;

    return Voltage;
}































