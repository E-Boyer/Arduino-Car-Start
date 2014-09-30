/*****************************************************************************
 * Author: Eric Boyer
 * Arduino_IO - This is a configuration file for the inputs and outputs of the
                Arduino Board.
******************************************************************************/

#include "arduino_io.h"

void initializeArduino(void){
    for(int i = 0; i < NELEMS(arduinoPins); i++){
        if(arduinoPins[i].enabled){
            pinMode(arduinoPins[i].number, arduinoPins[i].ioType);
        }
    }
    
    // open the serial port at 9600 bps:
#ifdef OUTPUT_STATEMENTS
    Serial.begin(9600);
#endif
}

/* Ftn: timer
   Desc: Function to act as a countdown timer.
   Parameters: endMillis - How long the timer should run (in milliseconds)
               startMillis - This is the time (in milliseconds) in which the timer started
   Notes:
     Example Usage) int start = millis();
                    while(timer(30000, start)){
                      // Do Stuff for 30 seconds
                    }
   
     ProTip: DO NOT call while(timer(XX ms, millis())) <- Cause that's how you get infinite timer...
   
     "abs" explanation: millis() overflows after ~50 days, so in the event that it overflows then it'll end the timer.
*/
boolean timer(unsigned int endMillis, unsigned int startMillis){
    if(abs(millis() - startMillis) < endMillis){
        return true; // Timer isn't finished yet
    }
    return false;    // Timer ended
}

/* printf
 * Print out a message to the console.
 */
void printf(String statement){
#ifdef OUTPUT_STATEMENTS
    Serial.println(statement);
    Serial.println("\r\n");
#endif
    return;
}
