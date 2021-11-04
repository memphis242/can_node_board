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
void Timer1_Init_Default(uint16_t period_val){
    
    // Configure T1CON and T3CON so that Timer1 is used for CCP2
    TMR1_DEFAULT;
    TMR1_FOR_COMP_CCP2;
    
    // Set up CCP2 as compare with T1CON and load the CCPR2 compare registers
    CCP2_Init_Default();
    CCP2_Compare_Val(period_val);
    
//    TMR1_ENABLE_INTERRUPT;
    
}

void Timer1_Enable(void){
    TMR1_ON;
}

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