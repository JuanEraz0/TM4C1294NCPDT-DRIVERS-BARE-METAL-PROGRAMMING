/*
 * adc.h
 *
 *  Created on: May 30, 2025
 *      Author: juanjoseerazopacheco
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <stdio.h>


uint32_t result_temperature;
uint32_t result_current;
uint32_t time;

float voltage_current;
uint16_t current;
float result_current_f;
float rounded;



void adc_init(void);
uint16_t adc_get_temperature_value();
uint16_t adc_get_current_value();






#endif /* ADC_H_ */
