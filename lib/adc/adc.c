/*
 * adc.c
 *
 * Created on: May 30, 2025
 * Author: juanjoseerazopacheco
 *
 * Description:
 * This module provides initialization and reading functions for ADC0 and ADC1
 * on the TM4C1294NCPDT microcontroller. It enables analog signal acquisition
 * using polling-based conversion, typically from PE4 (AIN9) and PB4 (AIN10).
 *
 * ADC0 uses Sequencer 3 for single sample conversion (e.g., external potentiometer).
 * ADC1 uses Sequencer 2 for other analog channels (e.g., external temperature sensor LM35).
 * PLL is temporarily enabled for ADC stabilization and disabled after setup.
 */

#include "adc.h"
#include "inc/tm4c1294ncpdt.h"
#include <math.h>

// -------------------- Global Variables --------------------

uint16_t adc0RawValue;       // Last conversion result from ADC0
uint16_t adc0VoltageValue;   // (Not used here)
uint16_t result;             // Shared result buffer for ADC1
int time;                    // Dummy delay variable for clock stabilization

// -------------------- ADC0 Initialization --------------------

void adc0_init(void) {


    // Enable clock to GPIOE for PE4 (AIN9)
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
    time = 0x01234567; // Dummy delay

    // Enable ADC0 module clock
    SYSCTL_RCGCADC_R |= 0x01;
    time = 0x01234567; // Dummy delay

    // Set sample rate to 125 KSPS
    ADC0_PC_R = 0x01;

    // Set sequencer priorities: SS3 highest
    ADC0_SSPRI_R = 0x0123;

    // Disable SS3 for configuration
    ADC0_ACTSS_R &= ~0x0008;

    // Set SS3 trigger to software (EMUX[15:12] = 0000)
    ADC0_EMUX_R &= ~0xF000;

    // Select analog input channel 9 (PE4)
    ADC0_SSMUX3_R = 0x09;

    // End of sequence and interrupt enable (not used)
    ADC0_SSCTL3_R = 0x0006;

    // Disable interrupt for SS3 (polling used)
    ADC0_IM_R &= ~0x0008;

    // Re-enable SS3
    ADC0_ACTSS_R |= 0x0008;

    // Enable PLL for stable ADC operation
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while ((SYSCTL_PLLSTAT_R & 0x01) == 0) {} // Wait for PLL lock

    // Optionally disable PLL to save power after setup
    SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR;
}

// -------------------- ADC1 Initialization --------------------

void adc1_init(void) {

    // Enable clock to GPIOE for PB4 (AIN10)
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    time = 0x01234567; // Dummy delay


    // Enable ADC1 module clock
    SYSCTL_RCGCADC_R |= 0x02;
    time = 0x01234567; // Dummy delay

    // Set sequencer priorities
    ADC1_SSPRI_R = 0x0123;

    // Disable SS2 for configuration
    ADC1_ACTSS_R &= ~0x04;

    // Set trigger source to software (EMUX[11:8] = 0000)
    ADC1_EMUX_R &= ~(0x0F00);

    // Select analog input channel 10 (PB4)
    ADC1_SSMUX2_R = 0x0A;

    // Configure SS2 control bits: END0 + IE0
    ADC1_SSCTL2_R = 0x0006;

    // Disable interrupt for SS2 (polling used)
    ADC1_IM_R &= ~0x04;

    // Enable SS2
    ADC1_ACTSS_R |= 0x04;

    // Enable PLL for ADC stability
    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while ((SYSCTL_PLLSTAT_R & 0x01) == 0) {}

    // Optionally disable PLL to save power after setup
    SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR;
}

// -------------------- ADC0 Read Function --------------------

uint16_t adc0_get_value(void) {
    // Trigger conversion on SS3
    ADC0_PSSI_R = 0x0008;

    // Wait for conversion to complete
    while ((ADC0_RIS_R & 0x08) == 0) {}

    // Read 12-bit result from FIFO
    adc0RawValue = (ADC0_SSFIFO3_R & 0xFFF);

    // Clear completion flag
    ADC0_ISC_R = 0x0008;

    return adc0RawValue;
}

// -------------------- ADC1 Read Function --------------------

uint16_t adc1_get_value(void) {
    // Trigger conversion on SS2
    ADC1_PSSI_R = 0x04;

    // Wait for conversion to complete
    while ((ADC1_RIS_R & 0x04) == 0) {}

    // Read 12-bit result from FIFO
    result = ADC1_SSFIFO2_R & 0xFFF;

    // Clear completion flag
    ADC1_ISC_R = 0x04;

    return result;
}
