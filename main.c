#include "GPIO.h"
#include "ADC.h"
#include "GLCD.h"
#include "UART.h"
#include <string.h> // Needed for strlen()
#include "LPC17xx.h"

// Global variables
extern unsigned int UART_Buffer_Count;
extern unsigned char UART_Buffer[BUFFER_SIZE];

void LED_Toggle(uint32_t port, uint32_t pin) {
    if (port == 1) {
        if (LPC_GPIO1->FIOPIN & (1 << pin)) { // Check if LED is on
            LPC_GPIO1->FIOCLR = (1 << pin);   // Turn off
        } else {
            LPC_GPIO1->FIOSET = (1 << pin);   // Turn on
        }
    } else if (port == 2) {
        if (LPC_GPIO2->FIOPIN & (1 << pin)) { // Check if LED is on
            LPC_GPIO2->FIOCLR = (1 << pin);   // Turn off
        } else {
            LPC_GPIO2->FIOSET = (1 << pin);   // Turn on
        }
    }
}

// Modified LED_Control function to toggle LEDs
void LED_Control(char command) {
    switch (command) {
        case '0':
            LED_Toggle(1, 28); // Toggle LED on P1.28
            break;
        case '1':
            LED_Toggle(1, 29); // Toggle LED on P1.29
            break;
        case '2':
            LED_Toggle(1, 31); // Toggle LED on P1.31
            break;
        case '3':
            LED_Toggle(2, 2);  // Toggle LED on P2.2
            break;
        case '4':
            LED_Toggle(2, 3);  // Toggle LED on P2.3
            break;
        case '5':
            LED_Toggle(2, 4);  // Toggle LED on P2.4
            break;
        case '6':
            LED_Toggle(2, 5);  // Toggle LED on P2.5
            break;
        case '7':
            LED_Toggle(2, 6);  // Toggle LED on P2.6
            break;
        default:
            break;
    }
}

int main(void) {
    GLCD_Initialize();   // Initialize GLCD
    UART_Initialize(9600); // Initialize UART at 9600 baud
    ADC_Initialize();    // Initialize ADC
    GPIO_Initialize();   // Initialize GPIO for Joystick and LEDs

    while (1) {
        ADC_StartConversion(); // Start ADC conversion

        if (ADC_ConversionDone()) {
            ADC_DisplayValue(); // Display ADC value on GLCD
            // Also send ADC value via UART inside ADC_DisplayValue() if needed
        }

        int joystickStatus = GPIO_ReadJoystick(); // Read joystick status

        switch (joystickStatus) {
            case 1: // Joystick pulled up
                UART_Send("International University", strlen("International University"));
                break;
            case 2: // Joystick pulled down
                UART_Send("Electrical Engineering", strlen("Electrical Engineering"));
                break;
            case 3: // Joystick pulled left
                UART_Send("Embedded System", strlen("Embedded System"));
                break;
            case 4: // Joystick pulled right
                UART_Send("Your Name", strlen("Your Name"));
                break;
            default:
                break;
        }
				
        if (UART_Buffer_Count > 0) {
					char received = UART_Buffer[0]; // Get first received character
					UART_Buffer_Count = 0;           // Clear buffer count

					LED_Control(received); // Call LED control based on received character
				}
    }
}
