/*
 * File:   ccp.c
 * Author: Abdullah Almosalami
 *
 * Created on October 28, 2021, 10:35 PM
 */


#include <xc.h>
#include "ccp.h"
#include "timer.h"


// Global and static variables relevant to CCP operation



// Functions

// COMPARE FUNCTIONS
/* Function: CCPx_Compare_Init_Default
 * ---------------------------------
 * Initialize the CCPx module to a default state. For this project, that means
 * setting the CCP2M[3:0] bits to 1011 (i.e., 0xB), which sets the CCPx module
 * to Compare mode, generates interrupt on compare match, and leaves the CCPx pin
 * as is. It also triggers a Special Trigger Event; for CCP1, this means
 * resetting the Timer registers associated with CCP1; for CCP2, this means
 * resetting the timer and starting an A/D conversion.
 * I also enable (unmask) the CCP2 interrupt because otherwise, there's
 * no point of using this peripheral.
 * 
 * NOTE: The timer that is used with the CCP module is NOT configured in the
 * CCPxCON register --> instead, it is in T3CON with the T3CCP2:T3CC1 bits, with
 * 00b representing Timer1 being the compare source for the CCP2 module. By
 * default here, I'm using Timer1 for either CCPx modules.
 * 
 * 
 * Parameters: uint16_t comp_val -- The value to place in the CCPRx register-pair
 * 
 * Returns: none
 */
void CCP1_Compare_Init_Default(uint16_t comp_val){
    CCP1M_COMPARE_DEFAULT;
    TMR1_FOR_CCPx;
    CCP1_SET_COMP_VAL(comp_val);
    ENABLE_CCP1_INTERRUPT;
}
void CCP2_Compare_Init_Default(uint16_t comp_val){
    CCP2M_COMPARE_DEFAULT;
    TMR1_FOR_CCPx;
    CCP2_SET_COMP_VAL(comp_val);
    ENABLE_CCP2_INTERRUPT;
}

// CAPTURE FUNCTIONS
/* Function: CCPx_Capture_Init_Default
 * ---------------------------------
 * Here I have the default capture state to be on every rising edge. So,
 * CCPxM = 0101 = 0x5. I also unmask the CCPx interrupt.
 * 
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void CCP1_Capture_Init_Default(void){
    CCP1M_CAPTURE_DEFAULT;
    TMR1_FOR_CCPx;
    ENABLE_CCP1_INTERRUPT;
}
void CCP2_Capture_Init_Default(void){
    CCP2M_CAPTURE_DEFAULT;
    TMR1_FOR_CCPx;
    ENABLE_CCP2_INTERRUPT;
}