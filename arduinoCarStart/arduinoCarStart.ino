/*****************************************************************************
 * Author: Eric Boyer
 * Arduino Car Ignition
 *****************************************************************************/
 
/*****************************************************************************
To Do List:
Input
 -System Activation/Deactivation alerts (Auditory/Visual)
 -System Status (?)
 -RFID Reading/Learning
 
 -Emergency Killswitch (For track use)
 
 -Car Setup (will probably want to use relays for powering car)
   -Buy extra ignition plug
   
 -Order RFID board and a few tags
   -Write RFID Tag save/delete functions
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

