#ifndef ADC_H
#define ADC_H

void ADC_Initialize(void);
void ADC_StartConversion(void);
int ADC_ConversionDone(void);
int ADC_GetValue(void);
void ADC_DisplayValue(void);

#endif
