/*
 * gpio.c
 *
 * Created on: May 30, 2025
 * Author: juanjoseerazopacheco
 *
 * Description:
 * This module provides initialization routines for various GPIO ports
 * on the TM4C1294NCPDT microcontroller.
 *
 * Each function configures the direction, digital/analog enable,
 * pull-up resistors, and alternate functions for pins used in the application.
 * Ports include configurations for buttons, stepper motor outputs,
 * ADC channels, and timer/PWM signals.
 */

#include "gpio.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
int time;

// ---------------------- Port J Initialization ----------------------

void gpio_init_portJ(void){
    // PJ0, PJ1, PJ2 as digital inputs for onboard pushbuttons with pull-up resistors
    SYSCTL_RCGCGPIO_R |= 0x100;            // Enable clock for Port J
    GPIO_PORTJ_AHB_DIR_R = 0x00;           // Configure as inputs
    GPIO_PORTJ_AHB_PUR_R = 0x07;           // Enable pull-up resistors on PJ[0:2]
    GPIO_PORTJ_AHB_DEN_R = 0x07;           // Enable digital functionality on PJ[0:2]
}

// ---------------------- Port L Initialization ----------------------

void gpio_init_portL(void){
    // PL0–PL3 configured as digital outputs for stepper motor coils
    SYSCTL_RCGCGPIO_R |= 0x400;            // Enable clock for Port L
    GPIO_PORTL_DIR_R = 0x0F;               // Set PL[0:3] as outputs
    GPIO_PORTL_DEN_R = 0x0F;               // Enable digital function for PL[0:3]
}

// ---------------------- Port B Initialization ----------------------

void gpio_init_portB(void){
    // PB4 (AIN10) configured as analog input for ADC1
     SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;            // Enable clock for Port B
     time = 0x01234567;                    // Dummy delay

     GPIO_PORTB_AHB_DIR_R &= ~0x10;        // Set PB4 as input
     GPIO_PORTB_AHB_AFSEL_R |= 0x10;       // Enable alternate function (ADC)
     GPIO_PORTB_AHB_DEN_R &= ~0x10;        // Disable digital function
     GPIO_PORTB_AHB_AMSEL_R |= 0x10;       // Enable analog mode
}

// ---------------------- Port D Initialization ----------------------

void gpio_init_portD(void) {
    // Initializes Port D: PD0 as GPIO input, PD4 as TIMER input (T3CCP0)
    SYSCTL_RCGCGPIO_R |= 0x0008;           // Enable clock to Port D
    GPIO_PORTD_AHB_DIR_R = 0x00;           // Set PD0 & PD4 as inputs
    GPIO_PORTD_AHB_DEN_R = 0x11;           // Enable digital for PD0 & PD4
    GPIO_PORTD_AHB_PUR_R |= 0x01;          // Enable pull-up on PD0
    GPIO_PORTD_AHB_DATA_R = 0x00;          // Clear data (good practice)

    GPIO_PORTD_AHB_AFSEL_R = 0x10;         // Enable alternate function for PD4
    GPIO_PORTD_AHB_PCTL_R = 0x00030000;    // Configure PD4 as T3CCP0
    GPIO_PORTD_AHB_AFSEL_R &= ~0x01;       // Ensure PD0 remains as GPIO
}

// ---------------------- Port N Initialization ----------------------

void gpio_init_portN(void){
    // PN2: Digital input with pull-up (e.g., user button)
    SYSCTL_RCGCGPIO_R |= 0x1000;          // Enable clock for Port N
    GPIO_PORTN_DIR_R = 0x00;              // Set PN2 as input
    GPIO_PORTN_DEN_R = 0x04;              // Enable digital on PN2
    GPIO_PORTN_PUR_R |= 0x04;             // Enable pull-up on PN2
    GPIO_PORTD_AHB_AFSEL_R &= ~0x04;      // Ensure PN2 is not in alternate function mode
}

// ---------------------- Port E Initialization ----------------------

void gpio_init_portE(void){
    // PE4: Analog input (AIN9) for ADC0

    SYSCTL_RCGCGPIO_R |= 0x01;            // Enable clock for Port E
    time = 0x01234567;                    // Dummy delay

    GPIO_PORTE_AHB_DIR_R &= ~0x10;        // Set PE4 as input
    GPIO_PORTE_AHB_AFSEL_R |= 0x10;       // Enable alternate function (ADC)
    GPIO_PORTE_AHB_DEN_R &= ~0x10;        // Disable digital function
    GPIO_PORTE_AHB_AMSEL_R |= 0x10;       // Enable analog mode
}

// ---------------------- Port M Initialization ----------------------

void gpio_init_portM(void){
    // PM7: Alternate function output (e.g., Timer/PWM output)
    SYSCTL_RCGCGPIO_R |= 0x800;           // Enable clock for Port M
    GPIO_PORTM_DIR_R = 0x80;              // Set PM7 as output
    GPIO_PORTM_AFSEL_R = 0x80;            // Enable alternate function
    GPIO_PORTM_PCTL_R = 0x30000000;       // Select alternate function (Timer or PWM)
    GPIO_PORTM_DEN_R = 0x80;              // Enable digital function on PM7
}
