

/**
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "inc/tm4c1294ncpdt.h"
#include "lib/gpio/gpio.h"
#include "lib/adc/adc.h"
#include "lib/timer/timer.h"
#include "lib/stepper_motor/stepper_motor.h"



// Global variables to store sensor and button input data


//Digital inputs vars

int pj0_status, pj1_status, pd0_status;

//ADC vars
uint32_t potentiometer;
uint16_t potentiometerVoltage;

//TIMER3 Vars

uint32_t static First = 0; //Timer0A first edge, 12.5ns units
uint8_t frequency;
float clk = 16e6; //Freq of mcu
float period;
float rpm = 0;

//TIMER5 Vars
int dutyCycle = 0x7FF;

//ISR control vars
volatile uint8_t hall_triggered = 0;

// Function prototypes

void drivers_setup(void);

void read_gpio_inputs(void);

//ISRs

void ISR_hall_init(void);

void ISR_hall_handler(void);

void ISR_switch_init(void);

void ISR_switch_handler(void);

void Timer03AIntHandler(void);



int main(void)
{
    drivers_setup();


    // Infinite loop: polling obtained data

    while(1){

        potentiometerVoltage = ((adc0_get_value()*330)/4095)/100;
        dutyCycle = adc0_get_value();
        TIMER5_TBMATCHR_R = dutyCycle;

        read_gpio_inputs();

        if (hall_triggered) {
            stepper_motor_rotates_right(); // ejecuta solo si fue activado
        }


    }

	return 0;
}

//Function implementations

void drivers_setup(void){
    gpio_init_portJ();
    gpio_init_portD();
    gpio_init_portL();
    gpio_init_portN();
    gpio_init_portM();
    ISR_switch_init();
    ISR_hall_init();
    adc0_init();
    timer3_init();
    timer5_init();


}

void read_gpio_inputs(){

    pj0_status = (GPIO_PORTJ_AHB_DATA_R & 0x01);
    pj1_status = (GPIO_PORTJ_AHB_DATA_R & 0x02);
    //pd0_status = (GPIO_PORTD_AHB_DATA_R & 0x01);
    pd0_status = GPIO_PORTD_AHB_MIS_R;
}

void ISR_hall_init(void){

    GPIO_PORTD_AHB_IM_R = 0x00; //Disables uDMA detect Port Interrupts
    GPIO_PORTD_AHB_IS_R = 0x00; //detects edge falling LOW -> HIGH (Pull Up resistance)
    GPIO_PORTD_AHB_IBE_R = 0x00;
    GPIO_PORTD_AHB_IEV_R = 0x01; //interrupt event as falling edge
    GPIO_PORTD_AHB_ICR_R=0x01; //Clean interrupt flag
    GPIO_PORTD_AHB_IM_R = 0x01; //Enables uDMA detect Port Interrupts

    NVIC_EN0_R |= 0x00000008;

}


void ISR_hall_handler(void){

    if (GPIO_PORTD_AHB_MIS_R & 0x01) { // verifica si fue PD0
        GPIO_PORTD_AHB_ICR_R = 0x01;   // limpia la interrupción
        hall_triggered = 0;  // Señal para main()

    }

}
void ISR_switch_init(void){

    GPIO_PORTN_IM_R = 0x00; //Disables uDMA detect Port Interrupts
    GPIO_PORTN_IS_R = 0x00; //detects edge falling HIGH->LOW (Pull Up resistance)
    GPIO_PORTN_IBE_R = 0x00;
    GPIO_PORTN_IEV_R = 0x00; //interrupt event as falling edge
    GPIO_PORTN_ICR_R=0x04; //Clean interrupt flag
    GPIO_PORTN_IM_R = 0x04; //Enables uDMA detect Port Interrupts

    NVIC_EN2_R |= 0x00000200;

}


void ISR_switch_handler(void){

    if (GPIO_PORTN_MIS_R & 0x04) {
        GPIO_PORTN_ICR_R = 0x04;   // limpia la interrupción
        hall_triggered = 1;  // Señal para main()

    }


}
void Timer03AIntHandler(void){

    TIMER3_ICR_R = 0X00000004; //cleans capture event flag
    if(First == 0) First = TIMER3_TAR_R;

    if(First > TIMER3_TAR_R)
        period = (float)( (First - TIMER3_TAR_R)&0x00FFFFFF); //12.5ns resolution
    else
        period = (float)( (0x00FFFFFF - TIMER3_TAR_R)+First); //C1 + (MAX_VAL - CURRENT_VAL)

    period = period * 1/clk;
    frequency = (uint8_t)1/period;
    First = TIMER3_TAR_R; //charges to compare with the next
    rpm = (float)frequency*60;

}
