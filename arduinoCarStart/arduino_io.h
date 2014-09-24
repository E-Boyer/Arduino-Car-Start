/*****************************************************************************
 * Author: Eric Boyer
 * Arduino_IO - This is a configuration file for the inputs and outputs of the
                Arduino Board.
******************************************************************************/

#ifndef ARDUINO_IO_H
#define ARDUINO_IO_H

#include <Arduino.h> // https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Arduino.h
#include <EEPROM.h>  // http://www.arduino.cc/en/Reference/EEPROM

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
    bool enabled; // Whether or not the pin is enabled
    byte number;  // The number of the pin, This might not work for analog pins...
    byte level;   // Pin level (HIGH and LOW)
    byte ioType;  // Input/Output Type (INPUT, INPUT_PULLUP-Reverses high & Low, and OUTPUT)
}Pin;

// This table will hold all of the pin information 
const Pin arduinoPins[] = {
    {false, 0, LOW, OUTPUT},  // Digital Pin 0 - Reserved for serial (if enabled)
    {false, 1, LOW, OUTPUT},  // Digital Pin 1 - Reserved for serial (if enabled)
    {true,  2, LOW, INPUT},   // Digital Pin 2 - ISR 0
    {false, 3, LOW, INPUT},   // Digital Pin 3 - ISR 1
    {true,  4, LOW, OUTPUT},  // Digital Pin 4
    {true,  5, LOW, OUTPUT},  // Digital Pin 5
    {true,  6, LOW, OUTPUT},  // Digital Pin 6
    {false, 7, LOW, OUTPUT},  // Digital Pin 7
    {true, 8, LOW, INPUT},  // Digital Pin 8
    {false, 9, LOW, OUTPUT},  // Digital Pin 9
    {false, 10, LOW, OUTPUT}, // Digital Pin 10
    {false, 11, LOW, OUTPUT}, // Digital Pin 11
    {false, 12, LOW, OUTPUT}, // Digital Pin 12
    {true,  13, LOW, OUTPUT}  // Onboard LED (Arduino UNO)
    // TODO: Add the Analog Pins
};

#define START_BUTTON arduinoPins[2].number
// For race tracks where an external killswitch is required. (This will not disable the battery.)
// #define EMERGENCY_KILLSWITCH 3
#define ACC_PIN      arduinoPins[4].number
#define ON_PIN       arduinoPins[5].number
#define STARTER_PIN  arduinoPins[6].number
#define ONBOARD_LED  arduinoPins[13].number


void initializeArduino(void);

bool timer(unsigned int endMillis, unsigned int startMillis);

void printf(String statement);

#endif
