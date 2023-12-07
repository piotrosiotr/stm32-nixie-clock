#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdint.h"

/*

    this module proides state machine 
    to control all other modules
    according to all upcoming events:
        button press
        button long press
        display time needs to be updated

*/

typedef enum _controller_state_t{
    STATE_MAIN,
    STATE_EDIT_HOURS,
    STATE_EDIT_MINUTES,
}controller_state_t;

typedef enum _controller_evt_t{
    EVT_SHORT_PRESS,
    EVT_LONG_PRESS,
    EVT_TIME_UPDATED,
    EVT_RAND_GLITCH
}controller_evt_t;

typedef void (*evt_callback_t)(controller_evt_t evt);

void controller_init(void);
void controller_set_state(controller_evt_t event);

#endif //CONTROLLER_H