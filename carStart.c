/******************************************************************************
 * Author: Eric Boyer
 * CarStart.c - Functions for Arduino Car Ignition System
 *****************************************************************************/

#include "arduino_io.h"
#include "rfid.h"

#define ACC_PIN     arduinoPins[4].number
#define ON_PIN      arduinoPins[5].number
#define STARTER_PIN arduinoPins[2].number
#define START_BUTTON arduinoPins[3].number

static const unsigned int RFID_READ_TIMEOUT = 30000; // milliseconds
static const unsigned int MOTOR_PRIME_TIMER = 1500; // milliseconds

// If button is pressed for less than this amount in milliseconds, it is considered a Tap.
static const unsigned int BTN_TAP_TIME = 1500; 

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
    RFID = readRFIDTag();
    return RFID;
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
    if(getCarState() == OFF){
        setCarState(LISTENING);
        RFID = VALIDATED;
    }
    
    // Start a timer for RFID Reader, if this times out then user must rescan RFID Tag
    while(timer(RFID_READ_TIMEOUT, startMillis)){
        carAction(getCarState(), buttonListener());
    }
    
    if(getCarState() == LISTENING){
        // RFID was read, but user didn't start car or put it into accessory mode
        setCarState(OFF);
        RFID = INVALID;
        // carOff();
    }
    return;
}

/* buttonListener
 * This function checks to see if the button is just being tapped, or if it's being pressed and held.
*/
static enum BUTTON_STATE buttonListener(int btn){    
    unsigned long btnTime = pulseIn(btn, HIGH, BTN_TAP_TIME);
    if(btnTime != 0){
        return TAPPED;
    }
    
    return btnListen(btn);
}

static enum BUTTON_STATE btnListen(int btn){
    if(digitalRead(btn) == HIGH){
        return PRESSED_AND_HELD;
    }
    return RELEASED;
}

static void carStart(void){
    unsigned int startMilli = millis();
    
    // Prime the motor with fuel
    while(timer(MOTOR_PRIME_TIMER, startMilli){
        digitalWrite(ON_PIN, HIGH);
    }
    
    digitalWrite(ON_PIN, LOW);
    
    while(btnListen(START_BUTTON) == PRESSED_AND_HELD){
        digitalWrite(STARTER_PIN, HIGH);
    }
    digitalWrite(ON_PIN, HIGH);
    
    digitalWrite(STARTER_PIN, LOW);
    
    setCarState(ON);
    return;
}

static void carOff(void){
    digitalWrite(STARTER_PIN, LOW);
    digitalWrite(ACC_PIN, LOW);
    digitalWrite(ON_PIN, LOW);
    
    setCarState(OFF);
    startBtnListener();
    return;
}

static void carAcc(void){
    digitalWrite(ACC_PIN, HIGH);
    setCarState(ACC);
    return;
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
    if(RFID){
        switch(car_state){
            case OFF:
                if(btn_state == PRESSED_AND_HELD){
                    carStart(); // Start car
                }
                else if(btn_state == TAPPED){
                    changeCarState(ACC);
                }
                break;
            case ACC:
                if(btn_state == PRESSED_AND_HELD){
                    carStart(): // Start Car
                }
                else if(btn_state == TAPPED){
                    carOff(); // Turn car off & set RFID = INVALID (after a timer)
                }
                break;
            case ON:
                if(btn_state == PRESSED_AND_HELD){
                    carOff(); // Turn car off & set RFID = INVALID
                }
            case LISTENING:
            default:
                break;
        }
    }
    return;
}
