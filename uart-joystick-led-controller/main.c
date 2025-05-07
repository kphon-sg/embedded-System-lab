#include "cmsis_os.h"                     // ARM::CMSIS:RTOS:Keil RTX
#include "LPC17xx.h"                      // Device header
#include "Uart.h"
#include "Joystick.h"
#include "Led.h"
#include "ADC.h"

char* UintToChar(int32_t value)
{
	int i;
	static char buffer[10] = {0};
	for(i=9;i>=0;i--)
	{
		buffer[i]="0123456789"[value % 10];
		i--;
		value/=10;
	}
	return buffer;
}

void Send_ADC_Value()
{
	int ADC_Value;
	ADC_StartConversion();
	while(ADC_ConversionDone()==0);
	ADC_Value = ADC_GetValue();
	UART_Send("\nADC Value = ", 13);
	UART_Send(UintToChar(ADC_Value), 10);
	osDelay(1000);
}

void Send_Data_Base_On_Joystick()
{
	Joystick_Status_Enum joystick_Status = Joystick_Get_Status();
	switch(joystick_Status)
	{
		case JOYSTICK_UP:
			UART_Send("\nInternational University",25);
			osDelay(1000);
			break;
		case JOYSTICK_DOWN:
			UART_Send("\nElectrical Engineering",23);
			osDelay(1000);
			break;
		case JOYSTICK_LEFT:
			UART_Send("\nEmbedded System",16);
			osDelay(1000);
			break;
		case JOYSTICK_RIGHT:
			UART_Send("\nStudent Name",13);
			osDelay(1000);
			break;
		default: break;
	}
}

void Turn_Leds_On_Off_Base_On_Uart_Receiving_Value()
{
	if(UART_Buffer_Count>0)
	{
		int i;
		for(i=0; i<UART_Buffer_Count; i++)
		{
			char functionIntention = UART_Buffer[i] >> 4;
			char functionTarget = UART_Buffer[i] & 0x0F;
			if(functionTarget < 8)
			{
				switch(functionIntention)
				{
					case 1: Led_On(functionTarget); break;
					case 2: Led_Off(functionTarget); break;
				}
			}
		}
		UART_Buffer_Count = 0;
	}
}

int main(void)
{
  UART_Initialize(19200);
	Joystick_Initialize();
	Led_Initialize();
	ADC_Initialize();
  while(1)
  {
		Send_ADC_Value();
		Send_Data_Base_On_Joystick();
		Turn_Leds_On_Off_Base_On_Uart_Receiving_Value();
  }
}
