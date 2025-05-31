/*
 * gpio.c
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 */

#include "gpio.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>


void gpio_init_portJ(void){

    //Function to init port J (PJ0 & PJ1) Tiva Embedded push buttons & External button pinned in PJ2

    SYSCTL_RCGCGPIO_R |= 0x100; //Enables clk to PORT J
    GPIO_PORTJ_AHB_DIR_R = 0x00; //Set PJ0 & PJ1 & PJ2 as input
    GPIO_PORTJ_AHB_PUR_R = 0x07; //Enables PJ0 & PJ1 & PJ2 Pull-Up Resistances
    GPIO_PORTJ_AHB_DEN_R = 0x07; //Set up PJ0 & PJ1 & PJ2 as digital pins

}
void gpio_init_portB(void){

}
void gpio_init_portC(void){

}


