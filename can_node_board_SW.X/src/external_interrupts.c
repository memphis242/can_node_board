/*
 * File:   external_interrupts.c
 * Author: Abdullah Almosalami
 *
 * Created on November 11, 2021, 11:15 AM
 */


#include <xc.h>
#include "external_interrupts.h"


// Global variables


// Functions

/* Function: external_interrupts_init_default
 * ---------------------------------
 * Here I set all the INTx pins to be utilized, on falling edge.
 *  
 * Parameters: none
 * 
 * Returns: none
 */
void external_interrupts_init_default(void){
    
    INTx_FALLING_EDGE;  // All on falling edge
    TRIS_INTx;  // Set RB0 to RB2 as inputs
    // Unmask the external interrupts
    INT0_ENABLE;
    INT1_ENABLE;
    INT2_ENABLE;
    
    // Just in case flags are not cleared
    CLEAR_INT0_FLAG;
    CLEAR_INT1_FLAG;
    CLEAR_INT2_FLAG;
    
}

/* Function: external_interrupts_init
 * ---------------------------------
 * This is a more generalized initialization routine for the external interrupts.
 * The which_pins parameter is checked for its three least significant bits to
 * correspond to which INTx should be initialized. If bit 0 is set, INT0 shall
 * be used. If bit 1 is set, INT1 shall be used, and likewise for bit 2.
 * The idea is you would send in the argument as an ORing of the constants
 * within the external_interrupt_edge_t enum. The trigger_edge parameter sets
 * the edge to trigger an interrupt from these pins.
 *  
 * Parameters: uint8_t which_pins -- Which INTx?
 *             external_interrupt_edge_t trigger_edge -- Which edge?
 * 
 * Returns: none
 */
void external_interrupts_init(uint8_t which_pins, external_interrupt_edge_t trigger_edge){
    if(TEST_BIT_SET(which_pins,0)) {    // INT0
        INTCON2bits.INTEDG0 = trigger_edge;
        TRIS_INT0;
        INT0_ENABLE;
        CLEAR_INT0_FLAG;
    }
    if(TEST_BIT_SET(which_pins,1)) {    // INT1
        INTCON2bits.INTEDG1 = trigger_edge;
        TRIS_INT1;
        INT1_ENABLE;
        CLEAR_INT1_FLAG;
    }
    if(TEST_BIT_SET(which_pins,2)) {
        INTCON2bits.INTEDG2 = trigger_edge;
        TRIS_INT2;
        INT2_ENABLE;
        CLEAR_INT2_FLAG;
    }
}
