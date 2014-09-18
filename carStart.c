/******************************************************************************
 * Author: Eric Boyer
 * CarStart.c - Functions for Arduino Car Ignition System
 *****************************************************************************/

#include "arduino_io.h"

#define ACC_PIN     arduinoPins[4].number
#define ON_PIN      arduinoPins[5].number
#define STARTER_PIN arduinoPins[2].number
#define START_BUTTON arduinoPins[3].number

static const unsigned int RFID_READ_TIMEOUT = 30000; // Milliseconds

// If button is pressed for less than this amount in milliseconds, it is considered a Tap.
static const unsigned int BTN_TAP_TIME = 2000; 

static enum rfidState {
    INVALID,
    VALID,
    VALIDATED,
    MASTER
}

static enum carState{
    OFF,
    ACC,
    ON,
    LISTENING
}

enum carState CAR = OFF;
enum rfidState RFID = INVALID;

static enum rfidState readRFID(void){
    // Check database and see if RFID tag is white-listed.
    
    return INVALID;
}

static enum carState getCarState(void){
    return CAR;
}

static void setCarState(enum carState newState){
    CAR = newState;
    return;
}

/* startBtnListener
   Start listening for input from the push button
*/
static void startBtnListener(void){
    unsigned long startMillis = millis();
    setCarState(LISTENING);
    
    // Start a timer for RFID Reader, if this times out then user must rescan RFID Tag
    while(timer(RFID_READ_TIMEOUT, startMillis)){
        carAction(getCarState(), buttonListener());

    }
    
    if(getCarState() == LISTENING){
        // RFID was read, but user didn't start car or put it into accessory mode
        setCarState(OFF);
    }
}

// REWRITE/FIX THIS: The way this function is currently written, it is nearly impossible for button to have 'RELEASED' state
static enum BUTTON_STATE buttonListener(void){
    unsigned int startMilli = 0;
    // Check if button is pressed (high)
    if(digitalRead(START_BUTTON) == HIGH){
        startMilli = millis();
        while(timer(BTN_TAP_TIME, startMilli){
            if(digitalRead(START_BUTTON) == LOW){
                return TAPPED;
            }
        }

        if(digitalRead(START_BUTTON) == HIGH){
            return PRESSED_AND_HELD;
        }
        return RELEASED;
    }
    else{
        return RELEASED;
    }
    
    return NO_ACTION;
}

/* timer
   Function to act as a countdown timer.
   endMillis - How long the timer should run (in milliseconds)
   startMillis - This is the time (in milliseconds) in which the timer started
   
   ProTip: DO NOT call timer(XX ms, millis()) <- Cause that's how you get infinite timer...
*/
static boolean timer(unsigned int endMillis, unsigned int startMillis){
    if((millis() - startMillis) < endMillis){
        return true; // Timer isn't finished yet
    }
    return false; // Timer ended
}

/* initialize
 * Do any necessary startup initialization for Arduino
 */
void initialize(void){
}

void loopRunner(void){
    if(getCarState() == OFF){
        switch(readRFID()){
            case MASTER:
                // Set it to learning mode?
                break;
            case VALID:
                // Signal a beep or LED, then listen for push button input
                startBtnListener();
                break;
            case INVALID:
            default:
                // Do nothing
                break;
        }
    }else if(RFID == VALIDATED){
        // carAction(getCarState(), buttonListener());
    }
}


void carAction(enum carState car_state, enum BUTTON_STATE btn_state){
    switch(car_state){
        case OFF:
            if(btn_state == PRESSED_AND_HELD){
                // Start car
            }
            else if(btn_state == TAPPED){
                changeCarState(ACC);
            }
            break;
        case ACC:
            if(btn_state == PRESSED_AND_HELD){
                // Start Car
            }
            else if(btn_state == TAPPED){
                // Turn car off & set RFID = INVALID
            }
            break;
        case ON:
            if(btn_state == PRESSED_AND_HELD){
                // Turn car off & set RFID = INVALID
            }
        case LISTENING:
        default:
            break;
    }
    return;
}
