

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



// Global variables to store sensor and button input data


uint32_t temperature;
int pj0_status, pj1_status, pj2_status;
float current;



// Function prototypes

void drivers_setup(void);

void ISR_init(void);

void ISR_handler(void);

void Timer03AIntHandler(void);

void read_gpio_inputs(void);

int main(void)
{
    drivers_setup();


    // Infinite loop: polling obtained data

    while(1){
        temperature = adc_get_temperature_value();
        current = adc_get_current_value();
        read_gpio_inputs();
        //SysCtlDelay(1600000); // ~100ms if clock is 16 MHz


    }

	return 0;
}

//Function implementations

void drivers_setup(void){
    adc_init();
    timer_init();
    ISR_init();
    gpio_init_portJ();

}

void read_gpio_inputs(){

    pj0_status = (GPIO_PORTJ_AHB_DATA_R & 0x01);
    pj1_status = (GPIO_PORTJ_AHB_DATA_R & 0x02);
    pj2_status = (GPIO_PORTJ_AHB_DATA_R & 0x04);

}

void ISR_init(void){


}


void ISR_handler(void){



}
void Timer03AIntHandler(void){

}
