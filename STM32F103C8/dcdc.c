#include "dcdc.h"
#include "stm32f10x.h"

// init all dcdc peripherals

void dcdc_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN;
    // PA11 - push-pull afio pwm pin
	GPIOA->CRH &= ~GPIO_CRH_CNF11;
	GPIOA->CRH |= GPIO_CRH_CNF11_1;
	GPIOA->CRH	&= ~GPIO_CRH_MODE11;
	GPIOA->CRH	|= GPIO_CRH_MODE11_1;
	
	// configure pwm timer
	// pwm prameters:
	// 	freq: 10 kHz
	//  duty: 42%
	TIM1->PSC = 2;
	uint16_t arr = 2400;
    TIM1->ARR = arr;	//2400
	TIM1->CCR4 = arr*1400/2400;	//1328
	TIM1->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P;
	TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1; 
	TIM1->CR1 &= ~TIM_CR1_DIR;
	TIM1->CR1 &= ~TIM_CR1_CMS;
	
}

// ennable pwm

void dcdc_start(void) {
    TIM1->CR1 |= TIM_CR1_CEN;
}

// disable pwm

void dcdc_stop(void) {
    TIM1->CR1 &= ~TIM_CR1_CEN;
}