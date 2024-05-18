#include "stm32f10x.h"
#include "MySPI.h"
#include "AD5293.h"
#include "delay.h"
#include "ADS1115.h"
#include "bsp_usart.h"
#include "oled.h"
#include "MyExti.h"
#include "Key.h"

int16_t ADS1115_ADC0, ADS1115_ADC1;
uint8_t ADC0_Buf[] = "ADC0:       V";
uint8_t ADC1_Buf[] = "ADC1:       V";
int i = 0x0067;
int flag = 1;
int gear = 3;
int ad_flag = 1;
float ADS1115_ADC01;
float ADS1115_ADC11;

void just_vol(float vol_ref, float vol, float output_signal);
void gear_position(int m);
void Dis_Dat(void);
void OLED_printf(unsigned char *s, int temp_data);

void OLED_printf(unsigned char *s, int temp_data)
{
    s += 5;
    *s = (temp_data < 0) ? '-' : ' ';
    temp_data = abs(temp_data);
    *(s + 1) = temp_data / 1000 % 10 + 0x30;
    *(s + 2) = '.';
    *(s + 3) = temp_data / 100 % 10 + 0x30;
    *(s + 4) = temp_data / 10 % 10 + 0x30;
    *(s + 5) = temp_data % 10 + 0x30;
}

void Dis_Dat(void)
{
    if (ad_flag == 0)
    {
        ADS1115_ADC0 = ADS1115_ReadVoltage(0);
        OLED_printf(ADC0_Buf, ADS1115_ADC0);
        Usart_SendString(DEBUG_USARTx, ADC0_Buf);
        OLED_ShowString(0, 0, ADC0_Buf, 16);

        ADS1115_ADC1 = ADS1115_ReadVoltage(1);
        OLED_printf(ADC1_Buf, ADS1115_ADC1);
        Usart_SendString(DEBUG_USARTx, ADC1_Buf);
        OLED_ShowString(0, 2, ADC1_Buf, 16);
    }
    else
    {
        ADS1115_ADC0 = ADS1115_ReadVoltage(0);
        ADS1115_ADC01 = (float)((ADS1115_ADC0 * 3.1415) / 2000.0);
        OLED_printf(ADC0_Buf, ADS1115_ADC0);
        Usart_SendString(DEBUG_USARTx, ADC0_Buf);
        OLED_ShowString(0, 0, "ADC0:", 16);
        OLED_Float(0, 50, ADS1115_ADC01, 4);

        ADS1115_ADC1 = ADS1115_ReadVoltage(1);
        ADS1115_ADC11 = (float)((ADS1115_ADC1 * 3.1415) / 2000.0);
        OLED_printf(ADC1_Buf, ADS1115_ADC1);
        Usart_SendString(DEBUG_USARTx, ADC1_Buf);
        OLED_ShowString(0, 2, "ADC1:", 16);
        OLED_Float(2, 50, ADS1115_ADC11, 4);
    }
}

int main(void)
{
    float input_signal, output_signal, input_original, output_original, amp;
    AD5293_Init();
    AD5293_Unlock();
    AD5293_Write(write, 0x00200);

    DelayInit();
    OLED_Init();
    OLED_Clear();
    USART_Config();
    ADS1115_IIC_Init();
    ADS1115_SetGain(GAIN_TWOTHIRDS);
    Key_Init();

    while (1)
    {
        Dis_Dat();
        DelayMs(300);
        input_original = ADS1115_ADC0 / 1000.0;
        output_original = ADS1115_ADC1 / 1000.0;

        switch (Key_GetNum())
        {
            case 0: break;
            case 1: ad_flag = ~ad_flag; break;
            case 2:
                flag++;
                if (flag > 3) flag = 1;
                break;
        }

        output_signal = (ad_flag == 1) ? (output_original * 3.1415) / 2.0 : output_original;
        input_signal = (ad_flag == 1) ? (input_original * 3.1415) / 2.0 : input_original;

        if (flag_auto == 0)
        {
            switch (flag)
            {
                case 1:
                    if (output_signal > 2.0) gear_position(--gear);
                    else if (output_signal < 0.5) gear_position(++gear);
                    break;
                case 2:
                    just_vol(1.0, 0.2, output_signal);
                    break;
                case 3:
                    just_vol(0.5, 0.1, output_signal);
                    break;
            }
        }
        else if (Key_GetNum() == 3)
        {
            gear_position(k++);
        }

        amp = output_original / input_original;
        OLED_ShowString(0, 4, "amp:", 16);
        OLED_Float(4, 30, amp, 4);
    }
}

void just_vol(float vol_ref, float vol, float output_signal)
{
    if ((output_signal > vol_ref) && ((output_signal - vol_ref) > vol))
    {
        AD5293_Write(write, i--);
        if (i <= 0x0001) i = 0x0002;
    }
    else if ((output_signal < vol_ref) && ((vol_ref - output_signal) > vol))
    {
        AD5293_Write(write, i++);
        if (i >= 0x3FFF) i = 0x3FFE;
    }
}

void gear_position(int m)
{
    switch (m)
    {
        case 0: AD5293_Write(write, 0x0014); break;
        case 1: AD5293_Write(write, 0x0033); break;
        case 2: AD5293_Write(write, 0x00CD); break;
        case 3: AD5293_Write(write, 0x0200); break;
        default: AD5293_Write(write, 0x00CD); break;
    }
}
\end{lstlisting}