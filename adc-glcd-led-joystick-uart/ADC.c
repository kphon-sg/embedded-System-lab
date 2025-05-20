#include "ADC.h"
#include "LPC17xx.h"
#include "GLCD.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

#define ADC_AVG_SAMPLES 8

void ADC_Initialize(void) {
    LPC_SC->PCONP |= (1 << 12); // Enable ADC power
    LPC_PINCON->PINSEL1 |= (1 << 18); // P0.25 as AD0.2
    LPC_PINCON->PINMODE1 &= ~(3 << 18); // Disable pull-up/pull-down
    LPC_ADC->ADCR = (1 << 2) | (4 << 8) | (1 << 21); // Select AD0.2, CLKDIV=4, power on
}

void ADC_StartConversion(void) {
    LPC_ADC->ADCR |= (1 << 24); // Start conversion
}

int ADC_ConversionDone(void) {
    return (LPC_ADC->ADGDR & (1U << 31)) ? 1 : 0;
}

int ADC_GetValue(void) {
    uint32_t result = (LPC_ADC->ADGDR >> 4) & 0xFFF;
    return result;
}

void ADC_DisplayValue(void) {
    static int buffer[ADC_AVG_SAMPLES] = {0};
    static int index = 0;
    static int sum = 0;

    int adcValue = ADC_GetValue();
    sum -= buffer[index];
    buffer[index] = adcValue;
    sum += adcValue;
    index = (index + 1) % ADC_AVG_SAMPLES;

    int avgValue = sum / ADC_AVG_SAMPLES;
    float voltage = (avgValue * 3.3) / 4095.0;

    char valueStr[32];
    sprintf(valueStr, "ADC: %d (%.2f V)", avgValue, voltage);
    GLCD_Clear();
    GLCD_DisplayString(0, 0, valueStr);

    // Send ADC value via UART
    UART_Send(valueStr, strlen(valueStr));
    UART_Send("\r\n", 2);
}