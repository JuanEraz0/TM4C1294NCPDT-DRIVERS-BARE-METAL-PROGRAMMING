/*
 * adc.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This file implements the ADC0 configuration and reading routines for
 *  analog signal acquisition (e.g., temperature sensor on PE4) using the
 *  TM4C1294NCPDT microcontroller.
 *  It includes PLL setup and simple polling-based conversion triggering.
 */

#include "adc.h"
#include "inc/tm4c1294ncpdt.h"
#include <math.h>

uint16_t adc0RawValue;
uint16_t adc0VoltageValue;
int time;

void adc0_init(void) {
    // Enable clock for GPIOE (PE4 used as analog input)
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
    // Dummy delay to allow clock to stabilize
    time = 0x01234567;

    // Enable clock to ADC0 module
    SYSCTL_RCGCADC_R |= 0x01;
    time = 0x01234567;

    // Set ADC0 sample rate to 125K samples/sec
    ADC0_PC_R = 0x01;

    // Set sequencer priorities (SS3 has highest priority)
    ADC0_SSPRI_R = 0x0123;

    // Disable SS3 for configuration
    ADC0_ACTSS_R &= ~0x0008;

    // Set SS3 trigger to software
    ADC0_EMUX_R &= ~0xF000;

    // Select channel 9 (PE4 / Temperature sensor)
    ADC0_SSMUX3_R = 0x09;

    // Configure control bits: end of sequence, and enable interrupt flag
    ADC0_SSCTL3_R = 0x0006;

    // Disable interrupt for SS3 (polling used)
    ADC0_IM_R &= ~0x0008;

    // Enable SS3
    ADC0_ACTSS_R |= 0x0008;

    // Power up PLL and wait for lock
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while ((SYSCTL_PLLSTAT_R & 0x01) == 0) {}

    // Optionally disable PLL after ADC setup
    SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR;
}

uint16_t adc0_get_value(void) {
    // Trigger conversion on sequencer 3
    ADC0_PSSI_R = 0x0008;

    // Wait until conversion completes
    while ((ADC0_RIS_R & 0x08) == 0) {}

    // Read 12-bit result from FIFO
    adc0RawValue = (ADC0_SSFIFO3_R & 0xFFF);

    // Clear interrupt flag
    ADC0_ISC_R = 0x0008;

    return adc0RawValue;
}

uint16_t adc1_get_value(void) {
    // Stub for future implementation
    return 0; // To avoid compiler warning
}
