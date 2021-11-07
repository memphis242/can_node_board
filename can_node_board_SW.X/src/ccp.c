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

// CCP2
/* Function: CCPx_Init_Default
 * ---------------------------------
 * Initialize the CCPx module to a default state. For this project, that means
 * setting the CCP2M[3:0] bits to 1010 (i.e., 0xB), which sets the CCPx module
 * in Compare mode, generates interrupt on compare match, and leaves the CCPx pin
 * as is. It also triggers a Special Trigger Event; for CCP1, this means
 * resetting the Timer registers associated with CCP1; for CCP2, this means
 * resetting the timer and starting an A/D conversion.
 * I also enable (unmask) the CCP2 interrupt because otherwise, there's
 * no point of using this peripheral.
 * 
 * NOTE: The timer that is used with the CCP module is NOT configured in the
 * CCPxCON register --> instead, it is in T3CON with the T3CCP2:T3CC1 bits, with
 * 00b representing Timer1 being the compare source for the CCP2 module.
 * 
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void CCP1_Init_Default(uint16_t comp_val){
    CCP1M_DEFAULT;
    CCP1_SET_COMP_VAL(comp_val);
    ENABLE_CCP1_INTERRUPT;
}
void CCP2_Init_Default(uint16_t comp_val){
    CCP2M_DEFAULT;
    CCP2_SET_COMP_VAL(comp_val);
    ENABLE_CCP2_INTERRUPT;
}
