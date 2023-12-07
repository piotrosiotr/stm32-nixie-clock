#include "stm32f10x.h"
#include "decoder.h"
#include "digit_swapper.h"

#define STATIC_DIGIT_INIT(_id_) {.id = _id_, .is_dimmed = 0, .value = 0, .blink_enabled = 0}

digit_t digit_0 = STATIC_DIGIT_INIT(0);
digit_t digit_1 = STATIC_DIGIT_INIT(1);
digit_t digit_2 = STATIC_DIGIT_INIT(2);
digit_t digit_3 = STATIC_DIGIT_INIT(3);

digit_t * display[] = {&digit_0, &digit_1, &digit_2, &digit_3};

// init anode swapping peripherals

void digit_swapper_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
    // PA4 - Anode 1 - PP
    GPIOA->CRL &= ~GPIO_CRL_CNF4;
    GPIOA->CRL |= GPIO_CRL_MODE4;
    // PA5 - Anode 2 - PP
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    // PA6 - Anode 3 - PP
    GPIOA->CRL &= ~GPIO_CRL_CNF6;
    GPIOA->CRL |= GPIO_CRL_MODE6;
    // PA7 - Anode 4 - PP
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_MODE7;
    // configure swap timer
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // enable interrupts
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    // configure blink timer
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // enable interrupts
    TIM4->DIER |= TIM_DIER_UIE;
    // set blink timer freq to 2Hz (2 times a second)
    TIM4->PSC = 7200-1;
    TIM4->ARR = 5000;
    TIM4->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM4_IRQn);
    
	decoder_init();
}

// enable anode swapping

void digit_swapper_start(void) {
    // anode swap timer freq is 400 Hz
    // (in reality every tube blinkks at
    // 50Hz (400 / 4 digits / half time enabled))
    TIM2->PSC = 7200-1;
    TIM2->ARR = 25;
    TIM2->CR1 |= TIM_CR1_CEN;
}

// disable anode swapping

void digit_swapper_stop(void) {
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

// set time to be displayed

void digit_swapper_set_time(time_t *t) {
    display[0]->value = t->hours/10;
    display[1]->value = t->hours%10;
    display[2]->value = t->minutes/10;
    display[3]->value = t->minutes%10;
}

// enable blink for tube by id

void digit_swapper_enable_blink(uint8_t digit_id) {
    for(uint8_t i=0; i<4; i++) {
        if(digit_id == display[i]->id) display[i]->blink_enabled = 1;
    }
}

// disable blink for tube by id

void digit_swapper_disable_blink(uint8_t digit_id) {
    for(uint8_t i=0; i<4; i++) {
        if(digit_id == display[i]->id) {
            display[i]->blink_enabled = 0;
            display[i]->is_dimmed = 0;
        }
    }
}

uint8_t digit_idx = 0;
uint8_t gap_flag =0;

void next_digit(void) {
    // gap flag is needed due to weak dcdc booster
    // half a time all tubes are off so the capacitor
    // can charge enough but tubes are a bit dusker
    if(gap_flag) {
        // disable all tubes
        GPIOA->BSRR = GPIO_BSRR_BR4;
        GPIOA->BSRR = GPIO_BSRR_BR5;
        GPIOA->BSRR = GPIO_BSRR_BR6;
        GPIOA->BSRR = GPIO_BSRR_BR7;
    } else {
        switch (digit_idx)
        {
            // enable tube 0 if not dimmed 
            case 0: {
                if(display[digit_idx]->is_dimmed == 0) {
                    decoder_set_digit(display[digit_idx]->value);
                    GPIOA->BSRR = GPIO_BSRR_BS4;
                }
            } break;
            // enable tube 1 if not dimmed 
            case 1: {
                if(display[digit_idx]->is_dimmed == 0) {
                    decoder_set_digit(display[digit_idx]->value);
                    GPIOA->BSRR = GPIO_BSRR_BS5;
                }
            } break;
            // enable tube 2 if not dimmed 
            case 2: {
                if(display[digit_idx]->is_dimmed == 0) {
                    decoder_set_digit(display[digit_idx]->value);
                    GPIOA->BSRR = GPIO_BSRR_BS6;
                }
            } break;
            // enable tube 3 if not dimmed 
            case 3: {
                if(display[digit_idx]->is_dimmed == 0) {
                    decoder_set_digit(display[digit_idx]->value);
                    GPIOA->BSRR = GPIO_BSRR_BS7;
                }        
            } break;
            
            default:
                break;
        }
        //vgo through tubes array
        digit_idx++;
        if(digit_idx > 3) digit_idx = 0;
    }
    gap_flag = !gap_flag;
}

void TIM2_IRQHandler(void) {
    // switch to next tube
    if(TIM2->SR &TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        next_digit();
    }
}

void TIM4_IRQHandler(void) {
    // blink effect for time setting
    if(TIM4->SR &TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;
        for(uint8_t i=0; i<4; i++) {
            if(display[i]->blink_enabled) display[i]->is_dimmed = !display[i]->is_dimmed;
        }
    }
}