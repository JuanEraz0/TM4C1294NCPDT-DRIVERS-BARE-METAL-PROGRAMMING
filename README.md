# TM4C1294NCPDT-DRIVERS-BARE-METAL-PROGRAMMING  
# Stepper Motor Controller – TM4C1294NCPDT

> **Author:** Juan José Erazo Pacheco  
> **Date:** May 2025  
> **Platform:** Tiva C Series TM4C1294NCPDT  
> **Toolchain:** TI Code Composer Studio / GCC (bare-metal)

---

## Overview

This embedded project demonstrates how to control a **stepper motor** using a **TM4C1294NCPDT microcontroller**, integrating multiple peripherals and techniques:

- GPIO for digital inputs (buttons, Hall sensor)  
- ADC for analog input via a potentiometer  
- PWM using Timer 5 for duty cycle control  
- Capture mode via Timer 3 to measure RPM  
- External interrupt handling  
- Modular and reusable driver structure

The motor rotates in response to external triggers, such as Hall effect sensor pulses or push buttons. A potentiometer sets the PWM duty cycle, and RPM is calculated using input capture mode.

---

## Project Structure
```bash
/project-root
│
├── src/
│   └── main.c                      # Main application logic
│
├── lib/
│   ├── adc/
│   │   ├── adc.c                   # ADC0 driver (PE4 input)
│   │   └── adc.h
│   ├── gpio/
│   │   ├── gpio.c                  # GPIO config for ports J, D, L, E, M, N
│   │   └── gpio.h
│   ├── timer/
│   │   ├── timer.c                 # Timer3 (Capture) and Timer5 (PWM)
│   │   └── timer.h
│   └── stepper_motor/
│       ├── stepper_motor.c         # Stepper motor control sequences
│       └── stepper_motor.h
│
├── inc/
│   └── tm4c1294ncpdt.h             # MCU register definitions
└── README.md                       # You are here
```

---

## Features

| Feature              | Description                                  |
|----------------------|----------------------------------------------|
| **GPIO**             | Reads button states and Hall sensor input    |
| **ADC0**             | Reads analog voltage from potentiometer      |
| **Timer 3**          | Capture mode to calculate RPM from pulses    |
| **Timer 5**          | Generates PWM using Timer B (16-bit mode)    |
| **Interrupts**       | Handles button and Hall events via GPIO ISR  |
| **Stepper Motor**    | Rotates left/right based on trigger source   |

---

## Hardware Requirements

- TM4C1294NCPDT Launchpad (Tiva C Series)  
- Stepper motor (e.g., 28BYJ-48 + ULN2003 driver)  
- Potentiometer (connected to PE4)  
- Push buttons connected to PJ0, PJ1, or PN2  
- Hall effect sensor connected to PD0  

---

## How It Works

1. **Motor trigger**: A rising edge on the Hall sensor or pressing a button enables motor movement.  
2. **Analog control**: A potentiometer adjusts the PWM signal applied to the motor.  
3. **Frequency capture**: Timer 3 calculates pulse period to estimate motor RPM.  
4. **PWM output**: Timer 5 generates PWM using the duty cycle derived from the ADC.  
5. **Direction**: The motor rotates clockwise by default when triggered.

---

## Interrupt Behavior

- **PD0 (Hall effect sensor)**: Rising edge enables motor rotation.  
- **PN2 (Push button)**: Falling edge also enables motor rotation.  
- **Timer3A**: Captures time between Hall pulses to calculate frequency and RPM.

---

## To-Do / Improvements

- Add `adc1_get_value()` support for dual analog sensing  
- Abstract hardware register accesses using macros for portability  
- Integrate FreeRTOS or cooperative scheduler  
- Replace delay loops with timer-based non-blocking wait  
- Add UART for real-time debugging or data logging  

---

## References

- Tiva™ TM4C129x Datasheet – Texas Instruments  
- *Embedded Systems: Real-Time Interfacing to ARM® Cortex™-M* – Jonathan Valvano  
- ARM® Cortex™-M4 Technical Reference Manual  

---

## License

This project is provided under the terms of the **GNU General Public License v3.0 (GPL-3.0)**.  
You are free to copy, modify, and distribute this code under the same license.  
Read the full license text here: [GNU GPL 3.0](https://www.gnu.org/licenses/gpl-3.0.html)

---

##  Getting Started – Load Project in Code Composer Studio (CCS)

### 1. Clone the Repository

```bash
git clone https://github.com/JuanEraz0/TM4C1294NCPDT-DRIVERS-BARE-METAL-PROGRAMMING.git
cd TM4C1294NCPDT-DRIVERS-BARE-METAL-PROGRAMMING
zip -r project.zip .
```
### 2. Open zip file in a CCS workspace 
### 3. Add TivaWare to Your Project

This project depends on the **TivaWare Peripheral Driver Library**.

#### If you already have TivaWare installed:

1. Right-click your project in **Code Composer Studio** and select **Properties**
2. Navigate to `Build → ARM Compiler → Include Options`
3. Click **Add** and provide the path to your TivaWare installation. For example:

```bash
C:/ti/TivaWare_C_Series-2.2.0.295
```

#### If you don’t have TivaWare installed:
You can clone a compatible version from this GitHub mirror:

```bash
git clone https://github.com/TaLucGiaHoang/SW-TM4C.git
```