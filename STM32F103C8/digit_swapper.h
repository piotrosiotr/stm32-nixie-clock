#ifndef DIGIT_SWAPPER_H
#define DIGIT_SWAPPER_H
#include "stdint.h"
#include "rtc.h"

/*

    this module provides dynamic indication
    for 4 tubes. it allows to set display time
    and to control digit blinking for time setting 

*/

typedef struct _digit_t{
    uint8_t id;             // id off a tube
    uint8_t value;          // display value
    uint8_t blink_enabled;  // blink enabled flag
    uint8_t is_dimmed;      // internal flag for blinking
}digit_t;

void digit_swapper_init(void);
void digit_swapper_start(void);
void digit_swapper_stop(void);
void digit_swapper_set_time(time_t *t);
void digit_swapper_enable_blink(uint8_t digit_id);
void digit_swapper_disable_blink(uint8_t digit_id);

#endif