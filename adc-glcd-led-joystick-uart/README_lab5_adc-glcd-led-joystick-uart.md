
# ADC + GLCD + Joystick + UART + LED Control (lab5)

This advanced lab demonstrates real-time system integration using multiple peripherals: ADC, GLCD display, joystick, UART communication, and GPIO control on the LPC17xx platform.

## 🔍 Overview

- Read analog voltage and display it on GLCD
- Send UART messages based on joystick direction
- Toggle LEDs by receiving characters over UART

## 🧭 Features

- **ADC** reads analog voltage
- **GLCD** displays "ADC: [value]"
- **Joystick** sends messages via UART
- **UART RX** toggles LEDs using characters `'0'` to `'7'`

## 💬 UART LED Control

Send a character from PC terminal:
- `'0'` → Toggle LED 0
- `'1'` → Toggle LED 1
- ...
- `'7'` → Toggle LED 7

## 🎯 Educational Focus

- Full-stack embedded integration
- Real-time I/O handling
- UART + display + input + output coordination
