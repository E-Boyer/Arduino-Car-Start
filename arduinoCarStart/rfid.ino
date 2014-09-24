/*****************************************************************************
 * Author: Eric Boyer
 * RFID tag reader functions and declarations
 *****************************************************************************/

#include "rfid.h"

// Store new RFID Tag ID's
static boolean saveTag(int id){
    return false;
}

// Delete a tag that has been stored
static void deleteTag(int id){
}

// Clear all tags, then store all new ones within range
static void clearAll(){
}

#define RFID_TEST_BUTTON 8 // For DEVELOPMENT PURPOSES ONLY

// Read the RFID tag from RFID reader
rfidState readRFIDTag(){
#ifdef RFID_TEST_BUTTON
    if(digitalRead(RFID_TEST_BUTTON) == HIGH){
        return VALID;
    }
#endif
    return INVALID;
}

