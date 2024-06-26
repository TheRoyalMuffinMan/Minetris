/*
 * File: draw.c
 * Author: Andrew Hoyle
 * Created on April 20, 2024, 2:57 PM
 */

#include "draw.h"

// Bit array of arrays for the letters A through Z
const uint8_t LETTERS[NUM_LETTERS][MATRIX_SIZE] = {
    // A
    {
        0b00011000,
        0b00100100,
        0b00100100,
        0b01111110,
        0b01100110,
        0b11000011,
        0b11000011,
        0b00000000
    },
    // B
    {
        0b11111000,
        0b11001100,
        0b11001100,
        0b11111000,
        0b11001100,
        0b11001100,
        0b11001100,
        0b11111000
    },
    // C
    {
        0b00111100,
        0b01100010,
        0b11000000,
        0b11000000,
        0b11000000,
        0b01100010,
        0b00111100,
        0b00000000
    },
    // D
    {
        0b11111000,
        0b11001100,
        0b11000110,
        0b11000110,
        0b11000110,
        0b11001100,
        0b11111000,
        0b00000000
    },
    // E
    {
        0b11111100,
        0b11000000,
        0b11000000,
        0b11111000,
        0b11000000,
        0b11000000,
        0b11111100,
        0b00000000
    },
    // F
    {
        0b11111100,
        0b11000000,
        0b11000000,
        0b11111000,
        0b11000000,
        0b11000000,
        0b11000000,
        0b00000000
    },
    // G
    {
        0b00111100,
        0b01100010,
        0b11000000,
        0b11000000,
        0b11001110,
        0b01100010,
        0b00111100,
        0b00000000
    },
    // H
    {
        0b11001100,
        0b11001100,
        0b11001100,
        0b11111100,
        0b11001100,
        0b11001100,
        0b11001100,
        0b00000000
    },
    // I
    {
        0b00111100,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00111100,
        0b00000000
    },
    // J
    {
        0b00001110,
        0b00000110,
        0b00000110,
        0b00000110,
        0b11000110,
        0b11000110,
        0b01111100,
        0b00000000
    },
    // K
    {
        0b11001100,
        0b11011000,
        0b11110000,
        0b11110000,
        0b11011000,
        0b11001100,
        0b11000110,
        0b00000000
    },
    // L
    {
        0b11000000,
        0b11000000,
        0b11000000,
        0b11000000,
        0b11000000,
        0b11000000,
        0b11111100,
        0b00000000
    },
    // M
    {
        0b11000110,
        0b11101110,
        0b11111110,
        0b11010110,
        0b11000110,
        0b11000110,
        0b11000110,
        0b00000000
    },
    // N
    {
        0b11000110,
        0b11100110,
        0b11110110,
        0b11011110,
        0b11001110,
        0b11000110,
        0b11000110,
        0b00000000
    },
    // O
    {
        0b00111100,
        0b01100110,
        0b11000011,
        0b11000011,
        0b11000011,
        0b01100110,
        0b00111100,
        0b00000000
    },
    // P
    {
        0b11111000,
        0b11001100,
        0b11001100,
        0b11111000,
        0b11000000,
        0b11000000,
        0b11000000,
        0b00000000
    },
    // Q
    {
        0b00111100,
        0b01100110,
        0b11000011,
        0b11000011,
        0b11011011,
        0b01100110,
        0b00111100,
        0b00000000
    },
    // R
    {
        0b11111000,
        0b11001100,
        0b11001100,
        0b11111000,
        0b11011000,
        0b11001100,
        0b11000110,
        0b00000000
    },
    // S
    {
        0b01111100,
        0b11000010,
        0b11100000,
        0b00111100,
        0b00000010,
        0b11000010,
        0b01111100,
        0b00000000
    },
    // T
    {
        0b11111100,
        0b00110000,
        0b00110000,
        0b00110000,
        0b00110000,
        0b00110000,
        0b00110000,
        0b00000000
    },
    // U
    {
        0b11000110,
        0b11000110,
        0b11000110,
        0b11000110,
        0b11000110,
        0b11000110,
        0b01111100,
        0b00000000
    },
    // V
    {
        0b11000110,
        0b11000110,
        0b11000110,
        0b11000110,
        0b11000110,
        0b01111100,
        0b00111000,
        0b00000000
    },
    // W
    {
        0b11000110,
        0b11000110,
        0b11000110,
        0b11010110,
        0b11010110,
        0b11111110,
        0b01101100,
        0b00000000
    },
    // X
    {
        0b11000110,
        0b11000110,
        0b01101100,
        0b00111000,
        0b00111000,
        0b01101100,
        0b11000110,
        0b00000000
    },
    // Y
    {
        0b11000110,
        0b11000110,
        0b01101100,
        0b00111000,
        0b00110000,
        0b00110000,
        0b00110000,
        0b00000000
    },
    // Z
    {
        0b11111100,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00110000,
        0b01100000,
        0b11111100,
        0b00000000
    }
};

