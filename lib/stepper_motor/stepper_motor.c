/*
 * stepper_motor.c
 *
 * Created on: 31 May 2025
 * Author: juanjoseerazopacheco
 *
 * Description:
 * Implements the control logic to drive a stepper motor using GPIO Port L
 * on the TM4C1294NCPDT microcontroller.
 *
 * The motor is rotated by sending specific 4-step sequences (half-stepping or wave drive)
 * to the motor coils connected to PL0–PL3. The direction of rotation is determined
 * by the order in which the signals are applied (CW or CCW).
 */

#include "stepper_motor.h"
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

// ---------------------- Stepper Sequences ----------------------

// Right (clockwise) rotation sequence for 4-step motor
int right_sequence[4] = {0x09, 0x0C, 0x06, 0x03};

// Left (counterclockwise) rotation sequence (reverse order)
int left_sequence[4]  = {0x03, 0x06, 0x0C, 0x09};

// Loop counters
int i;
volatile j; // Volatile to prevent optimization during delay loop

// ---------------------- Motor Control Functions ----------------------

/**
 * @brief Rotates the stepper motor clockwise by iterating through the forward sequence.
 *
 * @param delay Delay between coil activations to control speed.
 */
void stepper_motor_rotates_right(uint32_t delay) {
    for (i = 0; i < 4; i++) {
        GPIO_PORTL_DATA_R = right_sequence[i]; // Send pattern to PL0–PL3
        for (j = 0; j < delay; j++);           // Basic delay loop
    }
}

/**
 * @brief Rotates the stepper motor counterclockwise by iterating through the reverse sequence.
 *
 * @param delay Delay between coil activations to control speed.
 */
void stepper_motor_rotates_left(uint32_t delay) {
    for (i = 0; i < 4; i++) {
        GPIO_PORTL_DATA_R = left_sequence[i];  // Send pattern to PL0–PL3
        for (j = 0; j < delay; j++);           // Basic delay loop
    }
}
