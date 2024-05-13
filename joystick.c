/*
 * File: joystick.c
 * Author: Andrew Hoyle
 * Created on April 13, 2024, 12:41 AM
 */

#include "joystick.h"

ISR(PORTA_PORT_vect) {
    // Select Pressed
    if (PORTA.INTFLAGS & PIN4_bm) {
        if (select_btn_debounced) {
            select_pressed = true;
            select_btn_debounced = false;
        }
        PORTA.INTFLAGS &= PIN4_bm;
    }
}

// Initialization for all the buttons
void init_btn(void) {
    // Set buttons for (Select) as input
    PORTA.DIRCLR = PIN4_bm;
    
    // Set pull resistors for all button pins
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm;
    
    // Set interrupt handlers for all button pins
    PORTA.PIN4CTRL |= PORT_ISC_RISING_gc;
}

// Initialize ADC
void init_ADC(void) {
    // Set pin for input (X and Y)
    PORTD.DIRCLR = PIN6_bm | PIN1_bm;
    
    // Disable digital input buffer and pull-up resistor (X and Y)
    PORTD.PIN6CTRL &= ~PORT_ISC_gm;
    PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
    PORTD.PIN1CTRL &= ~PORT_ISC_gm;
    PORTD.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN1CTRL &= ~PORT_PULLUPEN_bm;
    
    // Setup pre-scalers, reference value, and resolution
    ADC0.CTRLC = ADC_PRESC_DIV4_gc;
    ADC0.CTRLC |= ADC_REFSEL_VDDREF_gc;
    ADC0.CTRLA |= ADC_RESSEL_10BIT_gc;
    
    // Select ADC channel (PIN 6 -> Start reading X)
    ADC0.MUXPOS = X_PIN;
    
    // Enable the ADC
    ADC0.CTRLA |= ADC_ENABLE_bm;
    // Enable FreeRun mode
    ADC0.CTRLA |= ADC_FREERUN_bm;
    // Start Conversions
    ADC0.COMMAND = ADC_STCONV_bm;

}

// Read from the joystick for either X or Y
uint16_t read_ADC(void) {
    // Clear the interrupt flag by writing 1
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    return ADC0.RES;
}

// Determine if conversion is done or not for free-run mode
bool conversion_done(void){
    return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}

// Determines horizontal direction given X value
void handle_xaxis(uint16_t x_reading, volatile horizontal_t* hdir) {
    if (x_reading > UPPER) {
        *hdir = RIGHT;
    } else if (x_reading < LOWER) {
        *hdir = LEFT;
    } else {
        *hdir = HORIZONTAL_NOOP;
    }
}

// Determines vertical direction given Y value
void handle_yaxis(uint16_t y_reading, volatile vertical_t* vdir) {
    if (y_reading > UPPER) {
        *vdir = UP;
    } else if (y_reading < LOWER) {
        *vdir = DOWN;
    } else {
        *vdir = VERTICAL_NOOP;
    }
}

// Perform a ADC read from the joystick for X and Y values
void read_joystick(volatile vertical_t* vdir, volatile horizontal_t* hdir) {
    volatile uint16_t x_reading = 0, y_reading = 0;
    if (conversion_done()) {
        if (ADC0.MUXPOS == X_PIN) {
            x_reading = read_ADC();
            handle_xaxis(x_reading, hdir);
            ADC0.MUXPOS = Y_PIN; 
        } else {
            y_reading = read_ADC();
            handle_yaxis(y_reading, vdir);
            ADC0.MUXPOS = X_PIN;
        }
    }
}

// Convert the determine directions to movement values
// Note: I decided to favor horizontal movement over vertical and 
// disallow the user to move diagonally as you couldn't in the original tetris
void joystick_to_movement(volatile int8_t* vertical, volatile int8_t* horizontal) {
    if (hori_direction == LEFT) *horizontal = MULTIPLIER_SIZE;
    else if (hori_direction == RIGHT) *horizontal = -MULTIPLIER_SIZE;
    else if (vert_direction == DOWN) *vertical = max(*vertical, MULTIPLIER_SIZE);
}
