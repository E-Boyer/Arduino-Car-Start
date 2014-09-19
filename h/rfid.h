/*****************************************************************************
 * Author: Eric Boyer
 * RFID tag reader functions and declarations
 *****************************************************************************/
 
#ifndef RFID_H
#define RFID_H

enum rfidState {
    INVALID,
    VALID,
    VALIDATED,
    MASTER
}

const unsigned int rfidTimeout = 30000; // Milliseconds

enum rfidState readRFIDTag(void);


#endif
