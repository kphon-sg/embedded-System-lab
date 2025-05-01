#include "uart.h"
#include"LPC17xx.h"

const long led_Index[] = {1<<6, 1<<5, 1<<4, 1<< 3, 1<< 2, 1<< 31, 1<< 29, 1<< 28};

void Led_Initialize()
{
  LPC_SC->PCONP       |= (1 << 15);    // Enable power to GPIO & IOCON
  LPC_GPIO1->FIODIR   |= 0xB0000000;   // P1.28,29,31 are outputs
  LPC_GPIO2->FIODIR   |= 0x0000007C;   // P2.2,3,4,5,6 are outputs
}

void Led_On(unsigned int index)
{
  if(index < 5)
  {
    LPC_GPIO2->FIOSET =  led_Index[index];
  }
  else if(index < 8)
  {
    LPC_GPIO1->FIOSET =  led_Index[index];
  }
}

void Led_Off(unsigned int index)
{
  if(index < 5)
  {
    LPC_GPIO2->FIOCLR =  led_Index[index];
  }
  else if(index < 8)
  {
    LPC_GPIO1->FIOCLR =  led_Index[index];
  }
}

void Leds_Set_Value(char led_Value)
{
	int gpio2_Set_Value=0, gpio2_Clr_Value=0, gpio1_Set_Value=0, gpio1_Clr_Value=0;
	int i;
	for(i=0;i<5;i++)
	{
		if(led_Value&(1<<i))
		{
			gpio2_Set_Value|=led_Index[i];
		}
		else
		{
			gpio2_Clr_Value|=led_Index[i];
		}
	}
	for(i=5;i<8;i++)
	{
		if(led_Value&(1<<i))
		{
			gpio1_Set_Value|=led_Index[i];
		}
		else
		{
			gpio1_Clr_Value|=led_Index[i];
		}
	}
	LPC_GPIO2->FIOSET=gpio2_Set_Value;
	LPC_GPIO2->FIOCLR=gpio2_Clr_Value;
	LPC_GPIO1->FIOSET=gpio1_Set_Value;
	LPC_GPIO1->FIOCLR=gpio1_Clr_Value;
}
unsigned int UART_Buffer_Count;
unsigned char UART_Buffer[BUFFER_SIZE];
char UART_TX_Is_Empty = 1;

void UART_Initialize(unsigned int baudRate)
{
  unsigned int divisorLatchRate;
  unsigned int peripheralClock;
	// Enable UART0 Power/Clock
  LPC_SC->PCONP |= 1<<3;
	// Setting PINSEL0[7:4] to [0101] to use P0.2 for TXD0, P0.3 for RXD0
	LPC_PINCON->PINSEL0 &= ~0x000000F0;  // Clear all the upper bA­ts (4~7) to 0
	LPC_PINCON->PINSEL0 |= 0x00000050;   // Set the upper bA­ts (4~7) to [0101]
	// Set UART0 Peripheral Clock = CCLK/4
  LPC_SC->PCLKSEL0 &= ~(3<<6);  // 00: CCLK/4, 01: CCLK, 10: CCLK/2, 11: 	CCLK/8 
	// 8-bit length, 1 stop bit, No Parity & enable access to Divisor Latches
	LPC_UART0->LCR |= 0x83;
	// Let DivAddVal be 0 & MulVal be 1 as default, not access FDR register
	// Calculate Peripheral Clock
  peripheralClock = SystemCoreClock/4;
	// Calculate Divisor Latch Rate, which is 16 * Baud rate
  divisorLatchRate = (peripheralClock/16)/baudRate;
	// Set value for Divisor Latch MSB register
  LPC_UART0->DLM = divisorLatchRate / 256;
	// Set value for Divisor Latch LSB register
  LPC_UART0->DLL = divisorLatchRate % 256;
	// Disable access to Divisor Latches
  LPC_UART0->LCR &= ~(1<<7);
	// Enable FIFO, and reset TX and RX FIFO
	// Set RX Trigger on 1 character recieved
  LPC_UART0->FCR = 0x07;
  // Enable UART0 interrupt
	LPC_UART0->IER = IER_EnableRBR | IER_EnableTHRE | IER_EnableRLS; 
	// Enable External Interrupt in the NVIC interrupt controller
  NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler (void)
{
  int InterruptIdRegister, LineStatusRegister;
  int interruptId;
  int Dummy;
	// Read Interrupt Identification Register
  InterruptIdRegister = LPC_UART0->IIR;
  // Get [3:1] bits indicating the Interrupt Identification
	interruptId = (InterruptIdRegister >> 1)&0x07;
  // RLS (Receive Line Status) Interrupt
  if (interruptId == IIR_ReceiveLineStatus)
  {
    // Not handle these errors in this lab
		// Read Line Status Register will clear the interrupt
    LineStatusRegister = LPC_UART0->LSR;
  }
  // RDA (Receive Data Available) Interrupt
  else if (interruptId == IIR_ReceiveDataAvailable)
  {
    // Get data from UART Reciever Buffer Register
		UART_Buffer[UART_Buffer_Count] = LPC_UART0->RBR;
    UART_Buffer_Count++;
    if (UART_Buffer_Count == 16)
    {
      // Buffer is overflow now
      UART_Buffer_Count = 0;
    }
  }
	// THRE (Transmit Holding Register Empty) Interrupt
  else if (interruptId == IIR_THREInterrupt)
  {
		// Transmitter Holding Register is empty now
    UART_TX_Is_Empty = 1;
  }
  // CTI (Character Timeout Indicator) Interrupt will never occur since
  // we rise "RDA (Receive Data Available) Interrupt"
  // every byte we received.
}

void UART_Send(char *bufferPtr, unsigned int length)
{
  while ( length != 0 )
  {
    // Wait until Transmission is done
		uart_TxChar(*bufferPtr);
		// Transmitter Holding Register is not empty now
    UART_TX_Is_Empty = 0;
    bufferPtr++;
    length--;
  }
}

void uart_TxChar(char ch)
{
    while((LPC_UART0->LSR&(1<<5))==0); // Wait for Previous transmission
    LPC_UART0->THR=ch;                                  // Load the data to be transmitted
}