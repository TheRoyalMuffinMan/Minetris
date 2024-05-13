/*
 * File: communication.c
 * Author: Andrew Hoyle
 *
 * Created on March 8, 2024, 3:24 PM
 */

#include "communication.h"

// Initialize I2C for the Display
void init_I2C(void) {
    // Set SCL and SDA as output
    PORTA.DIR |= PIN2_bm | PIN3_bm;
    
    // Standard 100kHz TWI, 4 Cycle Hold, 50ns SDA Hold Time
    TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;
    
    // Enable Run in Debug
    TWI0.DBGCTRL = TWI_DBGRUN_bm;
    
    // Clear MSTATUS (write 1 to flags). BUSSTATE set to idle
    TWI0.MSTATUS = TWI_RIF_bm | TWI_WIF_bm | TWI_CLKHOLD_bm | TWI_RXACK_bm |
            TWI_ARBLOST_bm | TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
    
    // Set baud rate to 10
    TWI0.MBAUD = 10;
    
    // [No ISRs] and Host Mode
    TWI0.MCTRLA = TWI_ENABLE_bm;
}

// Send Address (in WRITE MODE) then proceed to wait
void start_I2C(const int address) {
    TWI0.MADDR = (address << 1) | TWI_WRITE;
    TWI_WAIT();
}

// Stop the bus
void stop_I2C(void) {
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
}

// Writes the bytes in a data array to the LED display
bool write_I2C(const uint8_t* data, const uint8_t len) { 
    uint8_t count = 0;
    
    while (count < len) {
        // Write a byte
        TWI0.MDATA = data[count];
        
        // Wait for it to go through
        TWI_WAIT();
        
        // If the client NACKed, then abort the write
        if (CLIENT_NACK()) {
            return false;
        }
        
        // Increment the counter
        count++;
    }
    
    return true;
}
