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
extern uint8_t receive_byte;    // Used as part of SPI comms when I don't care about
                                // the received byte
// To help keep track of which buffers I'm loading up and which buffers got successfully sent...
uint8_t txbf0_full = 0x00u;
uint8_t txbf1_full = 0x00u;
uint8_t txbf2_full = 0x00u;
uint8_t txbf0_sent = 0x00u;
uint8_t txbf1_sent = 0x00u;
uint8_t txbf2_sent = 0x00u;

// To keep track of which receive buffers are waiting to be read
uint8_t rxbf0_full = 0x00u;
uint8_t rxbf1_full = 0x00u;

static uint8_t mcp2515_status = 0x00u;
// </editor-fold>


// Functions

// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="CONFIGURATION FUNCTIONS">
/**
 * <h3>Function: can_init_default</h3>
 * ----------------------------------------------------------------------------
 * <p>
 * Here I, assuming the MCP2515 is in configuration mode (should check first)
 * <ol>
 *      <li>ensure SPI has been initialized,</li>
 *      <li>check the MCP2515 is in configuration mode</li>
 *      <li>set up interrupt pins on MCP2515 (~RXxBF pins)</li>
 *      <li>set up CLKEN, CLKPRE in CANCTRL so that the CLKOUT pin is disabled,</li>
 *         (otherwise, leave other bits at their reset values)
 *      <li>set up the bit-timing to be 250kbps (CNFGx),</li>
 *      <li>set up TX-related control (TXBnCTRL, TXRTSCTRL)</li>
 *      <li>set up RX-related control (RXBnCTRL, BFPCTRL, filters, masks)</li>
 *      <li>configure the MCP2515's interrupts CANINTE</li>
 *      <li>Save all this in internally-linked global variables to remember current config</li>
 *      <li>Request the op mode to be Normal</li>
 * </ol>
 * </p>
 * 
 * @param none
 * 
 * @return none
 */
