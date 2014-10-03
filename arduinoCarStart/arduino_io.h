/*****************************************************************************
 * Author: Eric Boyer
 * Arduino_IO - This is a configuration file for the inputs and outputs of the
                Arduino Board.
******************************************************************************/

#ifndef ARDUINO_IO_H
#define ARDUINO_IO_H

#include <Arduino.h> // https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Arduino.h
#include <EEPROM.h>  // http://www.arduino.cc/en/Reference/EEPROM
// #include "Timer.h"   // http://playground.arduino.cc/Code/Timer

// Definition used to determine if Serial Output statements should be enabled or not
#define OUTPUT_STATEMENTS
// Find the number of elements in an array
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

typedef enum {
    NO_ACTION,
    TAPPED,
    PRESSED_AND_HELD,
    RELEASED
} BUTTON_STATE;

// This could be of use for program modularity purposes
typedef struct Pin {
    boolean enabled; // Whether or not the pin is enabled
    byte number;  // The number of the pin, This might not work for analog pins...
    byte level;   // Pin level (HIGH and LOW)
    byte ioType;  // Input/Output Type (INPUT, INPUT_PULLUP-Reverses high & Low, and OUTPUT)
} Pin;

// This table will hold all of the pin information 
const Pin arduinoPins[] = {                                                         /* App Specific Purpose */
    {false, 0, LOW, OUTPUT},  // Digital Pin 0 - Reserved for serial (if enabled)   /* RFID Tag Reader */
    {false, 1, LOW, OUTPUT},  // Digital Pin 1 - Reserved for serial (if enabled)   /* RFID Tag Reader */
    {true,  2, LOW, INPUT},   // Digital Pin 2 - ISR 0                              /* Start Button */
    {false, 3, LOW, INPUT},   // Digital Pin 3 - ISR 1                              /* Emergency Killswitch (If implemented) */
    {true,  4, LOW, OUTPUT},  // Digital Pin 4                                      /* ACC Output Pin */
    {true,  5, LOW, OUTPUT},  // Digital Pin 5                                      /* ON 1 Output Pin */
    {true,  6, LOW, OUTPUT},  // Digital Pin 6                                      /* ON 2 Output Pin */
    {true,  7, LOW, OUTPUT},  // Digital Pin 7                                      /* Starter Output Pin */
    {false, 8, LOW, INPUT},   // Digital Pin 8                                      /* Not Used */
    {false, 9, LOW, OUTPUT},  // Digital Pin 9                                      /* Not Used */
    {false, 10, LOW, OUTPUT}, // Digital Pin 10                                     /* Not Used */
    {true, 11, LOW, INPUT},   // Digital Pin 11                                     /* RFID 'Valid Tag Button' - DEV ONLY */
    {true, 12, LOW, INPUT},   // Digital Pin 12                                     /* Clutch Input Pin */
    {true,  13, LOW, OUTPUT}  // Onboard LED (Arduino UNO)
    
    // TODO: Add the Analog Pins
};

void initializeArduino(void);

boolean timer(unsigned int endMillis, unsigned int startMillis);

void printf(String statement);

#endif
