/******************************************************************************
 * Author: Eric Boyer
 * CarStart.c - Functions for Arduino Car Ignition System
 *****************************************************************************/

#include "arduino_io.h"
#include "rfid.h"

#define ACC_PIN     2 //arduinoPins[4].number
#define ON_PIN      3 //arduinoPins[5].number
#define STARTER_PIN 4 //arduinoPins[2].number
#define START_BUTTON 5 //arduinoPins[3].number

static const unsigned int RFID_READ_TIMEOUT = 30000; // milliseconds
static const unsigned int MOTOR_PRIME_TIMER = 1500; // milliseconds

// If button is pressed for less than this amount in milliseconds, it is considered a Tap.
static const unsigned int BTN_TAP_TIME = 1500; 

carState CAR = OFF;
rfidState RFID = INVALID;

static rfidState readRFID(){
    // Check database and see if RFID tag is white-listed.
    RFID = readRFIDTag();
    return RFID;
}

static carState getCarState(){
    return CAR;
}

static void setCarState(carState newState){
    CAR = newState;
    return;
}

void carAction(carState car_state, BUTTON_STATE btn_state);

/* startBtnListener
   Start listening for input from the push button
*/
static void startBtnListener(){
    unsigned long startMillis = millis();
    if(getCarState() == OFF){
        setCarState(LISTENING);
        RFID = VALIDATED;
    }
    
    // Start a timer for RFID Reader, if this times out then user must rescan RFID Tag
    while(timer(RFID_READ_TIMEOUT, startMillis)){
        carAction(getCarState(), buttonListener(START_BUTTON));
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
static BUTTON_STATE buttonListener(int btn){    
    unsigned long btnTime = pulseIn(btn, HIGH, BTN_TAP_TIME*1000);
    if((btnTime != 0) && (btnTime < (BTN_TAP_TIME*1000))){
        return TAPPED;
    }
    
    return btnListen(btn);
}

static BUTTON_STATE btnListen(int btn){
    if(digitalRead(btn) == HIGH){
        return PRESSED_AND_HELD;
    }
    return RELEASED;
}

static void carStart(){
    unsigned int startMilli = millis();
    
    // Prime the motor with fuel
    while(timer(MOTOR_PRIME_TIMER, startMilli)){
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

static void carOff(){
    digitalWrite(STARTER_PIN, LOW);
    digitalWrite(ACC_PIN, LOW);
    digitalWrite(ON_PIN, LOW);
    
    setCarState(OFF);
    startBtnListener();
    return;
}

static void carAcc(){
    digitalWrite(ACC_PIN, HIGH);
    setCarState(ACC);
    return;
}

/* initialize
 * Do any necessary startup initialization for Arduino
 */
void initialize(){
}

void loopRunner(){
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

void carAction(carState car_state, BUTTON_STATE btn_state){
    if(RFID){
        switch(car_state){
            case OFF:
                if(btn_state == PRESSED_AND_HELD){
                    carStart(); // Start car
                }
                else if(btn_state == TAPPED){
                    carAcc();
                }
                break;
            case ACC:
                if(btn_state == PRESSED_AND_HELD){
                    carStart(); // Start Car
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

