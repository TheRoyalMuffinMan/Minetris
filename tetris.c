/*
 * File: tetris.c
 * Author: Andrew Hoyle
 * Created on April 6, 2024, 4:16 PM
 */

#include "tetris.h"

// Maintain's tetris game state
tetris_t game;

// Handles bag randomizer
const shape_t INITIAL_BAG[NUMBER_OF_PIECES] = {
    I_PIECE, J_PIECE, 
    L_PIECE, O_PIECE, 
    S_PIECE, T_PIECE, 
    Z_PIECE
};
shape_t new_bag[NUMBER_OF_PIECES];
shape_t current_bag[NUMBER_OF_PIECES];
uint8_t bag_position = 0;

// Initial spawn locations for each tetromino
const point_t PIECES[NUMBER_OF_PIECES][ROTATIONS][PIECE_SIZE] = {
    {   // I Block
        {{40, 8}, {32, 8}, {24, 8}, {16, 8}},       // 0 degrees
                          {{24, 0}, 
                           {24, 8}, 
                           {24, 16}, 
                           {24, 24}},               // 90 degrees
        {{40, 16}, {32, 16}, {24, 16}, {16, 16}},   // 180 degrees
                  {{32, 0}, 
                   {32, 8}, 
                   {32, 16}, 
                   {32, 24}}                        // 270 degrees
    }, 
    {   // J Block
        {{40, 0},
         {40, 8}, {32, 8}, {24, 8}},                // 0 degrees
                 {{32, 0}, {24, 0},
                  {32, 8},
                  {32, 16}},                        // 90 degrees
        {{40, 8}, {32, 8}, {24, 8},         
                           {24, 16}},               // 180 degrees
                 {{32, 0},
                  {32, 8},
        {40, 16}, {32, 16}},                        // 270 degrees
         
    },
    {   // L Block
                         {{24, 0},
        {40, 8}, {32, 8}, {24, 8}},                 // 0 degrees
                {{32, 0},
                 {32, 8},
                 {32, 16}, {24, 16}},               // 90 degrees
        {{40, 8}, {32, 8}, {24, 8},
         {40, 16}},                                 // 180 degrees
        {{40, 0}, {32, 0},  
                  {32, 8},
                  {32, 16}}                         // 270 degrees
    },
    {   // O Block
        {{32, 0}, {24, 0},                      
         {32, 8}, {24, 8}},                         // 0 degrees
        {{32, 0}, {24, 0},
         {32, 8}, {24, 8}},                         // 90 degrees
        {{32, 0}, {24, 0},
         {32, 8}, {24, 8}},                         // 180 degrees
        {{32, 0}, {24, 0},
         {32, 8}, {24, 8}}                          // 270 degrees
    },
    {   // S Block
                {{32, 0}, {24, 0},
        {40, 8}, {32, 8}},                          // 0 degrees
                {{32, 0},
                 {32, 8}, {24, 8},
                          {24, 16}},                // 90 degrees
                 {{32, 8}, {24, 8},
        {40, 16}, {32, 16}},                        // 180 degrees
       {{40, 0},
        {40, 8}, {32, 8},
                 {32, 16}}                          // 270 degrees
                
    },
    {   // T Block
                {{32, 0},
        {40, 8}, {32, 8}, {24, 8}},                 // 90 degrees
                {{32, 0},
                 {32, 8}, {24, 8},
                 {32, 16}},                         // 180 degrees
       {{40, 8}, {32, 8}, {24, 8},
                 {32, 16}},                         // 270 degrees
                {{32, 0},
        {40, 8}, {32, 8},
                 {32, 16}}                          // 270 degrees
    },
    {   // Z Block
        {{40, 0}, {32, 0},
                  {32, 8}, {24, 8}},                // 0 degrees
                          {{24, 0},
                  {32, 8}, {24, 8},
                  {32, 16}},                        // 90 degrees
        {{40, 8}, {32, 8},
                  {32, 16}, {24, 16}},              // 180 degrees
                 {{32, 0},
         {40, 8}, {32, 8},
         {40, 16}}                                  // 270 degrees        
    }
};

// Incrementally randomize the next bag until we switch over
void incremental_randomizer(void) {
    // Randomly swap the current piece and another piece
    uint8_t current = NUMBER_OF_PIECES - bag_position - 1;
    uint8_t swapping = rand() % (current + 1); 
    shape_t temp = new_bag[current];
    new_bag[current] = new_bag[swapping];
    new_bag[swapping] = temp;
    
    // Switch to new bag when at last bag entry
    if (bag_position == NUMBER_OF_PIECES - 1) {
        memcpy(current_bag, new_bag, sizeof(shape_t) * NUMBER_OF_PIECES);
        bag_position = 0;
    } else {
        bag_position++;
    }
}

