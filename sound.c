/*
 * File: sound.c
 * Author: Andrew Hoyle
 * Created on April 26, 2024, 8:14 PM
 */

#include "sound.h"

// Music: Korobeiniki (A-Type) https://en.wikipedia.org/wiki/Korobeiniki
// Following notes and note duration (with some minor modifications) were based
// on the following link:
// https://www.jk-quantized.com/blog/2013/11/22/tetris-theme-song-using-processing
// All credit goes to the original creator
/* Respective note for each entry converted (saves on division operation)
NOTES = {
    659.25511, 493.8833, 523.25113, 587.32954, 523.25113, 493.8833, 440.0, 440.0,
    523.25113, 659.25511, 587.32954, 523.25113, 493.8833, 523.25113, 587.32954,
    659.25511, 523.25113, 440.0, 440.0, 440.0, 493.8833, 523.25113, 587.32954,
    698.45646, 880.0, 783.99087, 698.45646, 659.25511, 523.25113, 659.25511,
    587.32954, 523.25113, 493.8833, 493.8833, 523.25113, 587.32954, 659.25511,
    523.25113, 440.0, 440.0 
}
*/
const int CONVERTED_NOTES[NOTES_SIZE] = {
    5039, 6721, 6345, 5655, 6345, 6721, 7540, 7540,
    6345, 5039, 5655, 6345, 6721, 6345, 5655, 5039,
    6345, 7540, 7540, 7540, 6721, 6345, 5655, 4757,
    3778, 4239, 4757, 5039, 6345, 5039, 5655, 6345, 
    6721, 6721, 6345, 5655, 5039, 6345, 7540, 7540
};

const float NOTE_DURATION[NOTES_SIZE] = {
    203.125, 101.5625, 101.5625, 203.125, 101.5625, 101.5625, 203.125, 101.5625,
    101.5625, 203.125, 101.5625, 101.5625, 304.6875, 101.5625, 203.125, 203.125,
    203.125, 203.125, 101.5625, 101.5625, 101.5625, 101.5625, 304.6875, 101.5625,
    203.125, 101.5625, 101.5625, 304.6875, 101.5625, 203.125, 101.5625, 101.5625,
    203.125, 101.5625, 101.5625, 203.125, 203.125, 203.125, 203.125, 203.125
};

// Starts the FGM
inline void __attribute__((always_inline)) start_FGM(int note) {
    // First reset the TCA for FGM
    TCA0.SINGLE.CTRLESET = TCA_SINGLE_CMD_RESET_gc;
    
    // Set port for output
    PORTC.DIR |= PIN0_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_FRQ_gc;
    TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
    TCA0.SINGLE.CMP0 = CONVERTED_NOTES[note];
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTC_gc;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}

// Stops the FGM
inline void __attribute__((always_inline)) stop_FGM(void) {
    TCA0.SINGLE.CTRLA &= ~(TCA_SINGLE_ENABLE_bm);
}

// Play tetris soundtrack based on the following:
inline void __attribute__((always_inline)) sound(
    volatile uint32_t* note_time, 
    uint32_t milliseconds, 
    volatile uint8_t* note, 
    volatile uint8_t* last_note
) {
    if (elapsed_time(*note_time, milliseconds) >= NOTE_DURATION[*last_note]) {
        stop_FGM();
        start_FGM(*note);
        *last_note = *note;
        *note = (*note == NOTES_SIZE - 1) ? 0 : *note + 1;
        *note_time = milliseconds;
    }
}
