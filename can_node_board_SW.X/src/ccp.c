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
/* Function: CCP2_Init_Default
 * ---------------------------------
 * Initialize the CCP2 module to a default state. For this project, that means
 * setting the CCP2M[3:0] bits to 1010 (i.e., 0xA), which sets the CCP2 module
 * in Compare mode, generate interrupt on compare match, and leave the CCP2 pin
 * as is. The CCP2 pin on the PIC18F4620 is on pin 36, multiplexed with RB3 and
 * AN9. I also enable (unmask) the CCP2 interrupt because otherwise, there's no
 * point of using this peripheral.
 * NOTE: The timer that is used with the CCP module is NOT configured in the
 * CCPxCON register --> instead, it is in T3CON with the T3CCP2:T3CC1 bits, with
 * 00b representing Timer1 being the compare source for the CCP2 module.
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void CCP2_Init_Default(void){
    CCP2M_DEFAULT;
    ENABLE_CCP2_INTERRUPT;
}

/* Function: CCP2_Compare_Val
 * ---------------------------------
 * Sets the Compare register (CCPR2) of the CCP2 module.
 * 
 * Parameters: uint16_t comp_val --> The 16-bit value to place into the CCPR2
 *                                   register.
 * 
 * Returns: none
 */
void CCP2_Compare_Val(uint16_t comp_val){
    CCPR2 = comp_val;    
}