void can_init_default(void){
    
    // 1. Initialize SPI as a master
    SPI_Init_Master_Default();
    ei();   // Need to enable global interrupts for SPI to work!
    
    // 2. Make sure MCP2515 is in config mode by sending a RESET command and waiting...
    // If not in config mode, request to be in config mode and wait until this is confirmed
    do {
        mcp2515_cmd_reset();
        __delay_ms(100u); // Wait a little just to let the MCP2515 do its thing
    } while( mcp2515_current_opmode() != MCP2515_OPMODE_CONFIG);
    
    // 3. I use one external interrupt pin on the PIC for ~INT of the MCP2515
    di();   // Since I had previously enabled unmasked interrupts, and I'm about to
            // enable another interrupt, disable first before re-enabling...
    external_interrupts_init(EXT_INT_INT0, FALLING_EDGE);
    ei();
    
    // 4. Disable CLKOUT pin as it is enabled by default on POR --> i.e., CLKEN in
    //    CANCTRL = 0
    mcp2515_cmd_write_bit(MCP2515_CANCTRL, MCP2515_CLKOUT_ENABLE, 0x00u);
    
    /* 5. Set up bit-timing to 250kbps, assuming 40MHz MCP2515 oscillator
     * This amounts to BRP: 3, NPROPSEG = 5TQ, NPS1 = 8TQ, NPS2 = 6TQ --> SJW = 4TQ
     * BRP and SJW are set in CNF1, PROPSEG and PS1 in CNF2, and PS2 in both CNF2 and CNF3.
     * Note that CNF1, CNF2, and CNF3 can only be modified in configuration mode!
     */
    mcp2515_cmd_write_bit(MCP2515_CNF1, CNF1_SJW_BITS, CNF1_SET_SJW_4TQ);   // Set SJW to 4TQ
    mcp2515_cmd_write_bit(MCP2515_CNF1, CNF1_BRP_BITS, 0x03u);              // Set BRP to 3 --> 4
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_BTLMODE_BITS, 0x80u);          // PHSEG2 is determined by CNF3
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_SAM_BITS, 0x00u);              // Bus is only sampled once
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_PHSEG1_BITS, CNF2_SET_PHSEG1(7u)); // Set PS1 to 7 --> 8TQ
    mcp2515_cmd_write_bit(MCP2515_CNF2, CNF2_PRSEG_BITS, CNF2_SET_PROPSEG(4u)); // Set PROPSEG to 4 --> 5TQ
    mcp2515_cmd_write_bit(MCP2515_CNF3, CNF3_PHSEG2_BITS, CNF3_SET_PS2(5u));    // Set PS2 to 5 --> 6TQ
    
    /* 6. Set up TX-related control (TXBnCTRL, TXRTSCTRL)
     * For now, I will the TXBnCTRL registers at their default values. The only configuration
     * to do is the TXP bit-field, which sets the priority of the transmit buffers.
     * I don't have a priority in-mind at the moment so I will leave them all at 00
     * (the POR value), which sets all buffers at the same priority level (lowest priority).
     * 
     * For the TXRTSCTRL, I want the ~TXxRTS pins to be inputs (i.e., not RTS pins),
     * since I plan to use the RTS command over SPI to initiate a transmission.
     * This happens to also be the POR state of the register so nothing needed
     * to be done here either! Note if this needs to be changed later, the
     * TXRTSCTRL register can only be changed in configuration mode!
     */
    
    /* 7. Set up RX-related control (RXBnCTRL, BFPCTRL, filters, masks)
     * By default, I will just allow all messages through without filtering. The user can
     * reconfigure in filters and masks depending on the situation. Note that the filters
     * and masks can only be changed in configuration mode!
     * 
     * I will also enable rollover from RXB0 to RXB1.
     * 
     * I will also set the ~RXnBF pins to be disabled (on the board, they will be
     * left floating).
     */
    // Allow everything through
    mcp2515_cmd_write_bit(MCP2515_RXB0CTRL, MCP2515_RXBnCTRL_RXM, 0x00);
    mcp2515_cmd_write_bit(MCP2515_RXB1CTRL, MCP2515_RXBnCTRL_RXM, 0x00);
    // Enable rollover
    mcp2515_cmd_write_bit(MCP2515_RXB0CTRL, MCP2515_RXB0CTRL_BUKT, MCP2515_RX_ROLLOVER);
    // Set ~RXnBF pins to be disabled --> this is default POR state so no need to do anything
    
    /* 8. Configure the MCP2515's interrupts CANINTE
     * I plan to use the ~INT pin to process pretty much all the MCP2515 interrupts.
     * When it is set, in the ISR, I read the CANINTF register and handle the flags
     * that are set accordingly.
     */
    mcp2515_cmd_write(MCP2515_CANINTE, MCP2515_CANINTE_DEFAULT);
    mcp2515_cmd_write(MCP2515_CANINTF, 0x00u);  // Clear all flags in case any happen to be set
    
    /* 9. Save all this in internally-linked global variables to remember current config
     * I'll do this later...
     */
    
    // 10. Request the op mode to be Normal
    mcp2515_normal_mode();
    
    // Wait just a little...
    __delay_ms(100u);
    
}

void can_set_baud_rate(uint32_t baudrate, uint8_t propsec, uint8_t syncjump){
    
}

// </editor-fold>

// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="MCP2515 COMMAND FUNCTIONS">

/**
 * <h3>Function: mcp2515_current_opmode</h3>
 * ------------------------------------------------
 * <p>Returns the current operation mode of the MCP2515.</p>
 * 
 * @param none
 * 
 * @return opmode_t current_opmode -- The current operation mode
 */
opmode_t mcp2515_current_opmode(void){
    uint8_t opmode = 0x00;
    mcp2515_cmd_read(MCP2515_CANSTAT, &opmode);
    opmode_t current_opmode = (opmode_t) (opmode & MCP2515_CANSTAT_OPMODE_BITS);
    
    return current_opmode;
}

/**
 * <h3>Function: mcp2515_config_mode</h3>
 * ------------------------------------------------
 * <p>Requests to place the MCP2515 in configuration mode.</p>
 * 
 * @param none
 * 
 * @return none
 */
void mcp2515_config_mode(void){
    mcp2515_cmd_write_bit(MCP2515_CANCTRL, MCP2515_CANCTRL_REQOP_BITS, MCP2515_OPMODE_CONFIG);
}

