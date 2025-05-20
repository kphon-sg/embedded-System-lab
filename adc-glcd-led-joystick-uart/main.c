#include "GPIO.h"
#include "ADC.h"
#include "GLCD.h"
#include "UART.h"
#include <string.h>

// Global variables
extern unsigned int UART_Buffer_Count;
extern unsigned char UART_Buffer[BUFFER_SIZE];

// Toggle LED on specified port and pin
void LED_Toggle(uint32_t port, uint32_t pin) {
    if (port == 1) {
        if (LPC_GPIO1->FIOPIN & (1 << pin)) {
            LPC_GPIO1->FIOCLR = (1 << pin);
        } else {
            LPC_GPIO1->FIOSET = (1 << pin);
        }
    } else if (port == 2) {
        if (LPC_GPIO2->FIOPIN & (1 << pin)) {
            LPC_GPIO2->FIOCLR = (1 << pin);
        } else {
            LPC_GPIO2->FIOSET = (1 << pin);
        }
    }
}

// Control LEDs based on UART command
void LED_Control(char command) {
    switch (command) {
        case '0': LED_Toggle(1, 28); break; // P1.28
        case '1': LED_Toggle(1, 29); break; // P1.29
        case '2': LED_Toggle(1, 31); break; // P1.31
        case '3': LED_Toggle(2, 2);  break; // P2.2
        case '4': LED_Toggle(2, 3);  break; // P2.3
        case '5': LED_Toggle(2, 4);  break; // P2.4
        case '6': LED_Toggle(2, 5);  break; // P2.5
        case '7': LED_Toggle(2, 6);  break; // P2.6
        default: break;
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
        }

        int joystickStatus = GPIO_ReadJoystick(); // Read joystick status
        switch (joystickStatus) {
            case 1: // Up
                UART_Send("International University\r\n", strlen("International University\r\n"));
                break;
            case 2: // Down
                UART_Send("Electrical Engineering\r\n", strlen("Electrical Engineering\r\n"));
                break;
            case 3: // Left
                UART_Send("Embedded System\r\n", strlen("Embedded System\r\n"));
                break;
            case 4: // Right
                UART_Send("Your Name\r\n", strlen("Your Name\r\n"));
                break;
            default: break;
        }

        // Process all characters in UART buffer
        while (UART_Buffer_Count > 0) {
            char received = UART_Buffer[0];
            for (int i = 0; i < UART_Buffer_Count - 1; i++) {
                UART_Buffer[i] = UART_Buffer[i + 1]; // Shift buffer left
            }
            UART_Buffer_Count--;
            LED_Control(received); // Toggle LED based on received character
        }
    }
}