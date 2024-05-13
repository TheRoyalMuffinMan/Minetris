/*
 * File: timer.c
 * Author: Andrew Hoyle
 * Created on April 6, 2024, 4:13 PM
 */

#include "timer.h"

// Tracks passing milliseconds
volatile uint32_t milliseconds = 0;

// Tracks running time of a note
volatile uint32_t note_time = 0;

// Tracks current and last note
volatile uint8_t note = 0;
volatile uint8_t last_note = 0;

// Implementation of DebounceSwitch2() (with some modifications) 
// from https://www.ganssle.com/debouncing-pt2.htm
// All credit goes to the original creator
inline bool __attribute__((always_inline)) debounce(
    volatile uint16_t *pin_state, 
    PORT_t* port, int pin
) {
    *pin_state = (*pin_state << 1) | (port->IN & pin) | 0xe000;
    return (*pin_state == 0xf000);
}

// Count milliseconds as setup by the RTC clock
ISR(RTC_CNT_vect) {     
    milliseconds++;
    /* Time Sensitive Functions */
    // The following will poll for status of .IN for 
    // each respective PORT and PIN, therefore if a 
    // button is pressed, the debouncing algorithm will resolve
    // the pressed button and the interrupt handler will see that resolution
    if (debounce(&select_btn, &PORTA, PIN4_bm)) select_btn_debounced = true;
    // Play sound during game
    if (state == RUNNING_GAME) sound(&note_time, milliseconds, &note, &last_note);
    
    RTC.INTFLAGS &= RTC_OVF_bm;
}

// Initializes the real time counter peripheral
void init_RTC(void) {
    uint8_t temp;
    
    // Initialize 32.768kHz Oscillator:
    // Disable oscillator:
    temp = CLKCTRL.XOSC32KCTRLA;
    temp &= ~CLKCTRL_ENABLE_bm;
    // Writing to protected register
    ccp_write_io((void*) &CLKCTRL.XOSC32KCTRLA, temp);
    
    // Wait until XOSC32KS becomes 0
    while(CLKCTRL.MCLKSTATUS & CLKCTRL_XOSC32KS_bm);
    
    // SEL = 1 (Use Internal Crystal):
    temp = CLKCTRL.XOSC32KCTRLA;
    temp |= CLKCTRL_SEL_bm;
    // Writing to protected register
    ccp_write_io((void*) &CLKCTRL.XOSC32KCTRLA, temp);
    
    // Enable oscillator:
    temp = CLKCTRL.XOSC32KCTRLA;
    temp |= CLKCTRL_ENABLE_bm;
    // Writing to protected register
    ccp_write_io((void*)&CLKCTRL.XOSC32KCTRLA, temp);
    
    // Initialize RTC and Wait for all register to be synchronized
    while (RTC.STATUS > 0);

    // Set periods
    RTC.PER = 1;  // 1 millisecond

    // 32.768kHz Internal Crystal Oscillator (INT32K)
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;

    // Run in debug: enabled
    RTC.DBGCTRL |= RTC_DBGRUN_bm;
    
    RTC.CTRLA = RTC_PRESCALER_DIV32_gc  // 32
    | RTC_RTCEN_bm                      // Enable: enabled
    | RTC_RUNSTDBY_bm;                  // Run In Standby: enabled
    
    // Enable Overflow and Compare Interrupts
    RTC.INTCTRL |= RTC_OVF_bm;
}

// Atomically returns the milliseconds that have passed
uint32_t millis(void) {
    uint32_t ms;
    
    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        ms = milliseconds;
    }
    
    return ms;
}

// Determines elapsed time between start and end
// Note: Handles overflow when start > end
uint32_t elapsed_time(volatile uint32_t start, volatile uint32_t end) {
    if (end >= start) {
        return end - start;
    } else {
        return UINT32_MAX - start + end + 1;
    }
}
