#ifndef __ADS1115_H
#define __ADS1115_H
#include "ADS1115_IIC.h"   												  	  


#define ADS1115_ADDRESSS 0x48	 //写地址
//#define ADS1115_Rd 0x91  //读地址


#define  ADS1115_CONVERSIONDELAY         (100)
#define  ADS1115_POINTER_MASK        (0x03)
#define  ADS1115_POINTER_CONVERT     (0x00)
#define  ADS1115_POINTER_CONFIG      (0x01)
#define  ADS1115_POINTER_LOWTHRESH   (0x02)
#define  ADS1115_POINTER_HITHRESH    (0x03)

#define  ADS1115_OS_MASK      (0x0001)    ///< Operational status/single-shot conversion start
#define  ADS1115_OS_NOEFFECT  (0x0000)    ///< Write: Bit = 0
#define  ADS1115_OS_SINGLE    (0x0001)    ///< Write: Bit = 1
#define  ADS1115_OS_BUSY      (0x0000)    ///< Read: Bit = 0
#define  ADS1115_OS_NOTBUSY   (0x0001)    ///< Read: Bit = 1

#define  ADS1115_MUX_MASK     (0x0007)    ///< Input multiplexer configuration
#define  ADS1115_MUX_1        (0x0000)    ///< P = AIN0, N = AIN1(default)
#define  ADS1115_MUX_2        (0x0001)    ///< P = AIN0, N = AIN3
#define  ADS1115_MUX_3        (0x0002)    ///< P = AIN1, N = AIN3
#define  ADS1115_MUX_4        (0x0003)    ///< P = AIN2, N = AIN3
#define  ADS1115_MUX_5        (0x0004)    ///< P = AIN0, N = GND
#define  ADS1115_MUX_6        (0x0005)    ///< P = AIN1, N = GND
#define  ADS1115_MUX_7        (0x0006)    ///< P = AIN2, N = GND
#define  ADS1115_MUX_8        (0x0007)    ///< P = AIN3, N = GND

#define  ADS1115_PGA_MASK     (0x0008)    ///< Programmable gain amplifier configuration
#define  ADS1115_PGA_0        (0x0000)    ///< 6.144V 
#define  ADS1115_PGA_1        (0x0001)    ///< 4.096V 
#define  ADS1115_PGA_2        (0x0002)    ///< 2.048V (default)
#define  ADS1115_PGA_3        (0x0003)    ///< 1.024V 
#define  ADS1115_PGA_4        (0x0004)    ///< 0.512V 
#define  ADS1115_PGA_5        (0x0005)    ///< 0.256V 

#define  ADS1115_MODE_MASK    (0x0001)    ///< Device operating mode
#define  ADS1115_MODE_CONTIN  (0x0000)    ///< Continuous conversion mode
#define  ADS1115_MODE_SINGLE  (0x0001)    ///< Power-down single-shot mode (default)

#define  ADS1115_DR_MASK      (0x0000)    ///< Data rate(samples per second)
#define  ADS1115_DR_1         (0x0000)    ///< 8 SPS
#define  ADS1115_DR_2         (0x0001)    ///< 16 SPS
#define  ADS1115_DR_3         (0x0002)    ///< 32 SPS
#define  ADS1115_DR_4         (0x0003)    ///< 64 SPS
#define  ADS1115_DR_5         (0x0004)    ///< 128 SPS (default)
#define  ADS1115_DR_6         (0x0005)    ///< 250 SPS
#define  ADS1115_DR_7         (0x0006)    ///< 475 SPS
#define  ADS1115_DR_8         (0x0007)    ///< 860 SPS

#define  ADS1115_COMP_MODE_MASK   (0x0001)    ///< Comparator mode
#define  ADS1115_COMP_MODE_TRAD   (0x0000)    ///< Traditional comparator with hysteresis (default)
#define  ADS1115_COMP_MODE_WINDOW (0x0001)    ///< Window comparator

#define  ADS1115_COMP_POL_MASK    (0x0001)    ///< Comparator polarity
#define  ADS1115_COMP_POL_ACTVLOW (0x0000)    ///< Active low(default)
#define  ADS1115_COMP_POL_ACTVHIGH  (0x0001)  ///< Active high

#define  ADS1115_COMP_LAT_MASK    (0x0001)    ///< Latching comparator
#define  ADS1115_COMP_LAT_NONLAT  (0x0000)    ///< Non-latching comparator (default)
#define  ADS1115_COMP_LAT_LATCH   (0x0001)    ///< Latching comparator

