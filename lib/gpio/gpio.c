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

    //Function to init port J (PJ0 & PJ1) Tiva Embedded push buttons

    SYSCTL_RCGCGPIO_R |= 0x100; //Enables clk to PORT J
    GPIO_PORTJ_AHB_DIR_R = 0x00; //Set PJ0 & PJ1 & PJ2 as input
    GPIO_PORTJ_AHB_PUR_R = 0x07; //Enables PJ0 & PJ1 & PJ2 Pull-Up Resistances
    GPIO_PORTJ_AHB_DEN_R = 0x07; //Set up PJ0 & PJ1 & PJ2 as digital pins

}
void gpio_init_portL(void){

    SYSCTL_RCGCGPIO_R |= 0x400; //Enables clk to PORT L
    GPIO_PORTL_DIR_R = 0x0F; //Set PL[0:3] as output
    GPIO_PORTL_DEN_R = 0x0F; //Set up PJ0 & PJ1 & PJ2 as digital pins


}
void gpio_init_portD(void){

    //Function to Enable PD0 & PD4, PD0 as digital Input, and PD4 as TIMER

    SYSCTL_RCGCGPIO_R |= 0x0008; //Enables clk to PORT D
    GPIO_PORTD_AHB_DIR_R = 0x00; //Set PD0 & PD4 as input
    GPIO_PORTD_AHB_DEN_R = 0x11; //Set up PD0 & PD4 as digital pins
    GPIO_PORTD_AHB_PUR_R |= 0x01; //Enables PD0 Pull-Up Resistances

    GPIO_PORTD_AHB_DATA_R = 0x00;

    GPIO_PORTD_AHB_AFSEL_R = 0x10; //Enables alternate function in PD4
    GPIO_PORTD_AHB_PCTL_R = 0x00030000; // points to T3CCP0 TIMER3
    GPIO_PORTD_AHB_AFSEL_R &= ~0x01; // Asegura que PD0 es GPIO


}

void gpio_init_portN(void){
    SYSCTL_RCGCGPIO_R |= 0x1000; //Enables clk to PORT N
    GPIO_PORTN_DIR_R = 0x00;
    GPIO_PORTN_DEN_R = 0x04;
    GPIO_PORTN_PUR_R |= 0x04;
    GPIO_PORTD_AHB_AFSEL_R &= ~0x04;


}

void gpio_init_portE(void){
    int time;
    SYSCTL_RCGCGPIO_R |= 0x01;
    //while((SYSCTL_PRGPIO_R&0x1)==0){}
    time = 0x01234567;


    // setup gpio PE4 pin
    GPIO_PORTE_AHB_DIR_R &= ~0x10;
    GPIO_PORTE_AHB_AFSEL_R |= ~0x10;
    GPIO_PORTE_AHB_DEN_R &= ~0x10;
    GPIO_PORTE_AHB_AMSEL_R |= 0x10;


}

void gpio_init_portM(void){

    SYSCTL_RCGCGPIO_R |= 0x800; //Enables clk to PORT M
    GPIO_PORTM_DIR_R = 0x80;
    GPIO_PORTM_AFSEL_R = 0x80;
    GPIO_PORTM_PCTL_R =0X0;
    GPIO_PORTM_PCTL_R =0X30000000;
    GPIO_PORTM_DEN_R = 0x80;


}