// Bit array of arrays for the digits 0 through 9
const uint8_t DIGITS[NUM_DIGITS][MATRIX_SIZE] = {
    // 0
    {
        0b00111100,
        0b01000010,
        0b10000001,
        0b10000101,
        0b10001001,
        0b10010001,
        0b01000010,
        0b00111100
    },
    // 1
    {
        0b00011000,
        0b00111000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00111100,
        0b00000000
    },
    // 2
    {
        0b01111100,
        0b10000010,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b11111110,
        0b00000000
    },
    // 3
    {
        0b01111100,
        0b10000010,
        0b00000010,
        0b00001100,
        0b00000010,
        0b10000010,
        0b01111100,
        0b00000000
    },
    // 4
    {
        0b00000100,
        0b00001100,
        0b00010100,
        0b00100100,
        0b01000100,
        0b11111110,
        0b00000100,
        0b00000000
    },
    // 5
    {
        0b11111110,
        0b10000000,
        0b10000000,
        0b11111100,
        0b00000010,
        0b00000010,
        0b11111100,
        0b00000000
    },
    // 6
    {
        0b00001110,
        0b00010000,
        0b00100000,
        0b01111100,
        0b10000010,
        0b10000010,
        0b01111100,
        0b00000000
    },
    // 7
    {
        0b11111110,
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b00000000
    },
    // 8
    {
        0b01111100,
        0b10000010,
        0b10000010,
        0b01111100,
        0b10000010,
        0b10000010,
        0b01111100,
        0b00000000
    },
    // 9
    {
        0b01111100,
        0b10000010,
        0b10000010,
        0b01111110,
        0b00000010,
        0b00000010,
        0b01111100,
        0b00000000
    }
};

// Bit array of arrays for the arrows for [Left, Right, Up, Down]
const uint8_t ARROWS[NUM_ARROWS][MATRIX_SIZE] = {
    // Left
    {
        0b00000110,
        0b00001110,
        0b00011110,
        0b00111110,
        0b00011110,
        0b00001110,
        0b00000110,
        0b00000000
    },
    // Right
    {
        0b01100000,
        0b01110000,
        0b01111000,
        0b01111100,
        0b01111000,
        0b01110000,
        0b01100000,
        0b00000000
    },
    // Up
    {
        0b00000000,
        0b00010000,
        0b00111000,
        0b01111100,
        0b11111110,
        0b11111110,
        0b00000000,
        0b00000000
    },
    // Down
    {
        0b00000000,
        0b00000000,
        0b11111110,
        0b11111110,
        0b01111100,
        0b00111000,
        0b00010000,
        0b00000000
    }
    
};

// Bit array for the question mark
const uint8_t QUESTION_MARK[MATRIX_SIZE] = {
    0b00111100,
    0b01000010,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00000000,
    0b00001000,
    0b00000000
};

// Function to draw images using the screen array and top-right position
void draw(point_t topright, image_t type, uint8_t index) {
    const uint8_t *image = NULL;
    switch (type) {
        case LETTER:
            image = LETTERS[index];
            break;
        case ARROW:
            image = ARROWS[index];
            break;
        case DIGIT:
            image = DIGITS[index];
            break;
        case QUESTION:
            image = QUESTION_MARK;
    }
    
    // Draw the image on the screen
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (image[i] & (1 << j)) {
                set_bit(screen, topright.row + j, topright.col + i);
            }
        }
    }
}