// Custom comparator for the leaderboard
int compare_scores(const void *a, const void *b) {
    // Cast to player_t types for comparison
    const player_t *player1 = (const player_t *)a;
    const player_t *player2 = (const player_t *)b;
    
    // Check if either name starts with '?'
    bool player1_defined = (player1->name[0] != '?');
    bool player2_defined = (player2->name[0] != '?');

    // If only one name starts with '?', it should come after
    if (player1_defined && !player2_defined) return -1;
    if (!player1_defined && player2_defined) return 1;

    // Compare scores in descending order
    return player2->score - player1->score;
}

// Tries to save the name on the leaderboard
void try_to_save(void) {
    player_t lowest_score = score_board[SCORE_BOARD_SIZE - 1];
    // Can't save because the lowest score is defined and the current
    // player score is lower than the lowest score
    if (lowest_score.name[0] != '?' && lowest_score.score >= player.score){
        return;
    }
    
    // Either the score isn't defined or current player score is greater
    score_board[SCORE_BOARD_SIZE - 1] = player;
    
    // Sort the scores (from highest to lowest)
    qsort((player_t*) score_board, SCORE_BOARD_SIZE, sizeof(player_t), compare_scores);
    
    // Write the initialized scoreboard to EEPROM since it updated
    eeprom_write_block((void*) score_board, (void*)0, sizeof(score_board));
}

// Using the layout defined in PIECES to generate a n x n block per piece
void make_piece(
    point_t transform_location[TRANSFORM_PIECE_SIZE], 
    point_t outline[PIECE_SIZE]
) {
    int16_t index = 0;
    for (int16_t p = 0; p < PIECE_SIZE; p++) {
        int16_t r0 = outline[p].row;
        int16_t c0 = outline[p].col;
        
        // Generate a n x n block for all 4 blocks that make a piece
        for (int16_t r = 0; r < MULTIPLIER_SIZE; r++) {
            for (int16_t c = 0; c < MULTIPLIER_SIZE; c++) {
                int16_t rf = r + r0, cf = c + c0;
                transform_location[index++] = (point_t) {.row = rf, .col = cf};
            }
        }
    }
}

// Generate a random piece
void random_piece(void) {
    // Randomly select shape from the current bag
    game.piece.shape = current_bag[bag_position];
    game.piece.rotation = ROTATION_0;
    
    // Incrementally randomize the new bag and show next piece
    incremental_randomizer();
    display_LEDs(current_bag[bag_position] + 1);
    
    // Generate actual tetromino piece (n x n per block)
    point_t transform_location[TRANSFORM_PIECE_SIZE]; 
    make_piece(transform_location, (point_t*) PIECES[game.piece.shape][game.piece.rotation]);
    
    // Save Actual Piece Location
    for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {       
        game.piece.location[p] = transform_location[p];
        
        // Tetris Death or Topped Out Occurs
        if (is_colliding(game.piece.location[p])) {
            // Save user if possible
            try_to_save();   
            
            // Switch game states to GAME OVER
            state = GAME_OVER;
            game_over_time = millis();
            game_over();
            
            // Reset score and LEDs
            display_score(0);
            display_LEDs(0);
            
            // Stop music and reset music notes for next game
            stop_FGM();
            note = 0;
            last_note = 0;
            
            return;
        }
    }
    
    // Save piece rotations
    for (int16_t r = 0; r < ROTATIONS; r++) {
        for (int16_t p = 0; p < PIECE_SIZE; p++) {
            game.piece.rotations[r][p] = PIECES[game.piece.shape][r][p];
        }
    }
    
    // Set the piece after generating randomly
    update_piece();
}

// Update the display whenever called
void update_board(void) {
    display_screen(game.board);
}

// Set the piece and update the display
void update_piece(void) {
    for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
        set_bit(game.board, game.piece.location[p].row, game.piece.location[p].col);
    }
    update_board();
}

// Updates player score based on the amount of lines cleared
void score_player(uint8_t cleared_lines) {
    switch (cleared_lines) {
        case 1:
            player.score += 10;
            break;
        case 2:
            player.score += 30;
            break;
        case 3:
            player.score += 50;
            break;
        default:
            player.score += 80;
    }
}

// Move's the block from it's current position to a new one using masking
void move_block(
    point_t old_topright, 
    point_t new_topright, 
    int16_t clear_mask, 
    int16_t set_mask
) {
    int16_t row = old_topright.row, start_column = old_topright.col;
    // Clear block's previous position
    for (int16_t col = start_column; col < start_column + MULTIPLIER_SIZE; col++) {
        game.board[row][col] = game.board[row][col] & clear_mask;
    }
    
    // Move block to new position
    row = new_topright.row, start_column = new_topright.col;
    for (int16_t col = start_column; col < start_column + MULTIPLIER_SIZE; col++) {
        game.board[row][col] = game.board[row][col] | set_mask;
    }
}

