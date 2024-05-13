/* 
 * File: display.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives, and Functions for display.c
 */
 
#ifndef DISPLAY_H
#define	DISPLAY_H

#include <stdio.h>
#include "globals.h"
#include "communication.h"

void init_display(void);
void init_LEDs(void);
void reset_cursor(void);
void display_score(int score);
void display_screen(volatile uint8_t (*screen)[BOARD_COLUMNS]);
void display_LEDs(uint8_t next_piece);

#endif
