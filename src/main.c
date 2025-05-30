

/**
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "inc/tm4c1294ncpdt.h"
#include "lib/gpio/gpio.h"
#include "lib/adc/adc.h"
#include "lib/timer/timer.h"



void drivers_setup(void);



int main(void)
{
    drivers_setup();



	return 0;
}


void drivers_setup(void){
    adc_init();
    timer_init();
    gpio_init_portA();
    gpio_init_portB();
    gpio_init_portC();

}
