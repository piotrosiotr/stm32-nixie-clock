#ifndef DCDC_H
#define DCDC_H
#include "stdint.h"

/*

    this module provides all necessary peripherals
    control for hv dcdc booster.
    
*/

void dcdc_init(void);
void dcdc_start(void);
void dcdc_stop(void);
#endif