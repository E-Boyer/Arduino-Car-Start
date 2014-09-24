/******************************************************************************
 * Author: Eric Boyer
 * CarStart.c - Functions for Arduino Car Ignition System
 *****************************************************************************/

#include "arduino_io.h"
#include "rfid.h"

static const unsigned int RFID_READ_TIMEOUT = 30000; // milliseconds
static const unsigned int MOTOR_PRIME_TIMER = 1500; // milliseconds

// If button is pressed for less than this amount in milliseconds, it is considered a Tap.
static const unsigned int BTN_TAP_TIME = 1500; 

// Variable to store the current state of the vehicle.
static carState CAR = OFF;
// Variable to store the current RFID Tag value (Valid, Invalid, Validated, Master)
static rfidState RFID = INVALID;

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

/* rfidTimer
   Start listening for input from the push button
*/
static void rfidTimer(){
    unsigned long startMillis = millis();
    if(getCarState() == OFF){
        setCarState(LISTENING);
        RFID = VALIDATED;
        digitalWrite(ONBOARD_LED, HIGH);
    }
    
    // Start a timer for RFID Reader, if this times out then user must rescan RFID Tag
    while(timer(RFID_READ_TIMEOUT, startMillis) && (getCarState() == LISTENING)){
        carAction(getCarState(), buttonListener(START_BUTTON));
    }
    
    if(getCarState() == LISTENING){
        // RFID was read, but user didn't start car or put it into accessory mode
        setCarState(OFF);
        RFID = INVALID;
        digitalWrite(ONBOARD_LED, LOW);
    }
    return;
}

/* buttonListener
 * This function checks to see if the button is just being tapped, or if it's being pressed and held.
*/
static BUTTON_STATE buttonListener(int btn){    
    unsigned long btnTime = pulseIn(btn, HIGH, BTN_TAP_TIME*1000);
    if((btnTime != 0) && (btnTime < (BTN_TAP_TIME*1000))){
        printf("Button TAPPED.");
        return TAPPED;
    }else if(digitalRead(START_BUTTON) == HIGH){
        return btnStatef(btn);
    }
    return NO_ACTION;    
}

/* Ftn: btnStatef
   Desc: Function for returning the current state of the specified button
*/
static BUTTON_STATE btnStatef(int btn){
    if(digitalRead(btn) == HIGH){
        return PRESSED_AND_HELD;
    }
    return RELEASED;
}

/* Ftn: carStart
   Desc: 
*/
static void carStart(){
    unsigned int startMilli = millis();
    
    printf("carStart - Starting Car");
    
    // Prime the motor with fuel
    while(timer(MOTOR_PRIME_TIMER, startMilli) && (btnStatef(START_BUTTON) == PRESSED_AND_HELD)){
        digitalWrite(ON_PIN, HIGH);
    }
    
    digitalWrite(ON_PIN, LOW);
    
    while(btnStatef(START_BUTTON) == PRESSED_AND_HELD){
        digitalWrite(STARTER_PIN, HIGH);
    }
    digitalWrite(ON_PIN, HIGH);
    digitalWrite(ACC_PIN, HIGH);
    
    digitalWrite(STARTER_PIN, LOW);
    
    setCarState(ON);
    return;
}

/* Ftn: carOff
   Desc: This function is ran to turn the car off.
*/
static void carOff(){
    printf("carOff - Turning car off");

    digitalWrite(STARTER_PIN, LOW);
    digitalWrite(ACC_PIN, LOW);
    digitalWrite(ON_PIN, LOW);
    
    setCarState(OFF);
    rfidTimer();
    return;
}

/* Ftn: carAcc
   Desc: This function is ran to put the car into accessory mode.
*/
static void carAcc(){
    printf("carAcc - Putting car into Accessory Mode");
    
    digitalWrite(ACC_PIN, HIGH);
    setCarState(ACC);
    return;
}

/* Ftn: listenForRFID
   Desc: Calls the RFID reading function and checks to see if an RFID tag has been read.
*/
static void listenForRFID(){
    switch(readRFID()){
        case MASTER:
            // Set it to learning mode?
            break;
        case VALID:
            // Signal a beep or LED, then listen for push button input
            rfidTimer();
            break;
        case VALIDATED:
            break;
        case INVALID:
        default:
            // Do nothing
            break;
    }
}

/* Ftn: initialize
 * Desc: Do any necessary project specific startup initialization for Arduino
 */
void initialize(){
}

/* Ftn: loopRunner
   Desc: Function that is looped through indefinitely by the Arduino
*/
void loopRunner(){
    if(getCarState() == OFF){
        listenForRFID();
    }else if(RFID == VALIDATED){
        carAction(getCarState(), buttonListener(START_BUTTON));
    }
}

/* Ftn: carAction
   Desc: Performs an action based on the current state of the car and 
         input from the start button by the user.
*/
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

/* valetMode
   Put the car in valetMode so that it doesn't need to read the RFID Tag
     -Set Timeout or ## Allowed Car Starts
*/
void valetMode(){
    // Read Master Tag (?)
    // Wait for 10 Second button press or Tap it XX times
    
    // Signal user that car is now in Valet Mode
}

