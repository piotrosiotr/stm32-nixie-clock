#ifndef RTC_H
#define RTC_H

#include "stdint.h"

/*

    this module provides rtc peripheral 
    time control.

*/

typedef struct _time_t {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
}time_t;

void rtc_init(void);
void rtc_get_time(time_t *t);
void rtc_set_time(time_t *t);

#endif