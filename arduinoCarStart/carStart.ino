/******************************************************************************
 * Author: Eric Boyer
 * CarStart.c - Functions for Arduino Car Ignition System
 *****************************************************************************/

#include "arduino_io.h"
#include "carStart.h"
#include "rfid.h"

#define START_BUTTON arduinoPins[2].number
// For race tracks where an external killswitch is required. (This will not disable the battery.)
// #define EMERGENCY_KILLSWITCH 3
#define ACC_PIN      arduinoPins[4].number
#define ON_PIN1      arduinoPins[5].number
#define ON_PIN2      arduinoPins[6].number
#define STARTER_PIN  arduinoPins[7].number
// Uncomment this to use a pin to check if the clutch is pushed in or not.
// #define CLUTCH_PIN arduinoPins[12].number // Ground this pin to start the car
#define ONBOARD_LED  arduinoPins[13].number

// These are used for sounding an auditory alert to the user of the ignition state
#define ACTIVATE   1
#define DEACTIVATE 0

const static unsigned int MOTOR_PRIME_TIMER = 2000;  // milliseconds - Time to prime the motor with fuel

// If button is pressed for less than this amount in milliseconds, it is considered a Tap.
const static unsigned int BTN_TAP_TIME = 1500; 

const static unsigned int DEBOUNCE_TIME = 50; // milliseconds - Button Debounce time

// Variable to store the current state of the vehicle.
static carState CAR = OFF;
// Variable to store the current RFID Tag value (Valid, Invalid, Validated, Master)
static rfidState RFID = INVALID;

static rfidState readRFID(){
    // Check database and see if RFID tag is white-listed.
    if(RFID != VALIDATED){
        RFID = readRFIDTag();
    }
    return RFID;
}

static carState getCarState(){
    return CAR;
}

static void setCarState(carState newState){
    CAR = newState;
}

void carAction(carState car_state, BUTTON_STATE btn_state);

/* Ftn: rfidTimer
   Desc: Start listening for input from the push button
*/
static void rfidTimer(){
    unsigned long startMillis = millis();
    printf("RFID Timer waiting for user to turn on car.");
    setCarState(LISTENING);
    RFID = VALIDATED;
    digitalWrite(ONBOARD_LED, HIGH);
    
    // Start a timer for RFID Reader, if this times out then user must rescan RFID Tag
    while(timer(RFID_READ_TIMEOUT, startMillis) && (getCarState() == LISTENING)){
        carAction(getCarState(), buttonListener(START_BUTTON));
        delay(25); // Small delay.
    }
    
    if(getCarState() == LISTENING){
        printf("RFID Timer End - Invalidating tag.");
        // RFID was read, but user didn't start car or put it into accessory mode
        // setCarState(OFF);
        RFID = INVALID;
        digitalWrite(ONBOARD_LED, LOW);
        beep(DEACTIVATE);
    }
    return;
}

/* Ftn: debouncef
   Desc: Function for doing some button debouncing. This is for 'cleaner' input from the user
*/
static boolean debouncef(int btn){
    int state = digitalRead(btn); // Store current state of button.
    int bounceStartTime = millis();
    
    while(timer(DEBOUNCE_TIME, bounceStartTime)){
        delay(10); // Not sure if this delay should be here...
    }
    
    // Check if button is still in same state, if so return true (Good button press).
    if(state == digitalRead(btn)){
        return true;
    }
    return false;
}

/* Ftn: buttonListener
 * Desc: This function checks to see if the button is just being tapped, or if it's being pressed and held.
*/
static BUTTON_STATE buttonListener(int btn){
    // Convert the Tapped Timeout from millisecond to microsecond
    unsigned long tapMicroSec = BTN_TAP_TIME*1000;
    unsigned long btnTime = pulseIn(btn, HIGH, tapMicroSec);
    
    printf("buttonListener - Button Pressed time: " + btnTime);
    if((btnTime != 0) && (btnTime < tapMicroSec)){
        printf("Button TAPPED.");
        return TAPPED;
    }else if(digitalRead(btn) == HIGH){
        return btnStatef(btn);
    }
    return NO_ACTION;
}

