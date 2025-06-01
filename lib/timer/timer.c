/*
 * timer.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 */

#include "timer.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>


void timer3_init(void){
    SYSCTL_RCGCTIMER_R |= 0x08; //Enables TIMER3 clk
    while((SYSCTL_PRGPIO_R & 0x0008)==0){}
    TIMER3_CTL_R = 0x00000000; //Disables TIMER3
    TIMER3_CFG_R = 0x00000004; //Set up for 16 Bits
    TIMER3_TAMR_R =0x00000007; //Set up as capture Mode, step down count
    TIMER3_CTL_R &= 0xFFFFFFF3; //Evento flanco de subida (considerando condiciones iniciales de registro)

    TIMER3_TAILR_R = 0x00FFFF;

    //Valor de recarga ( p. 1004)

    TIMER3_TAPR_R = 0xFF; //preescaler TIMER A 24 Bits
    TIMER3_ICR_R = 0x00000004; //cleans interrupt possible flags
    TIMER3_IMR_R |= 0x00000004; //Enables capture interrupt
    NVIC_EN1_R = 1<<(35-32); //Enables TIME3 Interrupt
    TIMER3_CTL_R |= 0x00000001; //Enables TIMER3 in configuration

}

void timer5_init(void){
    SYSCTL_RCGCTIMER_R |= 0x20; //Enables TIMER5 clk
    while((SYSCTL_PRGPIO_R & 0x800)==0){}
    TIMER5_CTL_R &= ~0x100; //Disables TIMER5
    TIMER5_CFG_R = 0x4; //Set up for 16 Bits
    TIMER5_TBMR_R |= 0x40A; //Set up as PWM Mode, step down count, periodic MODE

    TIMER5_TBILR_R = 0xFFF;
    TIMER5_TBMATCHR_R = 0;

    //Valor de recarga ( p. 1004)

    //TIMER5_TBPR_R = 0xFF; //preescaler TIMER B 256

    TIMER5_CTL_R |= 0x4100; //Enables TIMER3 in configuration

}


