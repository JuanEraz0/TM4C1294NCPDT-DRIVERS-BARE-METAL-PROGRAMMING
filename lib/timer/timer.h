/*
 * timer.h
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This header declares initialization functions for two hardware timers (Timer 3 and Timer 5)
 *  on the TM4C1294NCPDT microcontroller.
 *  Timer 3 is configured for input capture mode (rising edge), and Timer 5 for PWM generation.
 */

#ifndef TIMER_H_
#define TIMER_H_

// Initializes Timer 3 in capture mode (rising edge detection)
void timer3_init(void);

// Initializes Timer 5 in PWM mode (16-bit, periodic)
void timer5_init(void);

#endif /* TIMER_H_ */
