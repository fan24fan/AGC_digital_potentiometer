#ifndef __AD5293_H
#define __AD5293_H
 
#define AD5293_RDY PAin(4) 
 
#define unlock 0x1802
#define nop 0x00
#define write 0x01
#define reset 0x04
#define read_data 0x02
#define read_com 0x07
 
 
void AD5293_CS_HIGH(void);
void AD5293_CS_LOW(void);
 
void AD5293_Init(void);
void AD5293_Unlock(void);
void AD5293_Write(u8 com, u16 data);
u16 AD5293_Read_data(u8 com);
 
#endif
