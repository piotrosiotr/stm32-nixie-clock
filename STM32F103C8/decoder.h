#ifndef DECODER_H
#define DECODER_H
#include "stdint.h"

/* 

    this module provides support for
    K155ID1 decoder ic

*/

void decoder_init(void);
void decoder_set_digit(uint8_t d);

#endif