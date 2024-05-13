/* 
 * File: menu.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives, Externs, and Functions for menu.c
 */

#ifndef MENU_H
#define	MENU_H

#include "globals.h"
#include "display.h"
#include "timer.h"
#include "draw.h"
#include "tetris.h"

#define CHARACTER_OFFSET 'A'
#define DIGIT_OFFSET '0'
#define THREE_LETTERS 3
#define FOUR_LETTERS 4
#define FIVE_LETTERS 5
#define SIX_LETTERS 6
#define SEVEN_LETTERS 7
#define CURSORS_LENGTH 2
#define DESCR_LENGTH 13
#define RIGHT_ARROW 1
#define UP_ARROW 2
#define DOWN_ARROW 3

extern volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS];
extern volatile state_t state;
extern volatile gameplay_t mode;
extern volatile player_t player;
extern volatile uint32_t leaderboard_time;
extern volatile player_t score_board[SCORE_BOARD_SIZE];

bool main_menu(volatile bool select_pressed, vertical_t vert);
bool mode_selection(volatile bool select_pressed, vertical_t vert);
bool name_entering(volatile bool select_pressed, vertical_t vert, horizontal_t hori);
void game_over(void);
void get_display_string(char *string, int index);
void leaderboard(void);

#endif
