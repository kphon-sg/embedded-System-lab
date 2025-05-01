#include "LPC17xx.h"                      // Device header
#include "uart.h"


int main(void)
{
  UART_Initialize(9600);
  Led_Initialize();

  while(1)
  {
		if(UART_Buffer_Count>0)
		{
			if(UART_Buffer[UART_Buffer_Count-1] == '1')
			{
				Leds_Set_Value(0xFF);
				uart_TxChar('G');
				UART_Send("\nLEDs are turned on",19);
				
			}
			else if(UART_Buffer[UART_Buffer_Count-1] == '0')
			{
				Leds_Set_Value(0x00);
				uart_TxChar('B');
				UART_Send("\nLEDs are turned off",20);
			}
			UART_Buffer_Count = 0;
		}
  }
}