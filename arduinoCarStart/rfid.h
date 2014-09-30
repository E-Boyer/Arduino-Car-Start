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

const static unsigned int RFID_READ_TIMEOUT = 30000; // milliseconds - Timeout for the RFID Reader if it reads a valid tag.

rfidState readRFIDTag();
void initRFIDReader();

#endif
