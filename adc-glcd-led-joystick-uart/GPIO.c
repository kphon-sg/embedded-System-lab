#include "GPIO.h"
#include "LPC17xx.h"

void GPIO_Initialize(void) {
    // Configure joystick pins as inputs (P1.23, P1.25, P1.24, P1.26)
    LPC_PINCON->PINSEL3 &= ~((3 << 14) | (3 << 18) | (3 << 16) | (3 << 20));
    LPC_PINCON->PINMODE3 &= ~((3 << 14) | (3 << 18) | (3 << 16) | (3 << 20));
    LPC_GPIO1->FIODIR &= ~((1 << 23) | (1 << 25) | (1 << 24) | (1 << 26));

    // Configure LED pins as outputs
    LPC_PINCON->PINSEL3 &= ~((3 << 24) | (3 << 26) | (3 << 30)); // P1.28, P1.29, P1.31
    LPC_PINCON->PINSEL4 &= ~((3 << 4) | (3 << 6) | (3 << 8) | (3 << 10) | (3 << 12)); // P2.2-P2.6
    LPC_GPIO1->FIODIR |= (1 << 28) | (1 << 29) | (1 << 31);
    LPC_GPIO2->FIODIR |= (0x7F << 2);
    LPC_GPIO1->FIOCLR = (1 << 28) | (1 << 29) | (1 << 31);
    LPC_GPIO2->FIOCLR = (0x7F << 2);
}

int GPIO_ReadJoystick(void) {
    if (!(LPC_GPIO1->FIOPIN & (1 << 23))) return 1; // Up
    if (!(LPC_GPIO1->FIOPIN & (1 << 25))) return 2; // Down
    if (!(LPC_GPIO1->FIOPIN & (1 << 24))) return 3; // Left
    if (!(LPC_GPIO1->FIOPIN & (1 << 26))) return 4; // Right
    return 0;
}

void GPIO_SetLED(int index, int state) {
    if (index < 3) {
        if (state)
            LPC_GPIO1->FIOSET = (1 << (28 + index));
        else
            LPC_GPIO1->FIOCLR = (1 << (28 + index));
    } else {
        if (state)
            LPC_GPIO2->FIOSET = (1 << (index - 1));
        else
            LPC_GPIO2->FIOCLR = (1 << (index - 1));
    }
}