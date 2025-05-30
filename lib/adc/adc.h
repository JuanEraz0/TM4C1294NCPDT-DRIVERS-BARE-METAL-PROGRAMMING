/*
 * adc.h
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 */

#ifndef LIB_ADC_ADC_H_
#define LIB_ADC_ADC_H_

#include <stdint.h>
#include <stdio.h>


uint32_t result_temperature;
uint32_t result_current;
uint32_t time;

float voltage_current;
float current;
float result_current_f;
float rounded;



void adc_init(void);
uint16_t adc_get_value();





#endif /* LIB_ADC_ADC_H_ */
