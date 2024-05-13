/* 
 * File: operations.h
 * Author: Andrew Hoyle
 * Comments: Includes and Functions for operations.c
 */
  
#ifndef OPERATIONS_H
#define	OPERATIONS_H

#include "globals.h"

int16_t mapped_row(int16_t unmapped_row);
void set_bit(
    volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS], 
    int16_t row, 
    int16_t column
);
void clear_bit(
    volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS], 
    int16_t row, 
    int16_t column
);
bool is_set(
    volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS], 
    int16_t row, 
    int16_t column
);
void clear_screen(volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS]);

#endif
