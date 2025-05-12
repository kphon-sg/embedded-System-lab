#include "adc.h"



// Last converted value
static unsigned int AD_lastValue;
// AD conversion done flag
static unsigned int AD_done;

void ADC_Initialize (void)
{
	// Enable power to ADC & IOCON
  LPC_SC->PCONP |= ((1 << 12) | (1 << 15));
  // Set PINSEL1[19:18] = 01 to select Function AD0.2 for pin P0.25
	LPC_PINCON->PINSEL1 |= 1<<18;
	LPC_PINCON->PINSEL1 &= ~(1<<19);
	// Select AD0.2 pin, ADC clock is 25MHz/5, Enable ADC
  LPC_ADC->ADCR = (1 << 2) | (1 << 10) | (1 << 21);
  // Enable Global ADC interrupt
	LPC_ADC->ADINTEN = (1 <<  8);
	// Enable ADC Interrupt in the NVIC interrupt controller
  NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void)
{
	// Read ADC clears interrupt
  int adstat = LPC_ADC->ADSTAT;
	// Store converted value
  AD_lastValue = (LPC_ADC->ADGDR >> 8) & 0xFF;
  AD_done = 1;
}

void ADC_StartConversion (void)
{
	// Stop conversion
  LPC_ADC->ADCR &= ~(7 << 24);
	// Start conversion
  LPC_ADC->ADCR |=  (1 << 24);
}

uint8_t ADC_ConversionDone(void)
{
  return AD_done;
}

uint16_t ADC_GetValue(void)
{
	return AD_lastValue;
}
volatile uint32_t msTicks;
    
void SysTick_Handler(void) 
{
      msTicks++;
}

// LED bit positions for GPIO2 (LEDs 1-5) and GPIO1 (LEDs 6-8)
#define LED1_BIT (1 << 6)
#define LED2_BIT (1 << 5)
#define LED3_BIT (1 << 4)
#define LED4_BIT (1 << 3)
#define LED5_BIT (1 << 2)
#define LED6_BIT (1 << 31)
#define LED7_BIT (1 << 29)
#define LED8_BIT (1 << 28)


void my_Delay (uint32_t dlyTicks) {
    uint32_t curTicks;
    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

void leds_init(void)
{
    // Enable power to GPIO & IOCON
    LPC_SC->PCONP |= (1 << 15);
    // Configure GPIO2 pins 2-6 as outputs (LEDs 1-5)
    LPC_GPIO2->FIODIR |= (LED1_BIT | LED2_BIT | LED3_BIT | LED4_BIT | LED5_BIT);
    // Configure GPIO1 pins 28,29,31 as outputs (LEDs 6-8)
    LPC_GPIO1->FIODIR |= (LED6_BIT | LED7_BIT | LED8_BIT);
}

void led_on(unsigned int index)
{
    switch(index)
    {
        case 1: 
            LPC_GPIO2->FIOSET = LED1_BIT;
            break;
        case 2: 
            LPC_GPIO2->FIOSET = LED2_BIT;
            break;
        case 3: 
            LPC_GPIO2->FIOSET = LED3_BIT;
            break;
        case 4: 
            LPC_GPIO2->FIOSET = LED4_BIT;
            break;
        case 5: 
            LPC_GPIO2->FIOSET = LED5_BIT;
            break;
        case 6: 
            LPC_GPIO1->FIOSET = LED6_BIT;
            break;
        case 7: 
            LPC_GPIO1->FIOSET = LED7_BIT;
            break;
        case 8: 
            LPC_GPIO1->FIOSET = LED8_BIT;
            break;
        default: 
            break;
    }
}

void led_off(unsigned int index)
{
    switch(index)
    {
        case 1: 
            LPC_GPIO2->FIOCLR = LED1_BIT;
            break;
        case 2: 
            LPC_GPIO2->FIOCLR = LED2_BIT;
            break;
        case 3: 
            LPC_GPIO2->FIOCLR = LED3_BIT;
            break;
        case 4: 
            LPC_GPIO2->FIOCLR = LED4_BIT;
            break;
        case 5: 
            LPC_GPIO2->FIOCLR = LED5_BIT;
            break;
        case 6: 
            LPC_GPIO1->FIOCLR = LED6_BIT;
            break;
        case 7: 
            LPC_GPIO1->FIOCLR = LED7_BIT;
            break;
        case 8: 
            LPC_GPIO1->FIOCLR = LED8_BIT;
            break;
        default: 
            break;
    }
}

void Led_Style_OnOffEachLed(void)
{
    int i;
    for(i=1; i<=8; i++)
    {
        led_on(i);                        // Turn on Led i
        my_Delay(2000);                    // Delay 1000ms
        led_off(i);                       // Turn off Led i
    }
}

void Led_Stye_LedChasing(void)
{
    int i;
    for (i=1; i<=8; i++)
    {
        led_on(i);
        my_Delay(2000);
    }
    for (i=8; i>=1; i--)
    {
        led_off(i);
        my_Delay(2000);
    }
}

void Led_Style_3LedChasing(void)
{
    int i;
    for (i=1; i<=9; i++)
    {
        led_off(i-1);
        led_off(i-2);
        led_off(i-3);
        led_on(i);
        led_on(i+1);
        led_on(i+2);
        my_Delay(1000);
    }
}

void leds_set_value(char led_Value)
{
    int i;
    for(i = 0; i < 8; i++)
    {
        if(led_Value & (1<<i))
        {
            led_on(i+1);
        }
        else
        {
            led_off(i+1);
        }
    }
}