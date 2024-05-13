/* 
 * File: draw.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives, Structs, Externs, and Functions for draw.c
 */

#ifndef DRAW_H
#define	DRAW_H

#include "globals.h"
#include "tetris.h"

#define NUM_LETTERS 26
#define NUM_DIGITS 10
#define NUM_ARROWS 4
#define MATRIX_SIZE 8

typedef enum {
    LETTER,
    ARROW,
    DIGIT,
    QUESTION
} image_t;

extern volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS];

void draw(point_t topright, image_t type, uint8_t index);

#endif
