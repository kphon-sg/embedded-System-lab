#ifndef ADC_H
#define ADC_H

#include "LPC17xx.h"

// Function prototypes
void ADC_Initialize(void);
void ADC_StartConversion(void);
uint8_t ADC_ConversionDone(void);
uint16_t ADC_GetValue(void);

#endif /* ADC_H */