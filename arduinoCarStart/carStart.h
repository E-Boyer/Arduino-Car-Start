#ifndef CARSTART_H
#define CARSTART_H

typedef enum {
    OFF,
    ACC,
    ON,
    LISTENING
} carState;

void initialize();
void loopRunner();

/****************************************************************************
 Ftn: carAction
 
 State Machine Logic
 off -> On -> Starter -> On -(Tap)-> off 
               ^          |
               | -------- < 
              (Press & Hold)
              
 off -(Tap)-> Acc -(Tap)-> off
               |
               --(Press & Hold)--> On
 ****************************************************************************/
void carAction(carState car_state, BUTTON_STATE btn_state);

#endif