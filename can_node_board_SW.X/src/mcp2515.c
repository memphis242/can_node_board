/*
 * File:   mcp2515.c
 * Author: Abdullah Almosalami
 *
 * Created on November 06, 2021, 5:10 PM
 *
 */


#include <xc.h>
#include "mcp2515.h"
#include "lcd_driver.h"
#include "ccp.h"
#include "timer.h"
#include "mssp_spi.h"
#include "external_interrupts.h"


// <editor-fold defaultstate="collapsed" desc="GLOBAL & STATIC VARIABLES">

// </editor-fold>


// Functions
/* Function: can_init_default
 * ---------------------------------
 * Here I, assuming the MCP2515 is in configuration mode (should check first)
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
void can_init_default(void){
    
    // Check if MCP2515 is in config mode...
    
    // 1. I use external interrupt pins on the PIC for ~RX0BF, ~RX1BF, and ~RESET
    external_interrupts_init_default();
    
    // 2. Now initialize SPI as a master
    SPI_Init_Master_Default();
    
    // 3. Disable CLKOUT pin as it is enabled by default on POR --> i.e., CLKEN in
    //    CANCTRL = 0
    mcp2515_cmd_write_bit(MCP2515_CANCTRL, MCP2515_CLKOUT_ENABLE, 0x00u);
    
    /* 4. Set up bit-timing to 250kbps, assuming 40MHz MCP2515 oscillator
     * This amounts to BRP: 3, NPROPSEG = 5TQ, NPS1 = 8TQ, NPS2 = 6TQ --> SJW = 4TQ
     * BRP and SJW are set in CNF1, PROPSEG and PS1 in CNF2, and PS2 in both CNF2 and CNF3
     */
    mcp2515_cmd_write_bit(MCP2515_CNF1, CNF1_SJW_BITS, CNF1_SET_SJW_4TQ);   // Set SJW to 4TQ
    mcp2515_cmd_write_bit(MCP2515_CNF1, CNF1_BRP_BITS, 0x03u);              // Set BRP to 3 --> 4
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_BTLMODE_BITS, 0x80u);          // PHSEG2 is determined by CNF3
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_SAM_BITS, 0x00u);              // Bus is only sampled once
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_PHSEG1_BITS, CNF2_SET_PHSEG1(7u)); // Set PS1 to 7 --> 8TQ
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_PRSEG_BITS, CNF2_SET_PROPSEG(4u)); // Set PROPSEG to 4 --> 5TQ
    mcp2515_cmd_write_bit(MCP2515_CNF3, CNF3_PHSEG2_BITS, CNF3_SET_PS2(5u));    // Set PS2 to 5 --> 6TQ
    
    
    
}

void can_spi_command(uint8_t cmd){
    
    
    
}

uint8_t can_spi_query(uint8_t query){
    
    return EXEC_SUCCESS;
}