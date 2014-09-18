/*****************************************************************************
 * Author: Eric Boyer
 * Arduino_IO - This is a configuration file for the inputs and outputs of the
                Arduino Board.
******************************************************************************/

#ifndef ARDUINO_IO_H
#define ARDUINO_IO_H

#define ONBOARD_LED 13

const enum BUTTON_STATE {
    NO_ACTION,
    TAPPED,
    PRESSED_AND_HELD,
    RELEASED
}

struct Pin {
    bool enabled;  // Whether or not the pin is enabled
    int number;    // The number of the pin, This might now work for analog pins...
    string level;  // Pin level (HIGH and LOW)
    string ioType; // Input/Output Type (INPUT, INPUT_PULLUP-Reverses high & Low, and OUTPUT)
};

// This table will hold all of the pin information 
const Pin arduinoPins = [
    {false, 0, "LOW", "OUTPUT"},  // Digital Pin 0 - Reserved for serial (if enabled)
    {false, 1, "LOW", "OUTPUT"},  // Digital Pin 1 - Reserved for serial (if enabled)
    {true, 2, "LOW", "OUTPUT"},  // Digital Pin 2
    {true, 3, "LOW", "INPUT"},  // Digital Pin 3
    {true, 4, "LOW", "OUTPUT"},  // Digital Pin 4
    {true, 5, "LOW", "OUTPUT"},  // Digital Pin 5
    {false, 6, "LOW", "OUTPUT"},  // Digital Pin 6
    {false, 7, "LOW", "OUTPUT"},  // Digital Pin 7
    {false, 8, "LOW", "OUTPUT"},  // Digital Pin 8
    {false, 9, "LOW", "OUTPUT"},  // Digital Pin 9
    {false, 10, "LOW", "OUTPUT"}, // Digital Pin 10
    {false, 11, "LOW", "OUTPUT"}, // Digital Pin 11
    {false, 12, "LOW", "OUTPUT"}  // Digital Pin 12
];

void setPin(int pin, string value);
void setPin(string pin, string value);

void initializeArduino(void);

#endif
