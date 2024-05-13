/*
 * File: display.c
 * Author: Andrew Hoyle
 * Created on April 6, 2024, 6:13 PM
 */

#include "display.h"

// OLED Display constants
const uint8_t OLED_ADDRESS = 0b0111100;
const uint8_t data_mode = 0x40;
const uint8_t command_mode = 0x00;

// Initialization length and commands for OLED
const uint8_t init_length = 20;
const uint8_t init[20] = {  
    0x00,             // Set command mode
    0xAE,             // Set display OFF
    0xA8, 0x3F,       // Set multiplex (HEIGHT-1): 0x1F for 128x32, 0x3F for 128x64
    0xD3, 0x00,       // Set display offset to 0
    0x40,             // Set display start line
    0xA0,             // Set segment re-map
    0xC0,             // Set COM output scan direction
    0xDA, 0x12,       // Set COM pins hardware configuration to sequential
    0x81, 0xCF,       // Set contrast control
    0xA4,             // Disable entire display on
    0xA6,             // Normal display
    0xD5, 0x80,       // Set display clock divide ratio / OSC frequency
    0x8D, 0x14,       // Enable charge pump
    0xAF              // Enable display ON
};

// Reset length and commands for OLED
const uint8_t reset_length = 11;
const uint8_t reset_cursor_commands[] = {
    0x00,             // Set lower nibble
    0x10,             // Set higher nibble
    0x20, 0x00,       // Set horizontal memory addressing mode
    0x21, 0x00, 0x7F, // Set min and max column
    0x22, 0x00, 0x07, // Set min and max page
    0xB0              // Set start page
};

// LED display constants
const uint8_t LED_ADDRESS = 0x71;
const uint8_t CURSOR_MOVE = 0x79;
const uint8_t CURSOR_LEFTMOST = 0x00;

// LEDs constants
const int LEDS_length = 3;
const int LEDS[3] = {PIN5_bm, PIN6_bm, PIN7_bm};

// Sets the display given the initialization buffer above
void init_display(void) {
    start_I2C(OLED_ADDRESS);
    write_I2C(init, init_length);
    stop_I2C();
}

// Sets up initial LEDs
inline void __attribute__((always_inline)) init_LEDs(void) {
    PORTA.DIR |= PIN5_bm | PIN6_bm | PIN7_bm;
    PORTA.OUTCLR |= PIN5_bm | PIN6_bm | PIN7_bm;
}

// Resets the cursor to column 0, page 0 for the display
void reset_cursor(void) {  
    start_I2C(OLED_ADDRESS);
    write_I2C(&command_mode, 1); 
    write_I2C((uint8_t*) reset_cursor_commands, reset_length);
    stop_I2C();
}

// Displays score on LED
void display_score(int score) {
    char data[5];
    uint8_t data_length = 4;

    // Convert the number to a character array
    sprintf(data, "%04d", score);
    
    start_I2C(LED_ADDRESS);
    write_I2C((uint8_t*) &CURSOR_MOVE, 1);
    write_I2C((uint8_t*) &CURSOR_LEFTMOST, 1);
    write_I2C((uint8_t*) data, data_length);
    
    stop_I2C();
}

// Displays the current tetris board on OLED
void display_screen(volatile uint8_t (*screen)[BOARD_COLUMNS]) {
    reset_cursor();   
    start_I2C(OLED_ADDRESS);
    write_I2C(&data_mode, 1);
    for (int r = 0; r < BOARD_ROWS; r++) {
        write_I2C((uint8_t *) screen[r], BOARD_COLUMNS);
    }
    stop_I2C();
}

// Turn on LEDs based on the next piece (1 -> 7)
void display_LEDs(uint8_t next_piece) {
    for (uint8_t pos = 0; pos < LEDS_length; pos++) {
        bool isSet = next_piece & (1 << pos);
        if (isSet) {
            PORTA.OUT |= LEDS[pos];
        } else {
            PORTA.OUT &= ~(LEDS[pos]);
        }
    }
}