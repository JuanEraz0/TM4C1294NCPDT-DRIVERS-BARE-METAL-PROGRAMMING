/*
 * timer.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This file contains implementations for configuring Timer 3 and Timer 5
 *  on the TM4C1294NCPDT microcontroller.
 *  Timer 3 is used for input capture on a rising edge, useful for pulse measurements.
 *  Timer 5 is configured for PWM signal generation using Timer B.
 */

#include "timer.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>

void timer3_init(void) {
    // Enable clock to Timer 3
    SYSCTL_RCGCTIMER_R |= 0x08;

    // Wait until GPIO module for Timer 3 is ready (Port D)
    while ((SYSCTL_PRGPIO_R & 0x0008) == 0) {}

    // Disable Timer A before configuration
    TIMER3_CTL_R = 0x00000000;

    // Configure Timer 3 as 16-bit timer
    TIMER3_CFG_R = 0x00000004;

    // Set Timer A mode: capture mode, count down, edge-time capture
    TIMER3_TAMR_R = 0x00000007;

    // Configure capture event for rising edge
    TIMER3_CTL_R &= 0xFFFFFFF3;

    // Set interval load value (optional upper bound)
    TIMER3_TAILR_R = 0x00FFFF;

    // Set prescaler for extended 24-bit range
    TIMER3_TAPR_R = 0xFF;

    // Clear any existing interrupt flags
    TIMER3_ICR_R = 0x00000004;

    // Enable capture interrupt
    TIMER3_IMR_R |= 0x00000004;

    // Enable interrupt in NVIC (interrupt number 35)
    NVIC_EN1_R = 1 << (35 - 32);

    // Enable Timer A
    TIMER3_CTL_R |= 0x00000001;
}

void timer5_init(void) {
    // Enable clock to Timer 5
    SYSCTL_RCGCTIMER_R |= 0x20;

    // Wait until GPIO module for Timer 5 is ready (Port M)
    while ((SYSCTL_PRGPIO_R & 0x800) == 0) {}

    // Disable Timer B before configuration
    TIMER5_CTL_R &= ~0x100;

    // Configure Timer 5 as 16-bit timer
    TIMER5_CFG_R = 0x00000004;

    // Set Timer B mode: PWM, periodic, count down
    TIMER5_TBMR_R |= 0x040A;

    // Set interval load value for PWM period
    TIMER5_TBILR_R = 0x0FFF;

    // Set match value for PWM duty cycle (initially 0%)
    TIMER5_TBMATCHR_R = 0x0000;

    // (Optional) prescaler could be configured if needed:
    // TIMER5_TBPR_R = 0xFF;

    // Enable Timer B with PWM configuration
    TIMER5_CTL_R |= 0x4100;
}