/**
 * <h3>Function: mcp2515_normal_mode</h3>
 * ------------------------------------------------
 * <p>Requests to place the MCP2515 in normal mode.</p>
 * 
 * @param none
 * 
 * @return none
 */
void mcp2515_normal_mode(void){
    mcp2515_cmd_write_bit(MCP2515_CANCTRL, MCP2515_CANCTRL_REQOP_BITS, MCP2515_OPMODE_NORMAL);
}

/**
 * <h3>Function: mcp2515_cmd_reset</h3>
 * ------------------------------------------------
 * <p>Sends a RESET command over SPI to the MCP2515.</p>
 * 
 * @param none
 * 
 * @return none
 */
void mcp2515_cmd_reset(void){
    SPI_Transfer_Byte(MCP2515_SPI_RESET, &receive_byte);
}

/**
 * <h3>Function: mcp2515_cmd_read_status</h3>
 * ------------------------------------------------
 * <p>Polls several status-type bits and returns a byte containing those bits.</p>
 * 
 * @param none
 * 
 * @return uint8_t receive_byte -- The format of the byte is as follows:
 * <p>RX0IF--RX1IF--TXREQ_TXB0--TX0IF--TXREQ_TXB1--TX1IF--TXREQ_TXB2--TX2IF</p>
 */
uint8_t mcp2515_cmd_read_status(void){
    SPI_Transfer_Byte(MCP2515_SPI_READ_STATUS, &receive_byte);
    return receive_byte;
}

/**
 * <h3>Function: mcp2515_cmd_rx_status</h3>
 * ------------------------------------------------
 * <p>Polls for filter match and message type of received message. See datasheet for details!</p>
 * 
 * @param none
 * 
 * @return none
 */
uint8_t mcp2515_cmd_rx_status(void){
    SPI_Transfer_Byte(MCP2515_SPI_RX_STATUS, &receive_byte);
    return receive_byte;
}

/**
 * <h3>Function: mcp2515_cmd_read</h3>
 * ------------------------------------------------
 * <p>Reads the data from the register at the selected address.</p>
 * 
 * @param uint8_t reg_address -- This is the address of the register you wish to read from.
 * @param uint8_t * buf -- This is where that data will be stored in.
 * 
 * @return none
 */
