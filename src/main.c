

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

uint32_t temperature;


void drivers_setup(void);

void ISR_init(void);

void ISR_handler(void);

void Timer03AIntHandler(void);


int main(void)
{
    drivers_setup();

    while(1){
        temperature = adc_get_value();
    }




	return 0;
}


void drivers_setup(void){
    adc_init();
    timer_init();
    ISR_init();
    gpio_init_portA();
    gpio_init_portB();
    gpio_init_portC();

}

void ISR_init(void){

}


void ISR_handler(void){



}
