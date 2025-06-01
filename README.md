# TM4C1294NCPDT-DRIVERS-BARE-METAL-PROGRAMMING
# Stepper Motor Controller – TM4C1294NCPDT

> **Author:** Juan José Erazo Pacheco  
> **Date:** May 2025  
> **Platform:** Tiva C Series TM4C1294NCPDT  
> **Toolchain:** TI Code Composer Studio / Keil / GCC (bare-metal)

---

## 🧠 Overview

This embedded project demonstrates how to control a **stepper motor** using a **TM4C1294NCPDT microcontroller**, integrating multiple peripherals and techniques:

- GPIO for digital inputs (buttons, Hall sensor)
- ADC for analog input via a potentiometer
- PWM using Timer 5 for duty cycle control
- Capture mode via Timer 3 to measure RPM
- External interrupt handling
- Modular and reusable driver structure

The motor rotates in response to external triggers, such as Hall effect sensor pulses or push buttons. A potentiometer sets the PWM duty cycle, and RPM is calculated using input capture mode.

---

## 🧩 Project Structure


---

## ⚙️ Features

| Feature              | Description                                  |
|----------------------|----------------------------------------------|
| **GPIO**             | Reads button states and Hall sensor input    |
| **ADC0**             | Reads analog voltage from potentiometer      |
| **Timer 3**          | Capture mode to calculate RPM from pulses    |
| **Timer 5**          | Generates PWM using Timer B (16-bit mode)    |
| **Interrupts**       | Handles button and Hall events via GPIO ISR  |
| **Stepper Motor**    | Rotates left/right based on trigger source   |

---

## 🛠️ Hardware Requirements

- TM4C1294NCPDT launchpad (Tiva C Series)
- Stepper motor (e.g., 28BYJ-48 + ULN2003)
- Potentiometer (connected to PE4)
- Push button(s) connected to PJ0, PJ1 or PN2
- Hall effect sensor on PD0 (external pulses)

---

## 🔄 How It Works

1. **Motor trigger**: A rising edge on the Hall sensor or pressing a button enables motor movement.
2. **Analog control**: A potentiometer adjusts the PWM signal applied to the motor.
3. **Frequency capture**: Timer 3 calculates pulse period to estimate motor RPM.
4. **PWM output**: Timer 5 generates PWM using the duty cycle derived from the ADC.
5. **Direction**: By default, the motor rotates clockwise when triggered.

---

## 🚨 Interrupt Behavior

- **PD0 (Hall effect)**: Rising edge enables motor rotation.
- **PN2 (Button)**: Falling edge also enables motor rotation.
- **Timer3A**: Captures time between Hall pulses to calculate frequency and RPM.

---

## 📋 To-Do / Improvements

- Add `adc1_get_value()` support for dual analog sensing
- Abstract hardware register accesses using macros for portability
- Integrate FreeRTOS or cooperative scheduler
- Replace delay loops with timer-based non-blocking wait
- Add UART for real-time debugging or data logging

---

## 📚 References

- Tiva™ TM4C129x Datasheet – Texas Instruments
- Embedded Systems: Real-Time Interfacing to ARM® Cortex™-M – Valvano
- ARM® Cortex™-M4 Technical Reference Manual

---

## ✅ License

This project is provided for educational purposes and is open for extension under the [MIT License](https://opensource.org/licenses/MIT).

