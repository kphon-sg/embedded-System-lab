#ifndef __UART_H__
#define __UART_H__

#define IER_EnableRBR                  0x01
#define IER_EnableTHRE                 0x02
#define IER_EnableRLS                  0x04

#define IIR_ReceiveLineStatus           0x03
#define IIR_ReceiveDataAvailable        0x02
#define IIR_CharacterTimeoutIndicator   0x06
#define IIR_THREInterrupt               0x01

#define LSR_ReceiverDataReady            0x01
#define LSR_TransmitterHoldingRegisterEmpty 0x20

#define BUFFER_SIZE                     0x40  // 64 bytes

extern unsigned int UART_Buffer_Count;
extern unsigned char UART_Buffer[BUFFER_SIZE];
extern void uart_TxChar(char ch);
extern void UART_Initialize(unsigned int baudRate);
extern void UART_Send(char *bufferPtr, unsigned int length);
extern void UART_SendData(char data);

#endif
