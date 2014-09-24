/*****************************************************************************
 * Author: Eric Boyer
 * RFID tag reader functions and declarations
 *****************************************************************************/
 
#ifndef RFID_H
#define RFID_H

typedef enum {
    INVALID, // Invalid RFID Tag has been read
    VALID,
    VALIDATED,
    MASTER
} rfidState;

const unsigned int rfidTimeout = 30000; // Milliseconds

rfidState readRFIDTag();

#endif
