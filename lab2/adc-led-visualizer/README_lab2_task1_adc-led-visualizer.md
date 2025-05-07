
# ADC LED Visualizer (lab2_task1)

This lab demonstrates how to read analog input from a potentiometer or sensor using the ADC of the LPC17xx microcontroller and display the result in real-time using 8 LEDs.

## 🔍 Overview

The system continuously reads the analog voltage from pin P0.25 (AD0.2), converts it using the ADC, and displays the value on 8 LEDs by interpreting the ADC result as an 8-bit binary number.

## 📥 Input

- Analog voltage (0V to 3.3V) via potentiometer or sensor connected to P0.25

## 💡 Output

- LEDs light up according to the binary value of the ADC result (8-bit)
  - Higher voltage → More LEDs ON
  - Lower voltage → Fewer LEDs ON

## 🔧 How to Use

1. Connect a potentiometer to AD0.2 (P0.25).
2. Build and flash the code to the board.
3. Vary the potentiometer and observe LED patterns change in real-time.

## 🎯 Learning Objectives

- Understand ADC functionality
- Convert analog signals to digital form
- Visualize ADC output with GPIO
