#ifndef CARSTART_H
#define CARSTART_H

typedef enum {
    OFF,
    ACC,
    ON,
    LISTENING
} carState;

void initializeApp();
void loopRunner();

/****************************************************************************
 Ftn: carAction
 
 State Machine Logic
 off -(Press & Hold)-> On -> Starter -> On -(Tap)-> off 
     |                       ^          |            ^
     |                       | -------- <            |
     |                      (Press & Hold)           |
     v                                               |
   (Tap) -> Acc -(Tap)-------------------------------^
                |
                --(Press & Hold)--> On
 ****************************************************************************/
void carAction(carState car_state, BUTTON_STATE btn_state);

#endif