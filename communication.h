/* 
 * File: communication.h
 * Author: Andrew Hoyle
 * Comments: Includes, Directives, and Functions for communication.c
 */
 
#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#include <avr/io.h>
#include "globals.h"

#define TWI_WAIT() while (!(TWI0.MSTATUS & TWI_WIF_bm))
#define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
#define TWI_WRITE false
#define TWI_READ true

void init_I2C(void);
void start_I2C(const int address);
void stop_I2C(void);
bool write_I2C(const uint8_t* data, const uint8_t len);

#endif
