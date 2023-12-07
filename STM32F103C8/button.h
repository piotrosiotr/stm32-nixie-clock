#ifndef BUTTON_H
#define BUTTON_H
#include "stdint.h"

/*

    this module enables button peripherals
    and provides button control handling.

*/

void button_init(void);
uint8_t button_get_state(void);

#endif