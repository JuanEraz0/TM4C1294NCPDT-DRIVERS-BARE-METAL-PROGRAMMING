/**
 * main.c
 *
 * Author: juanjoseerazopacheco
 *
 * Description:
 * Embedded application that integrates multiple drivers (GPIO, ADC, Timers, Interrupts, and Stepper Motor)
 * to control the direction and speed of a stepper motor based on sensor and user input.
 *
 * Components used:
 * - Timer 3: input capture to measure RPM via Hall effect sensor.
 * - Timer 5: PWM generation for motor control (speed modulation).
 * - ADC0: analog voltage input (e.g., potentiometer for speed).
 * - Digital inputs (PJ0, PJ1): direction control via buttons.
 * - Interrupts: handle asynchronous events (sensor and switch).
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
int pj0_status, pj1_status, pd0_status; // Button and sensor digital input readings

// ADC readings
uint16_t potentiometerVoltage;  // Stores converted voltage from ADC0
uint32_t delay;                 // Delay time between motor steps (determined by ADC)
uint32_t adc1rawValue;          // Raw value from ADC1
float temperature;              // Unused, reserved for future temperature readings
volatile int i;                 // Unused, avoid keeping unused variables

// Timer 3 (input capture) variables
uint32_t static First = 0;      // Stores the first edge timestamp captured by Timer 3
float frequency;                // Calculated frequency of input signal
float clk = 16e6;               // MCU clock frequency [Hz]
float period;                   // Period between two capture edges
float rpm = 0;                  // Calculated RPM

// Timer 5 (PWM) variables
int dutyCycle = 0x7FF;          // Default PWM duty cycle value

// Interrupt state
volatile uint8_t hall_triggered = 0; // Set by switch ISR and cleared by Hall ISR

// ---------------------- Function Prototypes ----------------------

void drivers_setup(void);          // Initializes all required peripherals
void read_gpio_inputs(void);       // Reads digital inputs from GPIO ports

// Interrupt service routines (ISRs)
void ISR_hall_init(void);          // Configures Hall sensor interrupt (PD0)
void ISR_hall_handler(void);       // Clears Hall interrupt and resets flag
void ISR_switch_init(void);        // Configures user switch interrupt (PN2)
void ISR_switch_handler(void);     // Sets trigger flag on button press
void Timer03AIntHandler(void);     // Timer 3 input capture interrupt handler

// ---------------------- Main Function ----------------------

int main(void)
{
    drivers_setup(); // Initialize all configured peripherals

    while (1) {
        // Read analog voltage and calculate duty cycle for PWM
        potentiometerVoltage = ((adc0_get_value() * 330) / 4095) / 100;
        dutyCycle = adc0_get_value(); // Redundant call to adc0_get_value()

        TIMER5_TBMATCHR_R = dutyCycle; // Update PWM match register to change duty cycle

        // Calculate delay between steps based on ADC reading (inverse relationship)
        delay = 10000 - (dutyCycle * 9500 / 4095);

        // Poll GPIO inputs for button states
        read_gpio_inputs();
        //adc1rawValue = adc1_get_value(); // Read second analog input (not used here)

        // Control motor direction based on button pressed and Hall trigger
        if (hall_triggered && pj0_status == 0) {
            stepper_motor_rotates_right(delay); // Rotate motor right
        } else if (hall_triggered && pj1_status == 0) {
            stepper_motor_rotates_left(delay);  // Rotate motor left
        }
    }

    return 0;
}

// ---------------------- Driver Setup ----------------------

void drivers_setup(void) {
    // Initialize GPIO ports used by system
    gpio_init_portJ();
    gpio_init_portD();
    gpio_init_portL();
    gpio_init_portN();
    gpio_init_portM();
    gpio_init_portB();

    // Configure interrupts and peripherals
    ISR_switch_init();
    ISR_hall_init();
    timer3_init();
    timer5_init();
    adc0_init();
    adc1_init();

}

void read_gpio_inputs(void) {
    // Read digital input states from port J (PJ0 and PJ1)
    pj0_status = (GPIO_PORTJ_AHB_DATA_R & 0x01);
    pj1_status = (GPIO_PORTJ_AHB_DATA_R & 0x02);

    // Read masked interrupt status from port D (PD0)
    pd0_status = GPIO_PORTD_AHB_MIS_R;
}

// ---------------------- ISR: Hall Sensor ----------------------

void ISR_hall_init(void) {
    // Configure PD0 to generate interrupt on rising edge
    GPIO_PORTD_AHB_IM_R = 0x00;       // Disable interrupts during config
    GPIO_PORTD_AHB_IS_R = 0x00;       // Edge-sensitive trigger
    GPIO_PORTD_AHB_IBE_R = 0x00;      // No both-edges
    GPIO_PORTD_AHB_IEV_R = 0x01;      // Rising edge
    GPIO_PORTD_AHB_ICR_R = 0x01;      // Clear any prior interrupts
    GPIO_PORTD_AHB_IM_R = 0x01;       // Enable PD0 interrupt

    NVIC_EN0_R |= 0x00000008;         // Enable interrupt #3 for GPIO Port D
}

void ISR_hall_handler(void) {
    if (GPIO_PORTD_AHB_MIS_R & 0x01) {
        GPIO_PORTD_AHB_ICR_R = 0x01;  // Clear interrupt flag
        hall_triggered = 0;           // Reset trigger flag
    }
}

// ---------------------- ISR: Switch (e.g., user button) ----------------------

void ISR_switch_init(void) {
    // Configure PN2 for falling edge interrupt (button press)
    GPIO_PORTN_IM_R = 0x00;        // Disable interrupts during config
    GPIO_PORTN_IS_R = 0x00;        // Edge-sensitive
    GPIO_PORTN_IBE_R = 0x00;       // No both-edges
    GPIO_PORTN_IEV_R = 0x00;       // Falling edge trigger
    GPIO_PORTN_ICR_R = 0x04;       // Clear any prior interrupt on PN2
    GPIO_PORTN_IM_R = 0x04;        // Enable PN2 interrupt

    NVIC_EN2_R |= 0x00000200;      // Enable interrupt #66 for GPIO Port N
}

void ISR_switch_handler(void) {
    if (GPIO_PORTN_MIS_R & 0x04) {
        GPIO_PORTN_ICR_R = 0x04;   // Clear interrupt flag
        hall_triggered = 1;        // Set trigger flag
    }
}

// ---------------------- Timer 3A Interrupt Handler (Capture) ----------------------

void Timer03AIntHandler(void) {
    TIMER3_ICR_R = 0x00000004;     // Clear capture interrupt flag (CAECINT)

    // Capture the first edge
    if (First == 0) {
        First = TIMER3_TAR_R;      // Store first timestamp
        return;
    }

    // Compute time difference between two rising edges (capture period)
    if (First > TIMER3_TAR_R)
        period = (float)((First - TIMER3_TAR_R) & 0x00FFFFFF); // Normal wrap-around case
    else
        period = (float)((0x00FFFFFF - TIMER3_TAR_R) + First); // Overflow case

    period = period / clk;         // Convert from clock ticks to seconds
    frequency = (1 / period);      // Compute signal frequency
    rpm = frequency * 60.0f;        // Convert to revolutions per minute

    First = TIMER3_TAR_R;          // Prepare for next edge capture
}
