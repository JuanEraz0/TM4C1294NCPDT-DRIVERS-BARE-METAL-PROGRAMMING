

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


//Digital inputs

int pj0_status, pj1_status, pd0_status;

//ADC inputs
uint32_t temperature;
uint16_t current;

//TIMER3 Vars

uint32_t static First = 0; //Timer0A first edge, 12.5ns units
uint8_t frequency;
float clk = 16e6; //Freq of mcu
float period;
float rpm = 0;



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
        stepper_motor_rotates_right();
        temperature = adc_get_temperature_value();
        current = adc_get_current_value();

        read_gpio_inputs();
        //SysCtlDelay(1600000); // ~100ms if clock is 16 MHz

    }

	return 0;
}

//Function implementations

void drivers_setup(void){
    gpio_init_portJ();
    gpio_init_portD();
    gpio_init_portL();

    timer3_init();
    adc_init();

}

void read_gpio_inputs(){

    pj0_status = (GPIO_PORTJ_AHB_DATA_R & 0x01);
    pj1_status = (GPIO_PORTJ_AHB_DATA_R & 0x02);
    pd0_status = (GPIO_PORTD_AHB_DATA_R & 0x01);

}

void ISR_hall_init(void){


}


void ISR_hall_handler(void){



}
void ISR_switch_init(void){


}


void ISR_switch_handler(void){



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

    rpm = (float)frequency*60;
    First = TIMER3_TAR_R; //charges to compare with the next
}
