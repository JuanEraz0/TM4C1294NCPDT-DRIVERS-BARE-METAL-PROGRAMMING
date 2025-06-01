/*
 * gpio.h
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This header provides GPIO initialization function declarations
 *  for different ports (J, L, D, N, E, M) on the TM4C1294NCPDT microcontroller.
 *  Each function sets up digital inputs, outputs, or alternate functions
 *  for specific use cases (e.g., buttons, ADC, timers).
 */

#ifndef GPIO_H_
#define GPIO_H_

// Initializes Port J (PJ0–PJ2) for digital input with pull-ups (push buttons)
void gpio_init_portJ(void);

// Initializes Port L (PL0–PL3) for digital output
void gpio_init_portL(void);

// Initializes Port D (PD0 as input, PD4 as timer input for T3CCP0)
void gpio_init_portD(void);

// Initializes Port N (PN2 as digital input with pull-up)
void gpio_init_portN(void);

// Initializes Port E (PE4 as analog input for ADC0)
void gpio_init_portE(void);

// Initializes Port M (PM7 for alternate function, typically PWM)
void gpio_init_portM(void);

#endif /* GPIO_H_ */
