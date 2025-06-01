/*
 * stepper_motor.h
 *
 *  Created on: 31 May 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This header declares functions to control a unipolar/bipolar stepper motor
 *  via Port L on the TM4C1294NCPDT microcontroller.
 *  Rotation is controlled by sending predefined sequences to motor coils.
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include <stdlib.h>
#include <stdio.h>

// Rotates the stepper motor clockwise using pulse sequence
void stepper_motor_rotates_right(void);

// Rotates the stepper motor counterclockwise using pulse sequence
void stepper_motor_rotates_left(void);

#endif /* STEPPER_MOTOR_H_ */