/* Ftn: btnStatef
   Desc: Function for returning the current state of the specified button
*/
static BUTTON_STATE btnStatef(int btn){
    if(digitalRead(btn) == HIGH){
        if(debouncef(btn)){
            return PRESSED_AND_HELD;
        }
        else{
            return NO_ACTION;
        }
    }
    return RELEASED;
}

/* Ftn: carStart
   Desc: This function does a couple things. It puts the car into ON mode for MOTOR_PRIME_TIMER 
         milliseconds, then it activates the Starter Pin while the user holds down the button.
*/
static void carStart(){
    unsigned int startMilli = millis();
    
    printf("carStart - Starting Car");
    
    // Prime the motor with fuel
    while(timer(MOTOR_PRIME_TIMER, startMilli) && (btnStatef(START_BUTTON) == PRESSED_AND_HELD)){
        digitalWrite(ON_PIN1, HIGH);
        digitalWrite(ON_PIN2, HIGH);
    }
    
    while(btnStatef(START_BUTTON) == PRESSED_AND_HELD){
        digitalWrite(ON_PIN1, LOW);
        digitalWrite(ON_PIN2, LOW);
        digitalWrite(STARTER_PIN, HIGH);
        delay(50);
    }
    
    digitalWrite(ON_PIN1, HIGH);
    digitalWrite(ON_PIN2, HIGH);
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
    digitalWrite(ON_PIN1, LOW);
    digitalWrite(ON_PIN2, LOW);
    
    setCarState(OFF);
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
            beep(ACTIVATE);
//            rfidTimer();
//            break;
        case VALIDATED:
            if(getCarState() == OFF){
                rfidTimer();
            }
            else{
                carAction(getCarState(), buttonListener(START_BUTTON));
            }
            break;
        case INVALID:
        default:
            // Do nothing
            break;
    }
}

/* Ftn: clutchPinf
   Desc: Checks to see if the clutch is pressed in or not.
   Returns true if clutch is pressed in, false otherwise.
*/
static boolean clutchPinf(){
#ifdef CLUTCH_PIN
    if(digitalRead(CLUTCH_PIN) == LOW){
        return false; // clutch is let out
    }
#endif
    return true; // clutch is in.
}

/* Ftn: initialize
 * Desc: Do any necessary project specific startup initialization for Arduino
 */
void initializeApp(){
    initRFIDReader();
}

/* Ftn: loopRunner
   Desc: Function that is looped through indefinitely by the Arduino
*/
void loopRunner(){
    listenForRFID();
}

/* Ftn: carAction
   Desc: Performs an action based on the current state of the car and input from the start button
         by the user.
*/
void carAction(carState car_state, BUTTON_STATE btn_state){
    if(RFID){
        switch(car_state){
            case OFF:
                if((btn_state == PRESSED_AND_HELD) && clutchPinf()){
                    carStart(); // Start car
                }
                else if(btn_state == TAPPED){
                    carAcc();
                }
                break;
            case ACC:
                if((btn_state == PRESSED_AND_HELD) && clutchPinf()){
                    carStart(); // Start Car
                }
                else if(btn_state == TAPPED){
                    carOff(); // Turn car off & set RFID = INVALID (after a timer)
                }
                break;
            case ON:
#ifdef CLUTCH_PIN
                if(btn_state == PRESSED_AND_HELD){
                    if(clutchPinf()){
                        carStart();
                    }else{
                        carOff(); // Turn car off & set RFID = INVALID
                    }
                }
#else
                if(btn_state == PRESSED_AND_HELD){
                    carOff(); // Turn car off & set RFID = INVALID
                }
#endif
                break;
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
    // Read Master Tag (?) - Have a separate Valet Tag
    // Wait for 10 Second button press or Tap it XX times
    
    // Signal user that car is now in Valet Mode
}

/* Ftn: beep
   Desc: Alerts the user that the ignition has been activated or deactivated
*/
void beep(int type){
    if(type == ACTIVATE){
        // beep beep
    }
    else if(type == DEACTIVATE){
        // beep
    }
}

