/*
 * File: operations.c
 * Author: Andrew Hoyle
 * Created on April 20, 2024, 3:27 PM
 */

#include "operations.h"

// Converts unmapped row index (0 -> 63) to bitmap index (0 -> 7)
int16_t mapped_row(int16_t unmapped_row) {
    return unmapped_row / 8;
}

// Sets the bit in the grid-based bitmap using the mapped row (0 -> 7) 
// and unmapped column (0 -> 127)
void set_bit(
    volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS], 
    int16_t row, 
    int16_t column
) {
    int16_t bit_row = mapped_row(row);   
    int16_t bit_position = row - bit_row * 8;
    screen[bit_row][column] |= (1 << bit_position);
}

// Clears the bit in the grid-based bitmap using the mapped row (0 -> 7) 
// and unmapped column (0 -> 127)
void clear_bit(
    volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS], 
    int16_t row, 
    int16_t column
) {
    int16_t bit_row = mapped_row(row);   
    int16_t bit_position = row - bit_row * 8;
    screen[bit_row][column] &= ~(1 << bit_position);
}

// Checks if the bit is set the grid-based bitmap using the mapped row (0 -> 7) 
// and unmapped column (0 -> 127)
bool is_set(
    volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS], 
    int16_t row, 
    int16_t column
) {
    int16_t bit_row = mapped_row(row);   
    int16_t bit_position = row - bit_row * 8;
    return screen[bit_row][column] & (1 << bit_position);
}

// Reset the screen
void clear_screen(volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS]) {
    for (int16_t r = 0; r < BOARD_ROWS; r++) {
        for (int16_t c = 0; c < BOARD_COLUMNS; c++) {
            screen[r][c] = 0x00;
        }
    }
}