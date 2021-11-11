/*
 * File:   timer.c
 * Author: Abdullah Almosalami
 *
 * Created on October 28, 2021, 10:12 PM
 */


#include <xc.h>
#include "timer.h"
#include "ccp.h"


// Global or static variables relevant to the timer modules


// Functions

// Timer1

/* Function: Timer1_Init_Default
 * ---------------------------------
 * Initialize Timer1 in the default manner, which for this project will be for
 * it to utilize CCP2 in the manner described in CCP2.h and to have the T1CON be
 * 16-bit, prescaler of 8, tmr1 osc off, internal clock, tmr1 off
 * 
 * Parameters: uint16_t period_val --> This is what will be loaded into CCP2
 * 
 * Returns: none
 */
void Timer1_Init_Default(void){
    TMR1_DEFAULT;
    TMR1_ENABLE_INTERRUPT;
}

/* Function: Timer1_Enable
 * ---------------------------------
 * Turns Timer1 on.
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void Timer1_Enable(void){
    TMR1_ON;
}

/* Function: Timer1_Disable
 * ---------------------------------
 * Turns Timer1 off.
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void Timer1_Disable(void){
    TMR1_OFF;
}


//    // Timer1 Overflow Interrupt
//    if(TMR1_IF && TMR1_ENABLE_BIT) {
//        
//        spi_ready_to_tx = 0x01;
//        
//        TMR1_CLEAR_IF;
//    }