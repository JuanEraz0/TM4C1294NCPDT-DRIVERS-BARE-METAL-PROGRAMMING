/*
 * adc.h
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 *
 *  Brief:
 *  This header provides declarations for ADC initialization and
 *  reading analog values from ADC0 and ADC1 on the TM4C1294NCPDT microcontroller.
 *  It is part of a modular low-level driver set for embedded development.
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stdio.h>

// Initializes ADC0 module
void adc0_init(void);

// Reads value from ADC0 (e.g., for analog input on PE4)
uint16_t adc0_get_value(void);

// Reads value from ADC1 (function prototype for future expansion)
uint16_t adc1_get_value(void);

#endif /* ADC_H_ */
