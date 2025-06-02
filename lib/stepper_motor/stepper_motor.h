/*
 * stepper_motor.h
 *
 * Created on: 31 May 2025
 * Author: juanjoseerazopacheco
 *
 * Description:
 * Header file for controlling a stepper motor (unipolar or bipolar)
 * using GPIO Port L on the TM4C1294NCPDT microcontroller.
 *
 * The motor is driven by applying specific sequences to the coils connected to PL0–PL3.
 * Delay between steps determines the speed of rotation.
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// ---------------------- Function Declarations ----------------------

/**
 * @brief Rotates the stepper motor clockwise (right) by applying a forward sequence.
 *
 * @param delay Delay between steps (in arbitrary units) to control speed.
 */
void stepper_motor_rotates_right(uint32_t delay);

/**
 * @brief Rotates the stepper motor counterclockwise (left) by applying a reverse sequence.
 *
 * @param delay Delay between steps (in arbitrary units) to control speed.
 */
void stepper_motor_rotates_left(uint32_t delay);

#endif /* STEPPER_MOTOR_H_ */
