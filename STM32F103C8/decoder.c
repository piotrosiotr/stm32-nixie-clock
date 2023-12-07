#include "decoder.h"
#include "stm32f10x.h"

// init decoder peripherals

void decoder_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // PA4 - push-pull
    GPIOB->CRL &= ~GPIO_CRL_CNF0;
    GPIOB->CRL |= GPIO_CRL_MODE0;
    // PA5 - push-pull
    GPIOB->CRL &= ~GPIO_CRL_CNF1;
    GPIOB->CRL |= GPIO_CRL_MODE1;
    // PA6 - push-pull
    GPIOB->CRH &= ~GPIO_CRH_CNF10;
    GPIOB->CRH |= GPIO_CRH_MODE10;
    // PA7 - push-pull
    GPIOB->CRH &= ~GPIO_CRH_CNF11;
    GPIOB->CRH |= GPIO_CRH_MODE11;
}

// set digit

void decoder_set_digit(uint8_t d) {
    // set pinns to state equivalent to binary format of digit
    // (check K155ID1 datasheet for more information) 
    GPIOB->BSRR = ((d&0x01) == 0)? GPIO_BSRR_BR0 : GPIO_BSRR_BS0;
    GPIOB->BSRR = (((d>>1)&0x01) == 0)? GPIO_BSRR_BR1 : GPIO_BSRR_BS1;
    GPIOB->BSRR = (((d>>2)&0x01) == 0)? GPIO_BSRR_BR10 : GPIO_BSRR_BS10;
    GPIOB->BSRR = (((d>>3)&0x01) == 0)? GPIO_BSRR_BR11 : GPIO_BSRR_BS11;
}