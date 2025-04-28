#include "ADC.h"
#include "LPC17xx.h"
#include "GLCD.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

void ADC_Initialize(void) {
    LPC_SC->PCONP |= (1 << 12);
    LPC_PINCON->PINSEL1 |= (1 << 4);
    LPC_ADC->ADCR = (1 << 2) | (4 << 8) | (1 << 21);
}

void ADC_StartConversion(void) {
    LPC_ADC->ADCR |= (1 << 24);
}

int ADC_ConversionDone(void) {
    return (LPC_ADC->ADGDR & (1U << 31)) ? 1 : 0;
}

int ADC_GetValue(void) {
    uint32_t result = (LPC_ADC->ADGDR >> 4) & 0xFFF;
    return result;
}

void ADC_DisplayValue(void) {
    int adcValue = ADC_GetValue();
    char valueStr[20];
    sprintf(valueStr, "ADC: %d", adcValue);
    GLCD_Clear();
    GLCD_DisplayString(0, 0, valueStr);
    //UART_Send(valueStr, strlen(valueStr));
}
