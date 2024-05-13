/* 
 * File: joystick.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives, Externs, and Functions for joystick.c
 */
 
#ifndef JOYSTICK_H
#define	JOYSTICK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "globals.h"

#define X_PIN ADC_MUXPOS_AIN6_gc
#define Y_PIN ADC_MUXPOS_AIN1_gc
#define UPPER 611
#define LOWER 411

extern volatile vertical_t vert_direction;
extern volatile horizontal_t hori_direction;
extern volatile uint16_t select_btn;
extern volatile bool select_btn_debounced;
extern volatile bool select_pressed;

void init_btn(void);
void init_ADC(void);
uint16_t read_ADC(void);
bool conversion_done(void);
void handle_xaxis(uint16_t x_reading, volatile horizontal_t* hdir);
void handle_yaxis(uint16_t y_reading, volatile vertical_t* vdir);
void read_joystick(volatile vertical_t* vdir, volatile horizontal_t* hdir);
void joystick_to_movement(volatile int8_t* vertical, volatile int8_t* horizontal);

#endif

