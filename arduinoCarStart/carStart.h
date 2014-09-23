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

void carAction(carState car_state, BUTTON_STATE btn_state);

#endif