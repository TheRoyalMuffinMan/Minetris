/*
 * File: main.c
 * Author: Andrew Hoyle
 * Created on March 8, 2024, 12:40 PM
 */

#ifndef F_CPU
#define F_CPU 3333333
#endif

#include <avr/io.h>
#include <avr/eeprom.h>
#include <time.h>
#include "globals.h"
#include "communication.h"
#include "display.h"
#include "timer.h"
#include "menu.h"
#include "joystick.h"
#include "tetris.h"

// Main OLED screen
volatile uint8_t screen[BOARD_ROWS][BOARD_COLUMNS];

// Current state
volatile state_t state = MAIN_MENU;
volatile gameplay_t mode = NORMAL;

// Current Player
volatile player_t player = {"AND", 0};

// Current score board
volatile player_t score_board[SCORE_BOARD_SIZE];

// State variables for debouncing the SELECT button
volatile uint16_t select_btn = 0;
volatile bool select_btn_debounced = false;
volatile bool select_pressed = false;

// State variables for direction given by joystick
volatile vertical_t vert_direction = VERTICAL_NOOP;
volatile horizontal_t hori_direction = HORIZONTAL_NOOP;

// Movement value based on the direction given by the joystick
volatile int8_t vertical_move = 0;
volatile int8_t horizontal_move = 0;

// Determines whether we should apply input or not for (RTC)
volatile uint32_t last_fall_time = 0;

// Determines whether game over screen is done or not
volatile uint32_t game_over_time = 0;

// Determines whether leaderboard screen is done or not
volatile uint32_t leaderboard_time = 0;

// General dispatch function that is handles the entire project
void dispatch() {
    // Always read state of the joystick
    read_joystick(&vert_direction, &hori_direction);

    // Convert joystick directions to movement values for the falling piece
    joystick_to_movement(&vertical_move, &horizontal_move);

    // Update falling piece's vertical position based on time
    uint32_t milliseconds = millis();
    if (elapsed_time(last_fall_time, milliseconds) >= FALL_DELAY) {
        vertical_move = MULTIPLIER_SIZE;
        last_fall_time = milliseconds;
    }
    
    // Main Finite State Machine Dispatcher
    switch (state) {
        // Transitions: MODE_SELECTION or LEADERBOARD
        case MAIN_MENU:
            if (main_menu(select_pressed, vert_direction)) {
                 select_pressed = false;
            }
            break;
        // Transitions: NAME_ENTERING
        case MODE_SELECTION:
            if (mode_selection(select_pressed, vert_direction)) {
                select_pressed = false;
            }
            break;
        // Transitions: RUNNING_GAME
        case NAME_ENTERING:
            if (name_entering(select_pressed, vert_direction, hori_direction)) {
                select_pressed = false;
            }
            break;
        // Transitions: GAME_OVER
        case RUNNING_GAME:            
            if (select_pressed) {
                rotate();
                select_pressed = false;
            }

            // Move horizontal and vertical
            move(horizontal_move, 0);
            move(0, vertical_move);
            break;
        // Transitions: LEADERBOARD
        case GAME_OVER:
            if (elapsed_time(game_over_time, milliseconds) >= GAME_OVER_DELAY) {
                state = LEADERBOARD;
                game_over_time = 0;
                leaderboard();
                leaderboard_time = milliseconds;
            }
            select_pressed = false;
            break;
        // Transitions: MAIN_MENU
        case LEADERBOARD:
            if (elapsed_time(leaderboard_time, milliseconds) >= LEARDERBOARD_DELAY) {
                state = MAIN_MENU;
                leaderboard_time = 0;
            }
            select_pressed = false;
            break;
    }
    
    // Reset user input movement operations
    vert_direction = VERTICAL_NOOP;
    hori_direction = HORIZONTAL_NOOP;
    vertical_move = 0, horizontal_move = 0;
}

int main(void){
    srand(time(NULL));
    
    // Initialize peripherals
    init_I2C();
    init_RTC();
    init_btn();
    init_ADC();
    init_display();
    init_LEDs();
    
    // Initialize some defaults
    // Read from EEPROM
    eeprom_read_block((void*) score_board, (const void*) 0, sizeof(score_board));
    // Start score at 0
    display_score(0);
    
    sei();
    while (true) {       
        dispatch();
    }
}
