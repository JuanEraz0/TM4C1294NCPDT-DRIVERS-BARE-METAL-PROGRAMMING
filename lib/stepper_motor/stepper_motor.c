/*
 * stepper_motor.c
 *
 *  Created on: 31 may 2025
 *      Author: juanjoseerazopacheco
 */

#include "stepper_motor.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

int right_sequence [4]= {0x9, 0xc, 0x6, 0x3}; // Stepper motor pulses sends to each coil in order to get a right edge movement
int left_sequence [4] = {0x3, 0x6, 0xc, 0x9}; // Stepper motor pulses sends to each coil in order to get a left edge movement
int i, j;

void stepper_motor_rotates_right(void){
    for(i=3; i >=0; i--){
        GPIO_PORTL_DATA_R = right_sequence[i];
        for(j=3000;j>=0;j--);
    }

}

void stepper_motor_rotates_left(void){
    for(i=3; i >= 0; i--){
        GPIO_PORTL_DATA_R = left_sequence[i];
    }
}





