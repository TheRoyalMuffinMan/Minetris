/* 
 * File: tetris.h
 * Author: Andrew Hoyle
 * Comments: Includes, Enums, Structs and Functions for tetris.c
 */

#ifndef TETRIS_H
#define	TETRIS_H

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "operations.h"
#include "display.h"
#include "timer.h"
#include "sound.h"
#include "menu.h"

// Piece generation directives
#define NUMBER_OF_PIECES 7
#define PIECE_SIZE 4
#define ROTATIONS 4
#define TRANSFORM_PIECE_SIZE 256

// Line clearing directives
#define FULL_ROW 0xFF
#define EMPTY_ROW 0x00
#define ALL_ROWS 64

typedef enum {
    ROTATION_0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270
} rotation_t;

typedef enum {
    I_PIECE, 
    J_PIECE, 
    L_PIECE, 
    O_PIECE, 
    S_PIECE, 
    T_PIECE, 
    Z_PIECE
} shape_t;

typedef struct {
   shape_t shape;
   rotation_t rotation;
   point_t rotations[ROTATIONS][PIECE_SIZE];
   point_t location[TRANSFORM_PIECE_SIZE];
} tetromino_t;

typedef struct {
    tetromino_t piece;
    volatile uint8_t (*board)[BOARD_COLUMNS];
} tetris_t;

extern volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS];
extern volatile state_t state;
extern volatile gameplay_t mode;
extern volatile player_t player;
extern volatile player_t score_board[SCORE_BOARD_SIZE];
extern volatile uint32_t game_over_time;
extern volatile uint8_t note;
extern volatile uint8_t last_note;

void incremental_randomizer(void);
int compare_scores(const void *a, const void *b);
void try_to_save(void);
void make_piece(
    point_t transform_location[TRANSFORM_PIECE_SIZE], 
    point_t outline[PIECE_SIZE]
);
void random_piece(void);
void update_board(void);
void update_piece(void);
void score_player(uint8_t cleared_lines);
void move_block(
    point_t old_topright, 
    point_t new_topright, 
    int16_t clear_mask, 
    int16_t set_mask
);
bool cell(int16_t row, int16_t start_column, int16_t mask, int16_t condition);
void cascade_down(int16_t start_column);
void clear_line(int16_t start_column, int16_t end_column);
bool check_clear(int16_t start_column, int16_t end_column);
void cascade_lines(void);
void normal_lines(void);
bool in_bounds(point_t position);
bool is_colliding(point_t position);
void rotate(void);
void move(int16_t row_dir, int16_t col_dir);
void start_game(void);

#endif
