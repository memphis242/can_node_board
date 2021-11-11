/*
 * File:   mcp2515.c
 * Author: Abdullah Almosalami
 *
 * Created on November 06, 2021, 5:10 PM
 *
 */


#include <xc.h>
#include "mcp2515.h"


// <editor-fold defaultstate="collapsed" desc="GLOBAL & STATIC VARIABLES">

// </editor-fold>


// Functions
/* Function: can_init_default
 * ---------------------------------
 * Here I
 *      1- set up interrupt pins on MCP2515 (~RXxBF pins)
 *      2- ensure SPI has been initialized,
 *      3- set up CLKEN, CLKPRE in CANCTRL so that the CLKOUT pin is disabled,
 *         (otherwise, leave other bits at their reset values)
 *      4- set up the bit-timing to be 250kbps (CNFGx),
 *      5- set up TX-related control (TXBnCTRL, TXRTSCTRL)
 *      6- set up RX-related control (RXBnCTRL, BFPCTRL, filters, masks)
 *      7- configure the MCP2515's interrupts CANINTE
 *      8- Save all this in internally-linked global variables to remember
 *         current config
 *      9- Request the op mode to be Normal
 * 
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void can_init_defaut(void){
    
    // I use external interrupt pins on the PIC for ~RX0BF, ~RX1BF, and ~RESET
    
    
}

void can_spi_command(uint8_t cmd){
    
}

uint8_t can_spi_query(uint8_t query){
    
}