// Determine if the current n x n cell matches condition after applying the mask
bool cell(int16_t row, int16_t start_column, int16_t mask, int16_t condition) {
    for (int16_t col = start_column; col < start_column + MULTIPLIER_SIZE; col++) {
        if ((game.board[row][col] & mask) != condition) {
            return false;
        }
    }
    return true;
}

// Shift blocks down given an individual line clear
void cascade_down(int16_t start_column) {    
    for (int16_t row = 0; row < BOARD_ROWS; row++) {
        point_t topright = {.row = row, .col = start_column};
        
        // Search for lowest empty n x n cell
        for (int16_t col = start_column; col < BOARD_COLUMNS; col += MULTIPLIER_SIZE) {
            if (!cell(row, col, FULL_ROW, EMPTY_ROW)) break;
            topright.row = row;
            topright.col = col;
        }

        // Go backwards (start column -> 0)
        for (int16_t col = start_column; col >= 0; col -= MULTIPLIER_SIZE) {
            // If it is a cell of all full rows (block), shift it down
            if (cell(row, col, FULL_ROW, FULL_ROW)) {
                point_t old_topright = {.row = row, .col = col};
                move_block(old_topright, topright, EMPTY_ROW, FULL_ROW);
                topright.row = row;
                topright.col = topright.col - MULTIPLIER_SIZE;
            }
        }
        update_board();
    }
}

// Performs a line check on the current column position
// Columns iterate as this for example: 124 -> 127 or 0 -> 3
void clear_line(int16_t start_column, int16_t end_column) {
    
     // Iterate forward for the columns (0 -> 127)
    for (int16_t col = start_column; col < end_column; col++) {
        
        // Iterate forwards for rows (0 -> 7)
        for (int16_t row = 0; row < BOARD_ROWS; row++) {
            
            // Clear the row
            game.board[row][col] = EMPTY_ROW;
        }
    }
}

// Performs a line check on the current column position
// Columns iterate as this for example: 124 -> 127 or 0 -> 3
bool check_clear(int16_t start_column, int16_t end_column) {
    
    // Iterate forward for the columns (0 -> 127)
    for (int16_t col = start_column; col < end_column; col++) {
        
        // Iterate forwards for rows (0 -> 7)
        for (int16_t row = 0; row < BOARD_ROWS; row++) {
            
            // If any entry isn't completely set, can't line clear
            if (game.board[row][col] != FULL_ROW) {
                return false;
            }
        }
    }
    
    return true;
}

// Check for any lines that need to be cleared
void cascade_lines(void) {
    bool clearing = true;
    uint8_t cleared_lines = 0;
    
    // Always start BOARD_COLUMNS - MULTIPLIER_SIZE
    int16_t start_column = BOARD_COLUMNS - MULTIPLIER_SIZE;
    int16_t first_cleared_column = 0;
    
    // Run a convergence algorithm until all lines are cleared
    while (clearing) {
        clearing = false;
        
        // Iterate backwards for columns (124 -> 0) 
        // with -MULTIPLIER_SIZE decrements: 124, 120, 116, ....
        for (int16_t col = start_column; col >= 0; col -= MULTIPLIER_SIZE) {
            
            // Check for a line clear
            if (check_clear(col, col + MULTIPLIER_SIZE)) {
                
                // First clear of this run
                if (!clearing) first_cleared_column = col;
                
                // If there is a line clear, clear that line and cascade 
                // the pieces above downward
                clear_line(col, col + MULTIPLIER_SIZE);
                
                // Break out after cascading for possible new lines
                // that need to be cleared
                cleared_lines++;
                clearing = true;
            }
        }
        if (clearing) {
            update_board();
            cascade_down(first_cleared_column);
        }
    }
    
    // Only update if any line clears happened
    if (cleared_lines > 0) {
        score_player(cleared_lines);
        display_score(player.score);
        update_board();
    }
}

// Handles line clearing similar to NES Tetris
void normal_lines(void) {
    int16_t col = BOARD_COLUMNS - MULTIPLIER_SIZE;
    uint8_t cleared_lines = 0;
    for (int16_t upper_col = col; upper_col >= 0; upper_col -= MULTIPLIER_SIZE) {
        int16_t count = 0;
        for (int16_t offset = 0; offset < MULTIPLIER_SIZE; offset++) {
            for (int16_t row = 0; row < BOARD_ROWS; row++) {
                if (game.board[row][upper_col + offset] == FULL_ROW) count++;
                game.board[row][col + offset] = game.board[row][upper_col + offset];
            }
        }
        if (count < ALL_ROWS) {
            col -= MULTIPLIER_SIZE;
        } else {
            cleared_lines++;
        }
    }
    
    if (cleared_lines > 0) {
        score_player(cleared_lines);
        display_score(player.score);
        update_board();
    }
}

