
# UART LED Control (lab2_task2)

This project demonstrates how to use UART communication on an LPC17xx microcontroller to control multiple LEDs through commands received from a computer terminal.

## 📌 Overview

The microcontroller receives a single character from a UART terminal (e.g., PuTTY, Tera Term). Based on that character, it turns all LEDs ON or OFF and sends back a response to confirm the action.

## 🛠️ Features

- UART communication at 9600 baud
- Toggle all 8 LEDs ON/OFF via UART commands
- Feedback via UART to confirm actions
- Uses interrupt-driven UART receive logic

## 📥 UART Commands

| Command | Action            | UART Response            |
|---------|-------------------|--------------------------|
| `'1'`   | Turn ON all LEDs  | `'G'` + "LEDs are turned on" |
| `'0'`   | Turn OFF all LEDs | `'B'` + "LEDs are turned off" |

## 📁 File Descriptions

- `task2.c`: Main logic loop; handles received UART commands and updates LED status accordingly.
- `uart.c`: UART initialization, transmit, receive buffer handling, interrupt configuration.
- `uart.h`: Header file containing declarations and macro definitions for UART functions.

## 🧪 How to Use

1. Flash the program to your LPC17xx-based development board.
2. Connect UART0 (P0.2/TXD, P0.3/RXD) to your PC via USB-to-Serial.
3. Open a terminal (e.g., PuTTY) with settings:
   - Baud rate: 9600
   - 8 data bits, 1 stop bit, no parity
4. Type `'1'` or `'0'` and press Enter:
   - `'1'` turns all LEDs on.
   - `'0'` turns all LEDs off.
5. Check LED states and UART terminal feedback.

## 📚 Educational Value

This lab demonstrates how UART can be used for:
- Simple command control
- Communication with external interfaces
- Real-time system feedback
- Interrupt-based I/O design

---

✅ **Recommended as a foundational lab for mastering UART + GPIO integration.**
