#include "controller.h"
#include "stddef.h"
#include "digit_swapper.h"
#include "rtc.h"

time_t editable_time;

controller_state_t current_state;

// this funnction executes on entering hours editing mode

void enter_hour_edit(controller_evt_t event) {
    // enable blink for hour digits
    digit_swapper_enable_blink(0);
    digit_swapper_enable_blink(1);
    // get current time
    rtc_get_time(&editable_time);
    // start time editing procedure
    digit_swapper_set_time(&editable_time);
    current_state = STATE_EDIT_HOURS;
}

// this funnction executes on entering minutes editing mode

void enter_min_edit(controller_evt_t event) {
    // disable hour digits blink
    digit_swapper_disable_blink(0);
    digit_swapper_disable_blink(1);
    // enable minute digits blink
    digit_swapper_enable_blink(2);
    digit_swapper_enable_blink(3);
    current_state = STATE_EDIT_MINUTES;
}

// handle hours editing

void increase_hours(controller_evt_t event) {
    editable_time.hours++;
    if(editable_time.hours>23) editable_time.hours = 0;
    digit_swapper_set_time(&editable_time);
    current_state = STATE_EDIT_HOURS;
}

// handle minutes editing

void increase_minutes(controller_evt_t event) {
    editable_time.minutes++;
    if(editable_time.minutes>59) editable_time.minutes = 0;
    digit_swapper_set_time(&editable_time);
    current_state = STATE_EDIT_MINUTES;
}

// random glitch

void play_glitch(controller_evt_t event) {
    // to be done
    current_state = STATE_MAIN;
}

// return from time editing mode

void enter_main_state(controller_evt_t event) {
    //disable minutes blink
    digit_swapper_disable_blink(2);
    digit_swapper_disable_blink(3);
    //apply time
    rtc_set_time(&editable_time);
    current_state = STATE_MAIN;
}

// update display time

void update_time(controller_evt_t event) {
    rtc_get_time(&editable_time);
    digit_swapper_set_time(&editable_time);
    current_state = STATE_MAIN;
}

// state machine table

                                            //  short press         long press          time upd        rand glitch
static const evt_callback_t callbacks[3][4] = {{&play_glitch,       &enter_hour_edit,   &update_time,    &play_glitch}, // main state
                                               {&increase_hours,    &enter_min_edit,    NULL,            NULL}, //hours editing state
                                               {&increase_minutes,  &enter_main_state,  NULL,            NULL}  //minutes editing state  
};

//init controller

void controller_init(void) {
    current_state = STATE_MAIN;
}

// handle upcoming event

void controller_set_state(controller_evt_t event) {
    if(callbacks[current_state][event] == NULL) return;
    (*(callbacks[current_state][event]))(event);
}