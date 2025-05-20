#include "GLCD.h"

void GLCD_Initialize(void) {
    GLCD_Init();
    GLCD_SetBackgroundColor(Black);
    GLCD_SetForegroundColor(White);
    GLCD_ClearScreen();
}

void GLCD_Clear(void) {
    GLCD_ClearScreen();
}

void GLCD_DisplayString(int x, int y, char* str) {
    GLCD_DisplayString(x, y, 1, (unsigned char *)str);
}