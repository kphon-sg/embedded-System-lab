#include "GPIO.h"
#include "LPC17xx.h"

void GPIO_Initialize(void) {
    LPC_GPIO1->FIODIR &= ~(0xF << 23);
    LPC_GPIO1->FIODIR |= (1<<28) | (1<<29) | (1<<31);
    LPC_GPIO2->FIODIR |= (0x7F << 2);
}

int GPIO_ReadJoystick(void) {
    uint32_t joystick = (LPC_GPIO1->FIOPIN >> 23) & 0xF;
    if (!(joystick & (1 << 0))) return 1;
    if (!(joystick & (1 << 1))) return 2;
    if (!(joystick & (1 << 2))) return 3;
    if (!(joystick & (1 << 3))) return 4;
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
