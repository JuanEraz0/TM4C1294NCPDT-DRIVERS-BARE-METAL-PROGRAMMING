/**
 * main.c
 *
 * Author: juanjoseerazopacheco
 *
 * Brief:
 * This program integrates multiple drivers (GPIO, ADC, Timers, Interrupts, and Stepper Motor)
 * to control the rotation of a stepper motor based on input from a Hall effect sensor.
 * It uses Timer 3 in capture mode to measure the input frequency (RPM), Timer 5 for PWM control,
 * and ADC0 for analog input (e.g., a potentiometer).
 * Digital inputs and interrupts allow responsive behavior to buttons and sensors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "inc/tm4c1294ncpdt.h"
#include "lib/gpio/gpio.h"
#include "lib/adc/adc.h"
#include "lib/timer/timer.h"
#include "lib/stepper_motor/stepper_motor.h"

// ---------------------- Global Variables ----------------------

// Digital input statuses
int pj0_status, pj1_status, pd0_status;

// ADC readings
uint32_t potentiometer;
uint16_t potentiometerVoltage;

// Timer 3 (input capture) variables
uint32_t static First = 0;    // First captured edge (Timer A)
uint8_t frequency;
float clk = 16e6;             // MCU clock frequency [Hz]
float period;
float rpm = 0;

// Timer 5 (PWM) variables
int dutyCycle = 0x7FF;        // Default duty cycle

// Interrupt state
volatile uint8_t hall_triggered = 0; // Used to signal main loop

// ---------------------- Function Prototypes ----------------------

void drivers_setup(void);
void read_gpio_inputs(void);

// Interrupt service routines
void ISR_hall_init(void);
void ISR_hall_handler(void);
void ISR_switch_init(void);
void ISR_switch_handler(void);
void Timer03AIntHandler(void);

// ---------------------- Main Function ----------------------

int main(void)
{
    drivers_setup(); // Initialize all configured peripherals

    while (1) {
        // Read analog voltage and map to duty cycle
        potentiometerVoltage = ((adc0_get_value() * 330) / 4095) / 100;
        dutyCycle = adc0_get_value();
        TIMER5_TBMATCHR_R = dutyCycle;

        // Polling GPIO inputs
        read_gpio_inputs();

        // If Hall sensor was triggered, rotate motor
        if (hall_triggered) {
            stepper_motor_rotates_right();
        }
    }

    return 0;
}

// ---------------------- Driver Setup ----------------------

void drivers_setup(void) {
    gpio_init_portJ();
    gpio_init_portD();
    gpio_init_portL();
    gpio_init_portN();
    gpio_init_portM();
    ISR_switch_init();
    ISR_hall_init();
    adc0_init();
    timer3_init();
    timer5_init();
}

void read_gpio_inputs(void) {
    pj0_status = (GPIO_PORTJ_AHB_DATA_R & 0x01);
    pj1_status = (GPIO_PORTJ_AHB_DATA_R & 0x02);
    // Optional: digital input or capture event
    pd0_status = GPIO_PORTD_AHB_MIS_R;
}

// ---------------------- ISR: Hall Sensor ----------------------

void ISR_hall_init(void) {
    GPIO_PORTD_AHB_IM_R = 0x00;       // Disable interrupts on PD0
    GPIO_PORTD_AHB_IS_R = 0x00;       // Edge-sensitive
    GPIO_PORTD_AHB_IBE_R = 0x00;      // Disable both-edge
    GPIO_PORTD_AHB_IEV_R = 0x01;      // Rising edge interrupt
    GPIO_PORTD_AHB_ICR_R = 0x01;      // Clear flag
    GPIO_PORTD_AHB_IM_R = 0x01;       // Enable interrupt on PD0

    NVIC_EN0_R |= 0x00000008;         // Enable interrupt 3 (GPIO Port D)
}

void ISR_hall_handler(void) {
    if (GPIO_PORTD_AHB_MIS_R & 0x01) {
        GPIO_PORTD_AHB_ICR_R = 0x01;  // Clear interrupt flag
        hall_triggered = 0;           // Signal to main loop (reset)
    }
}

// ---------------------- ISR: Switch (e.g., user button) ----------------------

void ISR_switch_init(void) {
    GPIO_PORTN_IM_R = 0x00;
    GPIO_PORTN_IS_R = 0x00;        // Edge-sensitive
    GPIO_PORTN_IBE_R = 0x00;
    GPIO_PORTN_IEV_R = 0x00;       // Falling edge interrupt
    GPIO_PORTN_ICR_R = 0x04;       // Clear flag on PN2
    GPIO_PORTN_IM_R = 0x04;        // Enable interrupt on PN2

    NVIC_EN2_R |= 0x00000200;      // Enable interrupt 66 (GPIO Port N)
}

void ISR_switch_handler(void) {
    if (GPIO_PORTN_MIS_R & 0x04) {
        GPIO_PORTN_ICR_R = 0x04;   // Clear interrupt flag
        hall_triggered = 1;        // Signal to main loop (trigger motor)
    }
}

// ---------------------- Timer 3A Interrupt Handler (Capture) ----------------------

void Timer03AIntHandler(void) {
    TIMER3_ICR_R = 0x00000004;     // Clear capture flag

    // First edge capture logic
    if (First == 0) {
        First = TIMER3_TAR_R;
        return;
    }

    // Compute period between two captures
    if (First > TIMER3_TAR_R)
        period = (float)((First - TIMER3_TAR_R) & 0x00FFFFFF); // Normal case
    else
        period = (float)((0x00FFFFFF - TIMER3_TAR_R) + First); // Timer overflow case

    period = period / clk;             // Convert ticks to seconds
    frequency = (uint8_t)(1 / period); // Frequency in Hz
    rpm = frequency * 60.0f;           // Convert to RPM

    First = TIMER3_TAR_R;              // Update for next cycle
}
