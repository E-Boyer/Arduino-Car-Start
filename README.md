Arduino-Car-Start
=================

This is a project that'll allow you to start your car via pushbutton using either bluetooth or RFID for authentication.

Note: This project is IN DEVELOPMENT and such I include no warranty (not like it'll get a warranty anyways). 
I will update this file when I have a stable release of code.

Please ask before you download and start modifying this, although if you start modifying it and make it better, I won't be mad. :)

Hardware List:
Arduino Uno
Wiring diagram of your car's ignition. (Or a friend that has some wiring experience).
Current Board Pin Information
    #define START_BUTTON - Pin 2
    
    #define ACC_PIN      - Pin 4
    #define ON_PIN1      - Pin 5
    #define ON_PIN2      - Pin 6
    #define STARTER_PIN  - Pin 7

I still need to do a wiring diagram for relays that'll power the car along with a 12v -> 5v converter that'll power the Aruino.