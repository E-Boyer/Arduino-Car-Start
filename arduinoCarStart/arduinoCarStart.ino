/*****************************************************************************
 * Author: Eric Boyer
 * Arduino Car Ignition
 *****************************************************************************/
 
/*****************************************************************************
To Do List:
Initialization
 -Pins
 -Get state of car in case of power loss

Input
 -Accessory
 -Start Car (function)
 -Loop Cases
   - off -> Acc -> off
   - off -> On -> Starter -> On -> off 
       -Do this by automatically priming engine then arduino will hit the starter 
        while user holds button down
   - Quick Start (If car dies) On -> Starter -> On
   - Car dies and you want to turn it off On -> Off
 -System Activation/Deactivation alerts (Auditory/Visual)
 -System Status (?)
 -RFID Reading/Learning
 -Simulation Setup
 -Car Setup (will probably want to use relays for powering car)
   -Buy extra ignition plug
******************************************************************************/

#include "error.h"
#include "arduino_io.h"

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
    delay(250);
}

