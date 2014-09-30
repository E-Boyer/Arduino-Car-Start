/*****************************************************************************
 * Author: Eric Boyer
 * RFID tag reader functions and declarations
 *****************************************************************************/

#include "rfid.h"

#define RFID_TEST_BUTTON 11 // For DEVELOPMENT PURPOSES ONLY

// Read from EEPROM
// http://arduino.cc/en/Reference/EEPROMRead

// Write to EEPROM  (100,000 write/erase cycles...)
// http://arduino.cc/en/Reference/EEPROMWrite

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

// Read the RFID tag from RFID reader
rfidState readRFIDTag(){
#ifdef RFID_TEST_BUTTON
    if(digitalRead(RFID_TEST_BUTTON) == HIGH){
        return VALID;
    }
#endif

    // Check list of valid RFID Tags
    
    return INVALID;
}