void mcp2515_cmd_read(uint8_t reg_address, uint8_t * buf){
    
    SPI_MASTER_CS_LOW;
    
    // First send READ command
    SPI_Transfer_Byte_without_CS(MCP2515_SPI_READ, &receive_byte);
    // Then send address
    SPI_Transfer_Byte_without_CS(reg_address, &receive_byte);
    // Then finally read in data at address
    SPI_Transfer_Byte_without_CS(0x00u, buf);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_read_sequential</h3>
 * ------------------------------------------------
 * <p>Reads the data from successive registers starting at the selected address.
 * This is possible because the MCP2515 supports such behavior.</p>
 * 
 * @param uint8_t start_reg_addr -- This is the address of the register you will start reading from
 * @param uint8_t * rxbuf -- This is where that data will be stored in.
 * @param uint8_t len -- This is how many bytes to read. MAKE SURE rxbuf has such size!
 * 
 * @return none
 */
void mcp2515_cmd_read_sequential(uint8_t start_reg_addr, uint8_t * rxbuf, uint8_t len){
    
    SPI_MASTER_CS_LOW;
    
    // First send READ command
    SPI_Transfer_Byte_without_CS(MCP2515_SPI_READ, &receive_byte);
    // Then send address of first byte to read
    SPI_Transfer_Byte_without_CS(start_reg_addr, &receive_byte);
    // Then begin reading sequentially len times
    for(uint8_t i=0; i<len; i++) SPI_Transfer_Byte_without_CS(0x00u, &rxbuf[i]);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_write</h3>
 * ------------------------------------------------
 * <p>Writes data to the register at the selected address.</p>
 * 
 * @param uint8_t reg_address -- This is the address of the register you wish to write to.
 * @param uint8_t val -- This is what will be written to the register. CAREFUL when using
 *                       this command to write to control registers!
 * 
 * @return none
 */
void mcp2515_cmd_write(uint8_t reg_address, uint8_t val){
    
    SPI_MASTER_CS_LOW;
    
    // First send WRITE command
    SPI_Transfer_Byte_without_CS(MCP2515_SPI_WRITE, &receive_byte);
    // Then send address
    SPI_Transfer_Byte_without_CS(reg_address, &receive_byte);
    // Then send data to be written
    SPI_Transfer_Byte_without_CS(val, &receive_byte);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_write_sequential</h3>
 * ------------------------------------------------
 * <p>Writes data to successive registers starting at the selected address.
 * This is possible because the MCP2515 supports such behavior.</p>
 * 
 * @param uint8_t start_reg_addr -- This is the address of the register you will start writing to
 * @param uint8_t * txbuf -- This is the data that will be written to the registers
 * @param uint8_t len -- This is how many registers you write. BE CAREFUL NOT TO GO BEYOND what you exactly intend!
 * 
 * @return none
 */
void mcp2515_cmd_write_sequential(uint8_t start_reg_addr, uint8_t * txbuf, uint8_t len){
    
    SPI_MASTER_CS_LOW;
    
    // First send WRITE command
    SPI_Transfer_Byte_without_CS(MCP2515_SPI_WRITE, &receive_byte);
    // Then send address of first byte to write to
    SPI_Transfer_Byte_without_CS(start_reg_addr, &receive_byte);
    // Then begin writing sequentially len times
    for(uint8_t i=0; i<len; i++) SPI_Transfer_Byte_without_CS(txbuf[i], &receive_byte);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_write_bit</h3>
 * ------------------------------------------------
 * <p>Writes data to the register at the selected address, similar to
 * mcp2515_cmd_write, but while utilizing the mask to ensure only certain
 * bits are written to and not the entire register. Not all registers can work
 * with this command - use only with control registers (e.g., CANCTRL, TXBxCTRL, ...)
 * and interrupt-related registers (e.g., CANINTE, CANINTF). NOTE that CANSTAT does
 * NOT work with this command!</p>
 * 
 * @param uint8_t reg_address -- This is the address of the register you wish to write to.
 * @param uint8_t mask -- Whichever bits are 1 in the mask, those bits will be allowed to get
 *                        written to in reg_address from val. For those bits that are 0, the
 *                        corresponding bits in val will be ignored.
 * @param uint8_t val -- This is what will be written to the register.
 * 
 * @return none
 */
void mcp2515_cmd_write_bit(uint8_t reg_address, uint8_t mask, uint8_t val){
    
    SPI_MASTER_CS_LOW;
    
    // First send BIT_MODIFY command
    SPI_Transfer_Byte_without_CS(MCP2515_SPI_BIT_MODIFY, &receive_byte);
    // Then send address
    SPI_Transfer_Byte_without_CS(reg_address, &receive_byte);
    // Then send mask
    SPI_Transfer_Byte_without_CS(mask, &receive_byte);
    // Then send data to be written
    SPI_Transfer_Byte_without_CS(val, &receive_byte);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_read_rx_buf</h3>
 * ------------------------------------------------
 * <p>Reads the RX buffer specified by rxb. This is equivalent to the READ command
 * but is a little faster since you will not need to send the address byte. Be prepared
 * with the right sized buffer! I have defined MCP2515_MSG_BUFF_SIZE_BYTES for this
 * purpose. Note that the MCP2515 allows you to start reading the buffer in its complete
 * form or just the data bytes. For now, I will have this function always read the full
 * buffer with the ID included.</p>
 * 
 * @param rxbuf_t rxb -- This specifies which RX buffer to read --> RXB0 or RXB1?
 * @param uint8_t * rx_buf -- This is where the buffer data will be stored. Again,
 *                            make sure this is the right size! MCP2515_MSG_BUFF_SIZE_BYTES
 * 
 * @return none
 */
void mcp2515_cmd_read_rx_buf(rxbuf_t rxb, uint8_t * rx_buf){
    
    SPI_MASTER_CS_LOW;
    
    // First send READ_RXBUF command with SPI_READ_RXBx_ID to indicate we are reading
    // the full buffer
    SPI_Transfer_Byte_without_CS((uint8_t) MCP2515_SPI_READ_RXBUF(rxb ? SPI_READ_RXB1_ID : SPI_READ_RXB0_ID), &receive_byte);
    // Then read in bytes!
    for(uint8_t i=0; i<MCP2515_MSG_BUFF_SIZE_BYTES; i++) SPI_Transfer_Byte_without_CS(0x00u, &rx_buf[i]);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_load_tx_buf</h3>
 * ------------------------------------------------
 * <p>Load the TX buffer specified by txb. This is equivalent to the WRITE command
 * but is a little faster since you will not need to send the address byte. Be prepared
 * with the right sized buffer! I have defined MCP2515_MSG_BUFF_SIZE_BYTES for this
 * purpose. Note that the MCP2515 allows you to start loading the buffer in its complete
 * form or just the data bytes. For now, I will have this function always load the full
 * buffer with the ID included.</p>
 * 
 * @param txbuf_t txb -- This specifies which TX buffer to load --> TXB0, TXB1, or TXB2
 * @param uint8_t * tx_buf -- This is what will be loaded into the buffer. Again,
 *                            make sure this is the right size! MCP2515_MSG_BUFF_SIZE_BYTES
 * 
 * @return none
 */
void mcp2515_cmd_load_tx_buf(txbuf_t txb, uint8_t * tx_buf){      // Programmer needs to have the write size buffer ready!! MCP2515_MSG_BUFF_SIZE_BYTES
    spi_load_txb_inst_t selected_txb;
    
    // Set which buffer will be loaded
    switch(txb){
        case TXB0:
            selected_txb = SPI_LOAD_TXB0_ID;
            break;
        case TXB1:
            selected_txb = SPI_LOAD_TXB1_ID;
            break;
        case TXB2:
            selected_txb = SPI_LOAD_TXB2_ID;
            break;
    }
    
    SPI_MASTER_CS_LOW;
    
    // First send LOAD_TX_BUFFER command with SPI_LOAD_TXBx_ID to indicate we are loading
    // the full buffer
    SPI_Transfer_Byte_without_CS(MCP2515_SPI_LOAD_TXBUF(selected_txb), &receive_byte);
    // Then load in bytes!
    for(uint8_t i=0; i<MCP2515_MSG_BUFF_SIZE_BYTES; i++) SPI_Transfer_Byte_without_CS(tx_buf[i], &receive_byte);
    
    SPI_MASTER_CS_HIGH;
    
}

/**
 * <h3>Function: mcp2515_cmd_rts</h3>
 * ------------------------------------------------
 * <p>Sends a Request-to-Send command which initiates a transmission.</p>
 * 
 * @param txbuf_t txb -- This specifies which TX buffer to send --> TXB0, TXB1, or TXB2
 * 
 * @return none
 */
void mcp2515_cmd_rts(txbuf_t txb){
    SPI_Transfer_Byte(MCP2515_SPI_RTS(txb), &receive_byte);
}
// </editor-fold>

// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="INTERMEDIATE LAYER">

/* These functions serves as the intermediary between the final MCP2515 command, which
 * will utilize an array buffer to send the individual bytes, and the application
 * layer, which makes use of can_msg structs since it's a little more intuitive to
 * work with.
 */


void can_compose_msg_std(can_msg * msg, uint8_t * mcp2515_tx_buf){
    // I'll leave this for now...
}


void can_parse_msg_std(can_msg * msg, uint8_t * mcp2515_rx_buf){
    // I'll leave this for now...
}

/**
 * <h3>Function: can_compose_msg_ext</h3>
 * <p>Convert a can_msg struct into the 13 bytes needed to go into a tx_buf before
 * sending the msg off. User needs to take care that mcp2515_tx_buf is large
 * enough! MCP2515_MSG_BUFF_SIZE_BYTES!</p>
 * 
 * <p>Recall that a buffer in the MCP2515 goes (in byte order) (refer to MCP2515
 * data sheet or my mcp2515.h header file for more detailed register layout):
 * <ol>
 *      <li>SIDH: SID10:SID3</li>
 *      <li>SIDL: SID2:SID0--undef--EXIDE--EID17:EID16</li>
 *      <li>EID8: EID15:EID8</li>
 *      <li>EID0: EID7:EID0 </li>
 *      <li>DLC: undef--RTR--undef-undef--DLC3:DLC0</li>
 *      <li>Data Bytes 0 to 7 (8 bytes)</li>
 * 
 * @param can_msg * msg -- The struct holding the CAN message
 * @param uint8_t * mcp2515_tx_buf -- The TX buffer that will be used with the
 *                                    LOAD_TX_BUFFER command.
 * 
 * @return none
 */
void can_compose_msg_ext(can_msg * msg, uint8_t * mcp2515_tx_buf){
    
    // Warning: Some yellow-belt bit Kung-Fu up ahead
    mcp2515_tx_buf[0] = (uint8_t) ( (msg->arb_field.sid & 0x07F8u) >> 3u );      // SID10:SID3
    mcp2515_tx_buf[1] = (uint8_t) ( ((uint32_t)(msg->arb_field.sid & 0x0007u) << 5u) |   // SID2:SID0
                        ((uint32_t)(msg->arb_field.exide) << 3u)         |   // EXIDE
                        ((msg->arb_field.eid & 0x30000) >> 16u) );   // EID17:EID16
    mcp2515_tx_buf[2] = (uint8_t) ( (msg->arb_field.eid & 0x0FF00) >> 8u );      // EID15:EID8
    mcp2515_tx_buf[3] = (uint8_t) (msg->arb_field.eid & 0x000FF);              // EID7:EID0
    mcp2515_tx_buf[4] = (uint8_t) ( (msg->arb_field.rtr << 6u)    |           // RTR
                        (msg->ctrl_field.dlc) );                     // DLC
    mcp2515_tx_buf[5] = msg->data_field.data0;
    mcp2515_tx_buf[6] = msg->data_field.data1;
    mcp2515_tx_buf[7] = msg->data_field.data2;
    mcp2515_tx_buf[8] = msg->data_field.data3;
    mcp2515_tx_buf[9] = msg->data_field.data4;
    mcp2515_tx_buf[10] = msg->data_field.data5;
    mcp2515_tx_buf[11] = msg->data_field.data6;
    mcp2515_tx_buf[12] = msg->data_field.data7;
    
}

/**
 * <h3>Function: can_parse_msg_ext</h3>
 * <p>Convert a mcp2515_rx_buf into a can_msg struct. User needs to take care
 * that mcp2515_tx_buf is large enough! MCP2515_MSG_BUFF_SIZE_BYTES!</p>
 * 
 * <p>Recall that a buffer in the MCP2515 goes (in byte order) (refer to MCP2515
 * data sheet or my mcp2515.h header file for more detailed register layout):
 * <ol>
 *      <li>SIDH: SID10:SID3</li>
 *      <li>SIDL: SID2:SID0--undef--EXIDE--EID17:EID16</li>
 *      <li>EID8: EID15:EID8</li>
 *      <li>EID0: EID7:EID0 </li>
 *      <li>DLC: undef--RTR--undef-undef--DLC3:DLC0</li>
 *      <li>Data Bytes 0 to 7 (8 bytes)</li>
 * 
 * @param can_msg * msg -- The struct holding the CAN message
 * @param uint8_t * mcp2515_rx_buf -- The RX buffer that was obtained with an
 *                                    READ_RX_BUFFER command.
 * 
 * @return none
 */
void can_parse_msg_ext(can_msg * msg, uint8_t * mcp2515_rx_buf){
    // Warning: More bit Kung-Fu up ahead...
    msg->arb_field.sid = (((uint16_t) mcp2515_rx_buf[0]) << 3u) | (((uint16_t) mcp2515_rx_buf[1] & 0x00E0) >> 5u);
    msg->arb_field.exide = (mcp2515_rx_buf[1] & 0x08) >> 3u;
    msg->arb_field.eid = (((uint32_t) mcp2515_rx_buf[1] & 0x0003) << 16u)    |
                         (((uint32_t) mcp2515_rx_buf[2]) << 8u)     |
                         ((uint32_t) mcp2515_rx_buf[3]);
    msg->arb_field.rtr = (mcp2515_rx_buf[4] & 0x40) >> 6u;
    msg->ctrl_field.dlc = mcp2515_rx_buf[4] & 0x0F;
    msg->data_field.data0 = mcp2515_rx_buf[5];
    msg->data_field.data1 = mcp2515_rx_buf[6];
    msg->data_field.data2 = mcp2515_rx_buf[7];
    msg->data_field.data3 = mcp2515_rx_buf[8];
    msg->data_field.data4 = mcp2515_rx_buf[9];
    msg->data_field.data5 = mcp2515_rx_buf[10];
    msg->data_field.data6 = mcp2515_rx_buf[11];
    msg->data_field.data7 = mcp2515_rx_buf[12];
    
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="API LAYER">

/**
 * <h3>Function: can_send</h3>
 * <p>This queues up a message to be sent. This is NOT a guarantee that the message
 * sent right away. For that, check the CANINTF register in the MCP2515. Otherwise,
 * I am handling this through static variable flags and the ISR.</p>
 * 
 * <p> NOTE! For the sake of speed, I do not check that we are in the right opmode
 * to send. Application should check CANSTAT_OPMODE bits if unsure before calling
 * this function!</p>
 * 
 * <p>Also note, if all the TX buffers are free, then you can call this function
 * three times in a row and it will load TXB0 --> TXB1 --> TXB2. After that, it
 * will return EXEC_FAIL, indicating it was unable to request sends because
 * buffers are currently all loaded.</p>
 * 
 * @param can_msg msg --> Message to send, placed in a can_msg struct
 * @return 
 */
uint8_t can_send(can_msg * msg){    // TODO: Include priority at some point...
    // Translate msg into a buffer
    uint8_t tx_msg_buf[MCP2515_MSG_BUFF_SIZE_BYTES] = {0u};
    can_compose_msg_ext(msg, tx_msg_buf);
    
    // Check for non-empty buffers
    // TODO - Not sure how to do this quickly through the MCP2515...
    // For now, I'll use internal static variables to keep track of which buffer
    // I'm requesting to get filled every time
    txbuf_t txbf_to_use = TXB0;     // By default, use TXB0
    if(txbf0_full){     // Check if TXB0 has been loaded already
        if(txbf1_full){ // If TXB0 full, is TXB1 full?
            if(txbf2_full){
                return EXEC_FAIL;   // If TXB2 is also full, then return EXEC_FAIL
                                    // and do not proceed further
            } else {
                txbf_to_use = TXB2;     // If yes, use TXB2
                txbf2_full = 0x01u;  // Set flag indicating it is now assumed to be full
            }
        } else {
            txbf_to_use = TXB1;     // If not, then use TXB1
            txbf1_full = 0x01u;  // Set flag indicating it is now assumed to be full
        }
    } else {
        txbf0_full = 0x01u;  // Set flag indicating it is now assumed to be full
    }
    
    // Load into an empty buffer
    mcp2515_cmd_load_tx_buf(txbf_to_use, tx_msg_buf);
    
    // Request to send
    mcp2515_cmd_rts(txbf_to_use);
    
    // Await confirmation from TXnIF getting set
    // Account for errors or arbitration loss
    // If successful, reset flags, including TXnIF
    // TODO: ---> This is MUCH better to do in the ISR...
    
    return EXEC_SUCCESS;
}

/**
 * <h3>Function: can_receive</h3>
 * <p>This is the routine that is called to handle the event a message is received.</p>
 * 
 * @param can_msg msg --> Message to send, placed in a can_msg struct
 * @return 
 */
uint8_t can_receive(can_msg * msg){
    
    if(rxbf0_full) {
        
    }
    
    return EXEC_SUCCESS;
}

//uint8_t can_remote_frame(can_msg_arb_field arb_field);
//uint8_t can_tx_cancel(void);
//uint8_t can_tx_available(void);
//uint8_t can_rx_pending(void);
//uint8_t can_rx_setmask(rx_mask_t mask_id, uint32_t mask, uint8_t is_extended);
//uint8_t can_rx_setfilter(rx_filt_t filt_id, uint32_t filter);
//uint8_t can_rx_mode(void);
//uint8_t can_mcp2515_config_options(mcp_2515_options_t option, uint8_t val);
//uint8_t can_read_error(uint8_t reg);
//uint8_t can_clear_bus_error(void);

// </editor-fold>

