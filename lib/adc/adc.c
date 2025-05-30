/*
 * adc.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 */

#include "adc.h"
#include "inc/tm4c1294ncpdt.h"
#include <math.h>





void adc_init(void){

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
    //while((SYSCTL_PRGPIO_R&0x1)==0){}
    time = 0x01234567;

    // setup gpio PE4 pin
    GPIO_PORTE_AHB_DIR_R &= ~0x10;
    GPIO_PORTE_AHB_AFSEL_R |= ~0x10;
    GPIO_PORTE_AHB_DEN_R &= ~0x10;
    GPIO_PORTE_AHB_AMSEL_R |= 0x10;

    // starts ADC0 channel clock

    SYSCTL_RCGCADC_R |= 0x01;
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

    // setup current adc sensor (PE5)
    ADC0_ACTSS_R &= ~0x0001;
    ADC0_SSMUX0_R = (ADC0_SSMUX0_R & 0xFFFFFFF0) + 0X0A;
    ADC0_SSCTL0_R = 0x0006;
    ADC0_IM_R &= ~0x0001;
    ADC0_ACTSS_R |= 0x0001;

    // starts PLL & wait frequency wet fixed

    SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;
    while ((SYSCTL_PLLSTAT_R & 0x01) == 0) {}
    SYSCTL_PLLFREQ0_R &= ~SYSCTL_PLLFREQ0_PLLPWR;

}

uint16_t adc_get_value(){


    ADC0_PSSI_R = 0x0008; // starts conversion
    while ((ADC0_RIS_R & 0x08) == 0) {} // wait conversion finished up
    result_temperature = (ADC0_SSFIFO3_R & 0xFFF); // result_temperatura -> FIFO3
    ADC0_ISC_R = 0x0008;

    // Lectura y cálculo de corriente
    ADC0_PSSI_R = 0x0001; // Arranca la conversión
    while ((ADC0_RIS_R & 0x01) == 0) {} // Espera a que termine la conversión
    result_current = (ADC0_SSFIFO0_R & 0xFFF);
    ADC0_ISC_R = 0x0001;
    result_current_f = (float)result_current;
    voltage_current = (result_current_f*5.5)/4095; // Resolución
    current = (voltage_current - 2.55) * 45.4545;
    return result_temperature;

}




