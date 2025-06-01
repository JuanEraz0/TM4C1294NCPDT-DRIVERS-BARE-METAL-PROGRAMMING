/*
 * adc.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 */

#include "adc.h"
#include "inc/tm4c1294ncpdt.h"
#include <math.h>


uint16_t adc0RawValue;
uint16_t adc0VoltageValue;
int time;

void adc0_init(void){

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
    //while((SYSCTL_PRGPIO_R&0x1)==0){}

    // starts ADC0 channel clock

    SYSCTL_RCGCADC_R |= 0x01; //Enables adc0 clock
    time = 0x01234567;

    // ADC0 setup to get temperature value (PE4)

    ADC0_PC_R = 0x01;
    ADC0_SSPRI_R = 0x0123;

    // setup temperature adc sensor (PE4)
    ADC0_ACTSS_R &= ~0x0008;
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF) + 0X09;
    ADC0_SSCTL3_R = 0x0006;
    ADC0_IM_R &= ~0x0008;
    ADC0_ACTSS_R |= 0x0008;

    // starts PLL & wait frequency wet fixed

    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while ((SYSCTL_PLLSTAT_R & 0x01) == 0) {}
    SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR;

}

uint16_t adc0_get_value(){

    // API to get temperature adc lecture
    ADC0_PSSI_R = 0x0008; // starts conversion
    while ((ADC0_RIS_R & 0x08) == 0) {} // wait conversion finished up
    adc0RawValue = (ADC0_SSFIFO3_R & 0xFFF); // result_temperatura -> FIFO3
    ADC0_ISC_R = 0x0008;

    return adc0RawValue;

}

uint16_t adc1_get_value(){


}




