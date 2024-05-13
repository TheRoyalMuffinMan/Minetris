/*
 * File: menu.c
 * Author: Andrew Hoyle
 * Created on April 20, 2024, 2:56 PM
 */

#include "menu.h"

// Constants for main menu
const char LOGO_BUFFER[SIX_LETTERS] = {'T', 'E', 'T', 'R', 'I', 'S'};
const point_t LOGO_POINTS[SIX_LETTERS] = {
    {48, 30}, {40, 30}, {32, 30}, 
    {24, 30}, {16, 30}, {8, 30}
};
const char START_BUFFER[FIVE_LETTERS] = {'S', 'T', 'A', 'R', 'T'};
const point_t START_POINTS[FIVE_LETTERS] = {
    {48, 60}, {40, 60}, {32, 60}, {24, 60}, {16, 60}
};
const char BOARD_BUFFER[FIVE_LETTERS] = {'B', 'O', 'A', 'R', 'D'};
const point_t BOARD_POINTS[FIVE_LETTERS] = {
    {48, 80}, {40, 80}, {32, 80}, {24, 80}, {16, 80}
};


// Constants for mode selection
const char MODE_BUFFER[FOUR_LETTERS] = {'M', 'O', 'D', 'E'};
const point_t MODE_POINTS[FOUR_LETTERS] = {
    {48, 30}, {40, 30}, {32, 30}, {24, 30}
};
const char NORMAL_BUFFER[SIX_LETTERS] = {'N', 'O', 'R', 'M', 'A', 'L'};
const point_t NORMAL_POINTS[SIX_LETTERS] = {
    {48, 60}, {40, 60}, {32, 60}, {24, 60}, {16, 60}, {8, 60} 
};
const char CASCADE_BUFFER[SEVEN_LETTERS] = {'C', 'A', 'S', 'C', 'A', 'D', 'E'};
const point_t CASCADE_POINTS[SEVEN_LETTERS] = {
    {48, 80}, {40, 80}, {32, 80}, 
    {24, 80}, {16, 80}, {8, 80}, {0, 80}
};

// Constants for name selection
const char NAME_BUFFER[FOUR_LETTERS] = {'N', 'A', 'M', 'E'};
const point_t NAME_POINTS[FOUR_LETTERS] = {
    {48, 30}, {40, 30}, {32, 30}, {24, 30}
};
const point_t USERNAME_POINTS[NAME_LENGTH] = {
    {48, 60}, {32, 60}, {16, 60}
};
const char DESCR_BUFFER[DESCR_LENGTH] = {
    'P', 'R', 'E', 'S', 'S',
    'T', 'O',
    'F', 'I', 'N', 'I', 'S', 'H'
};
const point_t DESCR_POINTS[DESCR_LENGTH] = {
    {48, 90}, {40, 90}, {32, 90}, {24, 90}, {16, 90},
    {48, 100}, {40, 100},
    {48, 110}, {40, 110}, {32, 110}, {24, 110}, {16, 110}, {8, 110},
};

// Constants for game over
const char GAME_BUFFER[FOUR_LETTERS] = {'G', 'A', 'M', 'E'};
const point_t GAME_POINTS[FOUR_LETTERS] = {
    {48, 40}, {40, 40}, {32, 40}, {24, 40}
};
const char OVER_BUFFER[FOUR_LETTERS] = {'O', 'V', 'E', 'R'};
const point_t OVER_POINTS[FOUR_LETTERS] = {
    {48, 70}, {40, 70}, {32, 70}, {24, 70}
};

// Constants for leaderboard
const char SCORES_BUFFER[SIX_LETTERS] = {'S', 'C', 'O', 'R', 'E', 'S'};
const point_t SCORES_POINTS[SIX_LETTERS] = {
    {48, 0}, {40, 0}, {32, 0}, {24, 0}, {16, 0}, {8, 0} 
};
const point_t BLUEPRINT_POINTS[SEVEN_LETTERS] = {
    {56, 24}, {48, 24}, {40, 24}, 
    {24, 24}, {16, 24}, {8, 24}, {0, 24} 
};

// Used for indexing cursors and letters for name selection
uint8_t cursor_index = 0;

