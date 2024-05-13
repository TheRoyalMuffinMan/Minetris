/* 
 * File: globals.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives, Enums, and Structs for all files
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

// Screen size
#define SCREEN_ROWS 64
#define SCREEN_COLUMNS 128

// Board size
#define BOARD_ROWS 8
#define BOARD_COLUMNS 128

// Size of each cell n x n
#define MULTIPLIER_SIZE 8

// Delays
#define FALL_DELAY 300
#define GAME_OVER_DELAY 1500
#define LEARDERBOARD_DELAY 3000

// Enterable name size
#define NAME_LENGTH 3

// Size of the displayable scoreboard
#define SCORE_BOARD_SIZE 10

#undef max
#define max(x, y) (((x) > (y)) ? (x) : (y))

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    UP,
    DOWN,
    VERTICAL_NOOP
} vertical_t;

typedef enum {
    LEFT,
    RIGHT,
    HORIZONTAL_NOOP
} horizontal_t;

typedef enum {
    X_AXIS,
    Y_AXIS
} axis_t;

typedef enum {
    MAIN_MENU,
    MODE_SELECTION,
    NAME_ENTERING,    
    RUNNING_GAME,
    GAME_OVER,
    LEADERBOARD
} state_t;

typedef enum {
    NORMAL,
    CASCADE
} gameplay_t;

typedef struct {
    uint8_t row;
    uint8_t col;
} point_t;

typedef struct {
    char name[NAME_LENGTH];
    int score;
} player_t;

#endif