// Check if position is out of bounds
bool in_bounds(point_t position) {
    // Row is outside of bounds
    if (position.row < 0 || position.row >= SCREEN_ROWS) {
        return false;
    }
    
    // Column outside of bounds
    if (position.col < 0 || position.col >= SCREEN_COLUMNS) {
        return false;
    }
    
    return true;
}

// Check if position is colliding with another set position
bool is_colliding(point_t position) {
    // Piece already there (colliding)
    return is_set(game.board, position.row, position.col);
}

// Rotate a piece clockwise if possible
void rotate(void) {
    // Clear all bits to begin with
    for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
        clear_bit(game.board, game.piece.location[p].row, game.piece.location[p].col);
    }
    
    rotation_t current = game.piece.rotation;
    rotation_t next = (rotation_t) (current != ROTATION_270 ? current + 1 : ROTATION_0);
    point_t new_rotation[TRANSFORM_PIECE_SIZE];
    make_piece(new_rotation, (point_t*) game.piece.rotations[next]);
    
    // Iterate all the points that make up a piece
    for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
        // Check if rotation is valid
        point_t position = new_rotation[p];
        bool valid = in_bounds(position);
        bool colliding = is_colliding(position);
        if (!valid || colliding) {
            // If rotation isn't valid, re-display cause of the clear
            update_piece();
            return;
        }
    }
    
    // Swap rotation
    game.piece.rotation = next;
    
    // Update all points that make a piece to the new rotated orientation
    for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
        game.piece.location[p] = new_rotation[p];
    }
    
    // Display updated piece orientation
    update_piece();
}

// Move in the horizontal or vertical direction
void move(int16_t row_dir, int16_t col_dir) {
    // No move
    if (row_dir == 0 && col_dir == 0) {
        return;
    }
    
    // Clear all bits to begin with
    for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
        clear_bit(game.board, game.piece.location[p].row, game.piece.location[p].col);
    }
    
    // Calculate maximum move spaces for rows and columns
    int16_t max_steps = max(abs(row_dir), abs(col_dir));
    
    // Set incrementer values for row and column 
    int16_t row_increment = 0, col_increment = 0;
    if (row_dir != 0) row_increment = (row_dir > 0 ? 1 : -1);
    if (col_dir != 0) col_increment = (col_dir > 0 ? 1 : -1);
    
    // Try all move spaces, this accounts for when a move space puts a 
    // piece out of bounds or in collision with another piece
    for (int step = 0; step < max_steps; step++) {
        
        // Check if all points of a piece can move in that direction
        for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
            point_t position = game.piece.location[p];
            position.row += row_increment;
            position.col += col_increment;
            
            // Check if new point is valid
            bool valid = in_bounds(position);
            bool colliding = is_colliding(position);
            if (!valid || colliding) {
                // If movement isn't valid, re-display cause of the clear
                update_piece();
                
                // Can't move down any further
                if (col_increment > 0) {
                    // Check if line is made before new piece added
                    switch (mode) {
                        case NORMAL:
                            normal_lines();
                            break;
                        case CASCADE:
                            cascade_lines();
                    }
                    
                    random_piece();
                }
                return;
            }
        }

        // Move the piece in that direction
        for (int16_t p = 0; p < TRANSFORM_PIECE_SIZE; p++) {
            game.piece.location[p].row += row_increment;
            game.piece.location[p].col += col_increment;
        }
        
        // Update the mirrored rotations if piece can move
        for (int16_t r = 0; r < ROTATIONS; r++) {
            for (int16_t p = 0; p < PIECE_SIZE; p++) {
                game.piece.rotations[r][p].row += row_increment;
                game.piece.rotations[r][p].col += col_increment;
            }
        }
    }
    
    // Display updated piece with new location
    update_piece();
}

// Starts the game of tetris
void start_game(void) {
    // Initialization
    game.board = screen;
    memcpy(current_bag, INITIAL_BAG, sizeof(shape_t) * NUMBER_OF_PIECES);
    memcpy(new_bag, INITIAL_BAG, sizeof(shape_t) * NUMBER_OF_PIECES);
    bag_position = 0;
    player.score = 0;
    
    // Reset OLED and LED displays
    display_score(player.score);
    clear_screen(game.board);
    
    // Initialize non-display LEDs
    PORTA.DIR |= PIN5_bm | PIN6_bm | PIN7_bm;
    
    // Play a new piece
    random_piece();
}
