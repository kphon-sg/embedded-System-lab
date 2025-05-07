
# UART + Joystick + LED Controller (lab4)

This lab showcases integration of joystick input, UART communication, and LED control on the LPC17xx microcontroller.

## 🔍 Overview

- Read analog input from ADC
- Read joystick direction
- Send corresponding messages via UART
- Control LEDs based on UART input

## 🎮 Joystick Commands

| Direction | Message Sent via UART          |
|-----------|--------------------------------|
| UP        | International University       |
| DOWN      | Electrical Engineering         |
| LEFT      | Embedded System                |
| RIGHT     | Student Name                   |

## 💡 UART Commands to Control LEDs

- Send 1 byte:
  - High nibble: action (1 = ON, 2 = OFF)
  - Low nibble: LED index (0-7)

Example:
- `0x11` → Turn ON LED 1
- `0x25` → Turn OFF LED 5

## 🧠 Learning Outcomes

- Multi-interface handling
- Real-time interaction
- Using UART for control commands