// Manages the main menu screen
bool main_menu(volatile bool select_pressed, vertical_t vert) {
    // If button is pressed, transition to the next state
    if (select_pressed) {
        if (cursor_index == 0) {
            state = MODE_SELECTION;
        } else {
            state = LEADERBOARD;
            leaderboard_time = millis();
            leaderboard();
        }
        cursor_index = 0;
        return true;
    }
    
    clear_screen(screen);
    
    // Draw the title
    for (int i = 0; i < SIX_LETTERS; i++) {
        draw(LOGO_POINTS[i], LETTER, LOGO_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Setup cursors for two options below
    point_t cursors[CURSORS_LENGTH] = {
        START_POINTS[0], BOARD_POINTS[0]
    };
    cursors[0].row += 8, cursors[1].row += 8;
    
    // Draw the start option
    for (int i = 0; i < FIVE_LETTERS; i++) {
        draw(START_POINTS[i], LETTER, START_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw the board option
    for (int i = 0; i < FIVE_LETTERS; i++) {
        draw(BOARD_POINTS[i], LETTER, BOARD_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw arrow between each option
    if (vert == UP) {
        cursor_index = 0;
    } else if (vert == DOWN) {
        cursor_index = 1;
    }
    draw(cursors[cursor_index], ARROW, RIGHT_ARROW);
    
    display_screen(screen);
    
    return false;
}

// Manages the mode selection screen
bool mode_selection(volatile bool select_pressed, vertical_t vert) {
    // If button is pressed, transition to the next state
    if (select_pressed) {
        if (cursor_index == 0) {
            mode = NORMAL;
        } else {
            mode = CASCADE;
        }
        state = NAME_ENTERING;
        cursor_index = 0;
        return true;
    }
    
    clear_screen(screen);
    
    // Draw the title
    for (int i = 0; i < FOUR_LETTERS; i++) {
        draw(MODE_POINTS[i], LETTER, MODE_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Setup cursors for two options below
    point_t cursors[CURSORS_LENGTH] = {
        NORMAL_POINTS[0], CASCADE_POINTS[0]
    };
    cursors[0].row += 8, cursors[1].row += 8;
    
    // Draw the normal option
    for (int i = 0; i < SIX_LETTERS; i++) {
        draw(NORMAL_POINTS[i], LETTER, NORMAL_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw the cascade option
    for (int i = 0; i < SEVEN_LETTERS; i++) {
        draw(CASCADE_POINTS[i], LETTER, CASCADE_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw arrow between each option
    if (vert == UP) {
        cursor_index = 0;
    } else if (vert == DOWN) {
        cursor_index = 1;
    }
    draw(cursors[cursor_index], ARROW, RIGHT_ARROW);
    
    display_screen(screen);
    
    return false;
}

// Manages the name entering screen
bool name_entering(volatile bool select_pressed, vertical_t vert, horizontal_t hori) {
    // If button is pressed, transition to the next state
    if (select_pressed) {
        start_game();
        state = RUNNING_GAME;
        cursor_index = 0;
        return true;
    }
    
    clear_screen(screen);
    
    // Change character
    if (vert != VERTICAL_NOOP) {
        if (vert == UP) {
            if (player.name[cursor_index] == 'Z') {
                player.name[cursor_index] = 'A';
            } else {
                player.name[cursor_index]++;
            }
        } else {
            if (player.name[cursor_index] == 'A') {
                player.name[cursor_index] = 'Z';
            } else {
                player.name[cursor_index]--;
            }
        }
    // Change position
    } else {
        if (hori == RIGHT) {
            if (cursor_index == 2) {
                cursor_index = cursor_index;
            } else {
                cursor_index = cursor_index + 1;
            }
        } else if (hori == LEFT) {
            if (cursor_index == 0) {
                cursor_index = cursor_index;
            } else {
                cursor_index = cursor_index - 1;
            }
        }
    }
    
    // Draw the title
    for (int i = 0; i < FOUR_LETTERS; i++) {
        draw(NAME_POINTS[i], LETTER, NAME_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw the name selected currently
    for (int i = 0; i < NAME_LENGTH; i++) {
        draw(USERNAME_POINTS[i], LETTER, player.name[i] - CHARACTER_OFFSET);
    }
    
    // Draw description to help user
    for (int i = 0; i < DESCR_LENGTH; i++) {
        draw(DESCR_POINTS[i], LETTER, DESCR_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Set Up and Down Cursor positions
    point_t cursors[CURSORS_LENGTH] = {
        USERNAME_POINTS[cursor_index], USERNAME_POINTS[cursor_index]
    };
    cursors[0].col += 8, cursors[1].col -= 8;
    draw(cursors[0], ARROW, DOWN_ARROW);
    draw(cursors[1], ARROW, UP_ARROW);
    
    display_screen(screen);
    
    return false;
}

// Manages the game over screen
void game_over(void) {   
    clear_screen(screen);
    
    // Draw the GAME word
    for (int i = 0; i < FOUR_LETTERS; i++) {
        draw(GAME_POINTS[i], LETTER, GAME_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw the OVER word
    for (int i = 0; i < FOUR_LETTERS; i++) {
        draw(OVER_POINTS[i], LETTER, OVER_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    display_screen(screen);    
}

// Helper function for getting display string from global score board array
void get_display_string(char *string, int index) {
    char score_string[5];
    sprintf(score_string, "%04d", score_board[index].score);
    for (int i = 0; i < NAME_LENGTH; i++) {
        string[i] = score_board[index].name[i];
    }
    for (int i = 3, j = 0; i < NAME_LENGTH + 4; i++, j++) {
        string[i] = score_string[j];
    }
}

// Manages the leaderboard screen
void leaderboard(void) {
    clear_screen(screen);
    
    // Draw the title
    for (int i = 0; i < SIX_LETTERS; i++) {
        draw(SCORES_POINTS[i], LETTER, SCORES_BUFFER[i] - CHARACTER_OFFSET);
    }
    
    // Draw letters and digits for the scoreboard as follows:
    // ??? XXXX
    // where ??? = letter and X = digit
    for (int i = 0; i < SCORE_BOARD_SIZE; i++) {
        char string[NAME_LENGTH + 4];
        get_display_string(string, i);
        
        // Draw the name first
        for (int k = 0; k < 3; k++) {
            point_t curr = BLUEPRINT_POINTS[k];
            curr.col = curr.col + 10 * i;
            if (string[k] == '?') {
                draw(curr, QUESTION, 0);
            } else {
                draw(curr, LETTER, string[k] - CHARACTER_OFFSET);
            }
        }
        
        // Draw the score next
        for (int k = 3; k < SEVEN_LETTERS; k++) {
            point_t curr = BLUEPRINT_POINTS[k];
            curr.col = curr.col + 10 * i;
            draw(curr, DIGIT, string[k] - DIGIT_OFFSET);
        }
    }
    
    display_screen(screen); 
}