#define  ADS1115_COMP_QUE_MASK    (0x0003)    ///< Comparator queue and disable
#define  ADS1115_COMP_QUE_1CONV   (0x0000)    ///< After one conversions
#define  ADS1115_COMP_QUE_2CONV   (0x0001)    ///< After two conversions
#define  ADS1115_COMP_QUE_4CONV   (0x0002)    ///< After four conversions
#define  ADS1115_COMP_QUE_NONE    (0x0003)    ///< Disable the comparator(default)

typedef enum {
    Ads219_ok,
    Ads219_InitError,
    Ads219_WriteRegError,
    Ads219_ReadRegError,
}Ads219_Status;

typedef enum
{
    OSMODE_SINGLE       =  ADS1115_OS_SINGLE,
    OSMODE_BUSY         =  ADS1115_OS_BUSY,
    OSMODE_NOTBUSY      =  ADS1115_OS_NOTBUSY
    
} ADSOSMode_t;

typedef enum
{
    GAIN_TWOTHIRDS      =  ADS1115_PGA_0, /**< ADC Range: 0~6.144V (1 bit = 0.1875mV)*/
    GAIN_ONE            =  ADS1115_PGA_1, /**< ADC Range: 0~4.096V (1 bit = 0.125mV)*/
    GAIN_TWO            =  ADS1115_PGA_2, /**< ADC Range: 0~2.048V (1 bit = 0.0625mV)*/
    GAIN_FOUR           =  ADS1115_PGA_3, /**< ADC Range: 0~1.024V (1 bit = 0.03125mV)*/
    GAIN_EIGHT          =  ADS1115_PGA_4, /**< ADC Range: 0~0.512V (1 bit = 0.015625mV)*/
    GAIN_SIXTEEN        =  ADS1115_PGA_5  /**< ADC Range: 0~0.256V (1 bit = 0.0078125mV)*/
} ADSGain_t;

typedef enum
{
    MODE_CONTIN          =  ADS1115_MODE_CONTIN,
    MODE_SINGLE          =  ADS1115_MODE_SINGLE
} ADSMode_t;

typedef enum
{
    RATE_8              =  ADS1115_DR_1, /**< 8 SPS */
    RATE_16             =  ADS1115_DR_2, /**< 16 SPS */
    RATE_32             =  ADS1115_DR_3, /**< 32 SPS */
    RATE_64             =  ADS1115_DR_4, /**< 64 SPS */
    RATE_128            =  ADS1115_DR_5, /**< 128 SPS (default) */
    RATE_250            =  ADS1115_DR_6, /**< 250 SPS */
    RATE_475            =  ADS1115_DR_7, /**< 475 SPS */
    RATE_860            =  ADS1115_DR_8  /**< 860 SPS */
} ADSRate_t;

typedef enum
{
    COMPMODE_TRAD       =  ADS1115_COMP_MODE_TRAD,
    COMPMODE_WINDOW     =  ADS1115_COMP_MODE_WINDOW
} ADSCompMode_t;

typedef enum
{
    COMPPOL_LOW         =  ADS1115_COMP_POL_ACTVLOW,
    COMPPOL_HIGH        =  ADS1115_COMP_POL_ACTVHIGH
} ADSCompPol_t;

typedef enum
{
    COMPLAT_NONLAT      =  ADS1115_COMP_LAT_NONLAT,
    COMPLAT_LATCH       =  ADS1115_COMP_LAT_LATCH
} ADSCompLat_t;

typedef enum
{
    COMPQUE_ONE         =  ADS1115_COMP_QUE_1CONV,
    COMPQUE_TWO         =  ADS1115_COMP_QUE_2CONV,
    COMPQUE_FOUR        =  ADS1115_COMP_QUE_4CONV,
    COMPQUE_NONE        =  ADS1115_COMP_QUE_NONE
} ADSCompQue_t;

typedef enum
{
     ADSMUX_1             =  ADS1115_MUX_1,
     ADSMUX_2             =  ADS1115_MUX_2,
     ADSMUX_3             =  ADS1115_MUX_3,
     ADSMUX_4             =  ADS1115_MUX_4,
     ADSMUX_5             =  ADS1115_MUX_5,
     ADSMUX_6             =  ADS1115_MUX_6,
     ADSMUX_7             =  ADS1115_MUX_7,
     ADSMUX_8             =  ADS1115_MUX_8
} ADSMux_t;

typedef enum
{
     ADSOS_NOEFFECT       =  ADS1115_OS_NOEFFECT,
     ADSOS_SINGLE         =  ADS1115_OS_SINGLE,
     ADSOS_BUSY           =  ADS1115_OS_BUSY,
     ADSOS_NOTBUSY        =  ADS1115_OS_NOTBUSY
} ADSOs_t;

/**********************************
函数声明
**********************************/
//uint8_t  ConfigeRegister(unsigned char channel);
void ADS1115_SetGain(uint8_t value);
uint16_t ADS1115_ReadVoltage(uint8_t channel);
#endif




































