#include "button.h"
#include "stm32f10x.h"
#include "controller.h"

uint8_t is_long_pressed = 0;

// init buttonn peripherals

void button_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // PA3 - button
    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
    GPIOA->CRL |= GPIO_CRL_CNF2_1; 
    // configure button pin irq
    AFIO->EXTICR[2] &= ~(AFIO_EXTICR1_EXTI0);
    // trigger - both fronts (press and release)
    EXTI->RTSR |= EXTI_RTSR_TR2;
    EXTI->FTSR |= EXTI_FTSR_TR2;
    EXTI->PR = EXTI_PR_PR2; 
    EXTI->IMR |= EXTI_IMR_MR2;
    NVIC_EnableIRQ(EXTI2_IRQn); 
    // configure timer for long press detection
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM3_IRQn);
    // set long press timeout to 1 second
    TIM3->PSC = 7200-1;
    TIM3->ARR = 10000;
}

// get button state

uint8_t button_get_state(void) {
    return (GPIOA->IDR & GPIO_IDR_IDR2)>>2;
}

// button pin irq handler 

void EXTI2_IRQHandler(void) {
  EXTI->PR = EXTI_PR_PR2;
  // get button value to recognize front
  uint8_t pressed = (GPIOA->IDR & GPIO_IDR_IDR2)>>2;
  if(pressed) {
    //start long press detection
    TIM3->CR1 |= TIM_CR1_CEN;
  } else {
    //stop counnter - press ended
    TIM3->CR1 &= ~TIM_CR1_CEN;
    if(!is_long_pressed) controller_set_state(EVT_SHORT_PRESS);
    is_long_pressed = 0;
  }

  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
}

// lonng press timer irrq handler

void TIM3_IRQHandler(void) {
    if(TIM3->SR &TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        // stop counting
        TIM3->CR1 &= ~TIM_CR1_CEN;
        // get button state
        uint8_t pressed = (GPIOA->IDR & GPIO_IDR_IDR2)>>2;
        if(pressed) {
            // long press
            is_long_pressed = 1;
            controller_set_state(EVT_LONG_PRESS);
        } else {
            // short press (in theory never to be reached)
        }
    }
}