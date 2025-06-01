/*
 * gpio.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This file implements initialization routines for various GPIO ports
 *  on the TM4C1294NCPDT microcontroller. It configures input/output directions,
 *  digital enables, pull-up resistors, and alternate functions for peripherals such as buttons or timers.
 */

#include "gpio.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>

void gpio_init_portJ(void){
    // Initializes Port J: PJ0, PJ1, PJ2 as digital inputs with pull-up resistors for onboard buttons
    SYSCTL_RCGCGPIO_R |= 0x100;            // Enable clock to Port J
    GPIO_PORTJ_AHB_DIR_R = 0x00;           // Set PJ0, PJ1, PJ2 as input
    GPIO_PORTJ_AHB_PUR_R = 0x07;           // Enable pull-up resistors
    GPIO_PORTJ_AHB_DEN_R = 0x07;           // Enable digital functionality
}

void gpio_init_portL(void){
    // Initializes Port L: PL0–PL3 as digital outputs
    SYSCTL_RCGCGPIO_R |= 0x400;            // Enable clock to Port L
    GPIO_PORTL_DIR_R = 0x0F;               // Set PL[0:3] as outputs
    GPIO_PORTL_DEN_R = 0x0F;               // Enable digital functionality
}

void gpio_init_portD(void){
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

void gpio_init_portN(void){
    // Initializes Port N: PN2 as GPIO input with pull-up
    SYSCTL_RCGCGPIO_R |= 0x1000;           // Enable clock to Port N
    GPIO_PORTN_DIR_R = 0x00;               // Set PN2 as input
    GPIO_PORTN_DEN_R = 0x04;               // Enable digital for PN2
    GPIO_PORTN_PUR_R |= 0x04;              // Enable pull-up resistor on PN2
    GPIO_PORTD_AHB_AFSEL_R &= ~0x04;       // Ensure it's not set to alternate function
}

void gpio_init_portE(void){
    // Initializes PE4 for ADC input
    int time;
    SYSCTL_RCGCGPIO_R |= 0x01;             // Enable clock to Port E
    time = 0x01234567;                     // Dummy delay (for stabilization)

    GPIO_PORTE_AHB_DIR_R &= ~0x10;         // Set PE4 as input
    GPIO_PORTE_AHB_AFSEL_R |= 0x10;        // Enable alternate function (ADC)
    GPIO_PORTE_AHB_DEN_R &= ~0x10;         // Disable digital function
    GPIO_PORTE_AHB_AMSEL_R |= 0x10;        // Enable analog mode
}

void gpio_init_portM(void){
    // Initializes Port M: PM7 for alternate function use
    SYSCTL_RCGCGPIO_R |= 0x800;            // Enable clock to Port M
    GPIO_PORTM_DIR_R = 0x80;               // Set PM7 as output
    GPIO_PORTM_AFSEL_R = 0x80;             // Enable alternate function
    GPIO_PORTM_PCTL_R = 0x30000000;        // Configure PM7 function (depending on module)
    GPIO_PORTM_DEN_R = 0x80;               // Enable digital function
}
