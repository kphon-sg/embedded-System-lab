#include "LPC17xx.h"
#include "UART.h"

unsigned int UART_Buffer_Count = 0;
unsigned char UART_Buffer[BUFFER_SIZE];
char UART_TX_Is_Empty = 1;

void UART_Initialize(unsigned int baudRate) {
    unsigned int divisorLatchRate;
    unsigned int peripheralClock;

    LPC_SC->PCONP |= (1 << 3); // Enable UART0
    LPC_PINCON->PINSEL0 &= ~(0xF << 4);
    LPC_PINCON->PINSEL0 |= (0x5 << 4); // P0.2 = TXD0, P0.3 = RXD0
    LPC_SC->PCLKSEL0 &= ~(3 << 6); // PCLK = CCLK/4

    LPC_UART0->LCR = 0x83; // 8 bits, no parity, 1 stop bit, DLAB=1
    peripheralClock = SystemCoreClock / 4; // PCLK=25MHz
    divisorLatchRate = (peripheralClock / (16 * baudRate));
    LPC_UART0->DLM = (divisorLatchRate >> 8) & 0xFF;
    LPC_UART0->DLL = divisorLatchRate & 0xFF;
    LPC_UART0->FDR = (0 << 0) | (1 << 4); // No fractional divider
    LPC_UART0->LCR &= ~(1 << 7); // Disable DLAB

    LPC_UART0->FCR = 0x07; // Enable FIFO, reset RX/TX FIFOs
    LPC_UART0->IER = IER_EnableRBR | IER_EnableTHRE | IER_EnableRLS;
    NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler(void) {
    unsigned int interruptId = (LPC_UART0->IIR >> 1) & 0x07;
    if (interruptId == IIR_ReceiveLineStatus) {
        volatile unsigned int status = LPC_UART0->LSR;
        (void)status;
    } else if (interruptId == IIR_ReceiveDataAvailable || interruptId == IIR_CharacterTimeoutIndicator) {
        UART_Buffer[UART_Buffer_Count++] = LPC_UART0->RBR;
        if (UART_Buffer_Count >= BUFFER_SIZE) {
            UART_Buffer_Count = 0; // Wrap around
        }
    } else if (interruptId == IIR_THREInterrupt) {
        UART_TX_Is_Empty = 1;
    }
}

void UART_Send(char *bufferPtr, unsigned int length) {
    while (length--) {
        uart_TxChar(*bufferPtr++);
    }
}

void uart_TxChar(char ch) {
    while (!(LPC_UART0->LSR & LSR_TransmitterHoldingRegisterEmpty));
    LPC_UART0->THR = ch;
    UART_TX_Is_Empty = 0;
}

void UART_SendData(char data) {
    uart_TxChar(data);
}