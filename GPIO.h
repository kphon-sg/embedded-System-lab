#ifndef GPIO_H
#define GPIO_H

void GPIO_Initialize(void);
int GPIO_ReadJoystick(void);
void GPIO_SetLED(int index, int state);

#endif // GPIO_H
