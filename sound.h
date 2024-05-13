/* 
 * File: sound.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives and Functions for sound.c
 */
 
#ifndef SOUND_H
#define	SOUND_H

#include <avr/io.h>
#include "globals.h"
#include "timer.h"

#define NOTES_SIZE 40

void start_FGM(int note);
void stop_FGM(void);
void sound(
    volatile uint32_t* note_time, 
    uint32_t milliseconds, 
    volatile uint8_t* note, 
    volatile uint8_t* last_note
);

#endif

