/* 
 * File: timer.h
 * Author: Andrew Hoyle
 * Comments: Includes and Functions for timer.c
 */
 
#ifndef TIMER_H
#define	TIMER_H

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include "globals.h"
#include "sound.h"

extern volatile state_t state;
extern volatile uint16_t select_btn;
extern volatile bool select_btn_debounced;
extern volatile bool select_pressed;

void init_RTC(void);
uint32_t millis(void);
uint32_t elapsed_time(volatile uint32_t start, volatile uint32_t end);

#endif
