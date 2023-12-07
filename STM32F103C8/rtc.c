#include "rtc.h"
#include "stm32f10x.h"
#include "controller.h"


time_t sys_time;

void rtc_init(void) {
    // enable rtc (check reference manual)
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN; 
    PWR->CR |= PWR_CR_DBP; 
    //if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
    //{
        RCC->BDCR |= RCC_BDCR_BDRST;  
        RCC->BDCR &= ~RCC_BDCR_BDRST;
        RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;     
        RCC->BDCR |= RCC_BDCR_LSEON;        
        while ((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON); 
        RCC->BDCR |= RCC_BDCR_RTCEN;         
        // enter configuration mode
        while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
        RTC->CRL |= RTC_CRL_CNF;            
        RTC->CRL &= ~RTC_CRL_SECF;
        RTC->PRLL = 0x7FFF;  
        // enable rtc second interrupt               
        RTC->CRH |= RTC_CRH_SECIE;
        NVIC_EnableIRQ(RTC_IRQn); 
        // exit rtc configuration mode
        RTC->CRL &= ~RTC_CRL_CNF;      
        while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
    //}
    RTC->CRL &= (uint16_t) ~RTC_CRL_RSF;
    while ((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF);


    
    sys_time.hours = 0;
    sys_time.minutes = 0;
    sys_time.seconds = 0;
}

// get system time from rtc module

void rtc_get_time(time_t *t) {
    uint32_t time_var = (RTC->CNTH<<16)|RTC->CNTL;
    t->hours = time_var/3600;
    t->minutes = (time_var%3600)/60;
    t->seconds = (time_var%3600)%60;
}

// set system time

void rtc_set_time(time_t *t) {
    uint32_t time_var = t->hours*3600+t->minutes*60+t->seconds;
    while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
    RTC->CRL |= RTC_CRL_CNF; 
    RTC->CNTL = (uint16_t)(time_var&0xFFFF);
    RTC->CNTH = (uint16_t)(time_var>>16);
    RTC->CRL &= ~RTC_CRL_CNF; 
    while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
}

// second innterrupt handler (generates every second)

void RTC_IRQHandler(void) {
    if(RTC->CRL &RTC_CRL_SECF) {
        RTC->CRL &= ~RTC_CRL_SECF;
        // get system time
        rtc_get_time(&sys_time);
        // do we need to update display time? 
        // (update neede every 60 seconds)
        if(sys_time.seconds == 0) {
            // time needs to be updated on display
            controller_set_state(EVT_TIME_UPDATED);
            // counter reaches 24 hours, set to 0
            if(sys_time.hours == 24) {
                sys_time.hours = 0;
                rtc_set_time(&sys_time);
            }
        }
    }
}