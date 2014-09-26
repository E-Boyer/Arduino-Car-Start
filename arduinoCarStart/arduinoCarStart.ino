/*****************************************************************************
 * Author: Eric Boyer
 * Arduino Car Ignition
 *****************************************************************************/
 
/*****************************************************************************
To Do List:
 -POSSIBLE ISSUE
   -PulseIn, will it return after the timeout or does it wait for the button to be released??
   -Do I also need to add some button debouncing to my btnStatef function?
   
Input
 -System Activation/Deactivation alerts (Auditory/Visual)
 -System Status (?)
 -RFID Reading/Learning
  
 -Emergency Killswitch (For track use)
 
 -Car Setup (will probably want to use relays for powering car)
   -Buy extra ignition plug
   
 -Order RFID board and a few tags
   -Write RFID Tag save/delete functions
   http://playground.arduino.cc/Learning/MFRC522

   http://www.electrodragon.com/product/mifare-rc522-rfid-card-readerdetector-ic-card/
   https://github.com/miguelbalboa/rfid
******************************************************************************/

#include "arduino_io.h"
#include "error.h"

#include "carStart.h"
#include "rfid.h"

/* Ftn: Setup
 * Desc: put your setup code here, to run once */
void setup(){
    initializeArduino();
    initialize();
}

/* Ftn: loop
 * Desc: put your main code here, to run repeatedly  */
void loop(){
    loopRunner();
    delay(200);
}

