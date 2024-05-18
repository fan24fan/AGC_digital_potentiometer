#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "AD5293.h"
#include "delay.h"
#include "ADS1115.h"
#include "bsp_usart.h"
#include "oled.h"
#include "MyExti.h"
#include "Key.h"
int16_t ADS1115_ADC0, ADS1115_ADC1, ADS1115_ADC2, ADS1115_ADC3;
uint8_t ADC0_Buf[]={"ADC0:       V"};
uint8_t ADC1_Buf[]={"ADC1:       V"};
uint8_t ADC2_Buf[]={"ADC2:       V"};
uint8_t ADC3_Buf[]={"ADC3:       V"};
int i =0x0067;
int	flag =1;
int gear =3;
int ad_flag =1;
int flag_auto =0;
float ADS1115_ADC01;
float ADS1115_ADC11;
void just_vol(float vol_ref,float vol,float output_singal);
void gear_position(int m);
void Dis_Dat(void);
void OLED_printf(unsigned char *s,int temp_data);

void OLED_printf(unsigned char *s,int temp_data)
{
	s=s+5;
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';
//	*++s =temp_data/10000%10+0x30;      //千位
	*++s =temp_data/1000%10+0x30;      //千位
	*++s ='.'; 	
	*++s =temp_data/100%10+0x30;  //百位
	*++s =temp_data/10%10+0x30;    //十位
	*++s =temp_data%10+0x30; 			  //个位
//	*++s ='\0'; 

}
void Dis_Dat(void)
{
		if(ad_flag == 0)
		{
			ADS1115_ADC0 = ADS1115_ReadVoltage(0);
				OLED_printf(ADC0_Buf,ADS1115_ADC0);
				Usart_SendString( DEBUG_USARTx,ADC0_Buf);
				OLED_ShowString(0,0,ADC0_Buf,16);
		
			ADS1115_ADC1 = ADS1115_ReadVoltage(1);
				OLED_printf(ADC1_Buf, ADS1115_ADC1);
				Usart_SendString( DEBUG_USARTx,ADC1_Buf);
				OLED_ShowString(0,2,ADC1_Buf,16);
		}
		else
		{
			ADS1115_ADC0 = ADS1115_ReadVoltage(0);
			ADS1115_ADC01 =(float)((ADS1115_ADC0*3.1415)/2000.0);
				OLED_printf(ADC0_Buf,ADS1115_ADC0);
				Usart_SendString( DEBUG_USARTx,ADC0_Buf);
				//OLED_ShowString(0,0,ADC0_Buf,16);
				OLED_ShowString(0,0,"ADC0:",16);
				OLED_Float(0,50,ADS1115_ADC01,4);
		
			ADS1115_ADC1 = ADS1115_ReadVoltage(1);
			ADS1115_ADC11 =(float)((ADS1115_ADC1*3.1415)/2000.0);
				OLED_printf(ADC1_Buf, ADS1115_ADC1);
				Usart_SendString( DEBUG_USARTx,ADC1_Buf);
				//OLED_ShowString(0,2,ADC1_Buf,16);
				OLED_ShowString(0,2,"ADC1:",16);
				OLED_Float(2,50,ADS1115_ADC11,4);
		}
        
//		
//        ADS1115_ADC2 = ADS1115_ReadVoltage(2);
//				OLED_printf(ADC2_Buf, ADS1115_ADC2);
//				Usart_SendString( DEBUG_USARTx,ADC2_Buf);
//				OLED_ShowString(0,4,ADC2_Buf,16);
//		
//        ADS1115_ADC3 = ADS1115_ReadVoltage(3);
//				OLED_printf(ADC3_Buf, ADS1115_ADC3);
//				Usart_SendString( DEBUG_USARTx,ADC3_Buf);
//				OLED_ShowString(0,6,ADC3_Buf,16);
	
}

int main(void)
 {	
	float input_singal;
	float output_singal;
	float input_original;
	float output_original;
	float amp;
	int k =0;
    AD5293_Init();
	AD5293_Unlock();
	AD5293_Write(write,0x00200);
	
	DelayInit();
	OLED_Init();
	OLED_Clear();
	USART_Config();
	ADS1115_IIC_Init();
	ADS1115_SetGain(GAIN_TWOTHIRDS);
	Key_Init();
	while(1)
	{
		Dis_Dat();
		DelayMs(300);
		input_original =ADS1115_ADC0/1000.0;
		output_original =ADS1115_ADC1/1000.0;
		
		switch(Key_GetNum())
		{
			case 0:
				break;
			
			case 1:
				ad_flag =~ad_flag;
				break;
			
			case 2:
				flag ++;
				if(flag>3)
				{
					flag =1;
				}
				break;
		}
		if(ad_flag == 1)
		{
			output_singal =(output_original*3.1415)/2.0;
			input_singal =(input_original*3.1415)/2.0;
		}
		else
		{
			output_singal = output_original;
			input_singal  = input_original;
		}
		if(flag_auto == 0)
		{
		switch(flag)
		{
			case 1://输入0.1～5V，输出0.5～2V
				if(output_singal>2.0)
				{
					gear_position(gear --);
					if(gear<0)
					{
						gear =0;
					}
				}
				else if(output_singal<0.5)
				{
					gear_position(gear ++);
					if(gear>3)
					{
						gear =3;
					}
				}
				break;
			
			case 2://输入0.1～5V，输出0.8～1.2V
				just_vol(1.0,0.2,output_singal);break;
				
			case 3://输入0.05～5V，输出0.4～0.6V
				just_vol(0.5,0.1,output_singal);break;
		}
	}
	else
	{
		if(Key_GetNum() == 3)
		{
			gear_position(k++);
			if(gear>3)
			{
				gear =3;
			}
		}
	}
		amp =(float)((output_original)/(input_original));
		OLED_ShowString(0,4,"amp:",16);
		OLED_Float(4,30,amp,4);
	}
}
void just_vol(float vol_ref,float vol,float output_singal)
{
	if((output_singal >vol_ref)&&((output_singal-vol_ref)>vol))
{
    	AD5293_Write(write,i --);
    	if(i<=0x0001)
    	{
    		i = 0x0002;
    	}
    }
    else if((output_singal <vol_ref)&&((vol_ref - output_singal)>vol))
    {
    	AD5293_Write(write,i ++);
    	if(i>= 0x3FFF)
    	{
    		i = 0x3FFE;
    	}
    }
 }

 //50K版本，接地电阻5K
 //100K版本，接地电阻10K
void gear_position(int m)
{
	switch(m)
	{
		case 0:
			AD5293_Write(write,0x0014);break;//0.2倍
		
		case 1:
			AD5293_Write(write,0x0033);break;//0.5倍
			
		case 2:
			AD5293_Write(write,0x00CD);break;//2倍
		
		case 3:
			AD5293_Write(write,0x0200);break;//5倍
		
		default:
			AD5293_Write(write,0x00CD);break;//2倍
	}
}


//KEY1按键检测,区分交直流输入
//void EXTI3_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line3) == SET)
//	{
//		
//		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0)
//		{
//			ad_flag= ~ad_flag;
//		}
//		EXTI_ClearITPendingBit(EXTI_Line3);
//	}
//}

////KEY2按键检测，自动增益控制范围的标志信号
//void EXTI4_IRQHandler(void)
//{
//	if (EXTI_GetITStatus(EXTI_Line4) == SET)
//	{
//		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
//		{
//			flag ++;
//			if(flag>3)
//			{
//				flag =1;
//			}
//		}
//		EXTI_ClearITPendingBit(EXTI_Line4);
//	}
//}



