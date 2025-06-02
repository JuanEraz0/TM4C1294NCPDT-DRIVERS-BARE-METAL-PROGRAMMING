/*
 * adc.h
 *
 * Created on: May 30, 2025
 * Author: juanjoseerazopacheco
 *
 * Description:
 * Header file for ADC driver functions on the TM4C1294NCPDT microcontroller.
 *
 * Provides initialization and read interfaces for ADC0 and ADC1 modules.
 * Designed for modular use in low-level embedded system applications
 * where analog signal acquisition is required (e.g., temperature sensors, potentiometers).
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stdio.h>

// ---------------------- Function Declarations ----------------------

/**
 * @brief Initializes ADC0 using sequencer 3.
 *        PE4 (AIN9) is typically used as the analog input.
 */
void adc0_init(void);

/**
 * @brief Initializes ADC1 using sequencer 2.
 *        PB4 (AIN10) is the default analog input for this setup.
 */
void adc1_init(void);

/**
 * @brief Triggers a conversion on ADC0 and returns a 12-bit result.
 * @return 12-bit unsigned integer from analog input (PE4 / AIN9)
 */
uint16_t adc0_get_value(void);

/**
 * @brief Triggers a conversion on ADC1 and returns a 12-bit result.
 * @return 12-bit unsigned integer from analog input (PB4 / AIN10)
 */
uint16_t adc1_get_value(void);

#endif /* ADC_H_ */
