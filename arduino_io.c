/*****************************************************************************
 * Author: Eric Boyer
 * Arduino_IO - This is a configuration file for the inputs and outputs of the
                Arduino Board.
******************************************************************************/

#include "arduino_io.h"

void initializeArduino(void){
    for(int i = 0; i < arduinoPins.size(); i++){
        if(arduinoPins[i].enabled){
            pinMode(arduinoPins[i].number, arduinoPins[i].ioType);
        }
    }
}

void setPin(int pin, string value){
    digitalWrite(pin, value);
}

void setPin(string pin, string value){
    analogWrite(pin, value);
}
