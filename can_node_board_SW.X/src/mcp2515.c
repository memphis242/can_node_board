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
extern uint8_t receive_byte;
// </editor-fold>


// Functions
/**
 * Function: can_init_default
 * ----------------------------------------------------------------------------
 * <p>
 * Here I, assuming the MCP2515 is in configuration mode (should check first)
 * <ol>
 *      <li>set up interrupt pins on MCP2515 (~RXxBF pins)</li>
 *      <li>ensure SPI has been initialized,</li>
 *      <li>set up CLKEN, CLKPRE in CANCTRL so that the CLKOUT pin is disabled,</li>
 *         (otherwise, leave other bits at their reset values)
 *      <li>set up the bit-timing to be 250kbps (CNFGx),</li>
 *      <li>set up TX-related control (TXBnCTRL, TXRTSCTRL)</li>
 *      <li>set up RX-related control (RXBnCTRL, BFPCTRL, filters, masks)</li>
 *      <li>configure the MCP2515's interrupts CANINTE</li>
 *      <li>Save all this in internally-linked global variables to remember</li>
 *         current config
 *      <li>Request the op mode to be Normal</li>
 * </ol>
 * </p>
 * 
 * @param none
 * 
 * @return none
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

void can_set_baud_rate(uint32_t baudrate, uint8_t propsec, uint8_t syncjump){
    
}

/**
 * Function: mcp2515_cmd_reset
 * Sends a RESET command over SPI to the MCP2515.
 * 
 * @param none
 * 
 * @return none
 */
void mcp2515_cmd_reset(void){
    SPI_Transfer_Byte(MCP2515_SPI_RESET, &receive_byte);
    
}

uint8_t mcp2515_cmd_read_status(void);
uint8_t mcp2515_cmd_rx_status(void);
void mcp2515_cmd_read(uint8_t reg_address, uint8_t * buf);
void mcp2515_cmd_read_sequential(uint8_t start_reg_addr, uint8_t * rxbuf, uint8_t len);
void mcp2515_cmd_write(uint8_t reg_address, uint8_t val);
void mcp2515_cmd_write_sequential(uint8_t start_reg_addr, uint8_t * txbuf, uint8_t len);
void mcp2515_cmd_write_bit(uint8_t reg_address, uint8_t mask, uint8_t val);
uint8_t * mcp2515_cmd_read_rx_buf(rxbuf_t rxb);    // Programmer needs to have the write size buffer ready!! MCP2515_MSG_BUFF_SIZE_BYTES
void mcp2515_cmd_load_tx_buf(txbuf_t txb, uint8_t tx_buf);      // Programmer needs to have the write size buffer ready!! MCP2515_MSG_BUFF_SIZE_BYTES