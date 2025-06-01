/*
 * stepper_motor.c
 *
 *  Created on: 31 May 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This file implements the control logic to drive a stepper motor using GPIO Port L.
 *  It defines the pulse sequences needed to rotate the motor clockwise and counterclockwise,
 *  sending the sequences directly to the data register of Port L.
 */

#include "stepper_motor.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

// Sequence for rotating motor clockwise (right)
int right_sequence[4] = {0x09, 0x0C, 0x06, 0x03};

// Sequence for rotating motor counterclockwise (left)
int left_sequence[4]  = {0x03, 0x06, 0x0C, 0x09};

int i, j;

void stepper_motor_rotates_right(void) {
    // Iterate through pulse sequence in reverse to rotate right
    for (i = 3; i >= 0; i--) {
        GPIO_PORTL_DATA_R = right_sequence[i];

        // Simple delay loop for motor step timing (non-blocking in real-time)
        for (j = 3000; j >= 0; j--);
    }
}

void stepper_motor_rotates_left(void) {
    // Iterate through pulse sequence in reverse to rotate left
    for (i = 3; i >= 0; i--) {
        GPIO_PORTL_DATA_R = left_sequence[i];

        // Missing delay here could cause erratic behavior in some motors
        for (j = 3000; j >= 0; j--); // Optional: Match timing with right rotation
    }
}
