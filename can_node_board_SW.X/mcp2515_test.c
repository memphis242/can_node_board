/*
 * File:   mcp2515_test.c
 * Author: Abdullah Almosalami
 *
 * Created on November 11, 2021, 8:13 PM
 * 
 * I will have Node 1 send every 100ms the state of its two buttons, an "engine speed"
 * signal (i.e., analog input from 10k pot), and two counters - a tx_counter and
 * ack_counter. The tx_counter will be incremented on every successful message
 * transmission, and the ack_counter will be incremented every time Node 2 sends
 * an acknowledge message. Node 1 also receives a message from Node 2 every 500ms
 * to toggle an LED.
 * 
 * I will have Node 2 receive the Node 1 state message 
 * 
 */


// <editor-fold defaultstate="collapsed" desc="PIC18F4620 Configuration Bit Settings">
// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTBE  // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
// </editor-fold>


#include <xc.h>
#include <stdint.h>
#include "mcp2515.h"
#include "lcd_driver.h"
#include "ccp.h"
#include "timer.h"
#include "mssp_spi.h"
#include "external_interrupts.h"
#include "mcp2515_test.h"
#include "adc.h"


#ifndef _XTAL_FREQ
#define _XTAL_FREQ                  40000000U   // 40MHz --> Defining this will allow use of __delay_ms())
#endif


// I will use the following to make it more convenient to write both the code
// the Test Node 1 and the Test Node 2 in the same file
#define NODE1         0u
#define NODE2         1u
#define CURRENT_NODE    NODE1
//#define CURRENT_NODE    NODE2

//#define TX_RX_DEBUG


// Extra defines
#ifndef ENABLE_PERIPHERAL_INTERRUPTS
#define ENABLE_PERIPHERAL_INTERRUPTS    (INTCONbits.PEIE = 1u)
#define DISABLE_PERIPHERAL_INTERRUPTS   (INTCONbits.PEIE = 0u)
#endif

// External variables
// For SPI
extern uint8_t spi_transfer_complete_flag;
// For the MCP2515
static volatile uint8_t mcp2515_interrupt_event = 0x00u;  // Used to represent if ~INT pin was cleared
extern uint8_t txbf0_full;
extern uint8_t txbf1_full;
extern uint8_t txbf2_full;
extern uint8_t txbf0_sent;
extern uint8_t txbf1_sent;
extern uint8_t txbf2_sent;
extern uint8_t rxbf0_full;
extern uint8_t rxbf1_full;

#if CURRENT_NODE == NODE1
static volatile uint8_t tmr_100ms_next = 0x00;  // This is used to indicate when 100ms has passed
static can_msg node1_rxbf0_msg;
static can_msg node1_rxbf1_msg;
static can_msg node1_state_msg;     // byte 0: button states, bytes 1 & 2: eng_spd,
                                    // bytes 3 & 4: tx_counter, bytes 5 & 6: ack_counter
static volatile uint8_t node1_ready_to_tx = 0x00;
static volatile uint16_t eng_spd_pot_reading = 0x0000u;
static uint16_t tx_counter = 0x0000u;   // Counter to keep track of how many messages have been sent
static uint16_t ack_counter = 0x0000u;  // Counter to keep track of how many messages have been
                                        // acknowledged by Node2
static uint8_t mcp2515_intf_buf = 0x00u;
static uint8_t mcp2515_eflg_buf = 0x00u;
static uint8_t mcp2515_txb0ctrl_buf = 0x00u;
static uint8_t mcp2515_txb1ctrl_buf = 0x00u;
static uint8_t mcp2515_txb2ctrl_buf = 0x00u;
static uint8_t mcp2515_canctrl_buf = 0x00u;
static uint8_t mcp2515_one_shot_mode = 0x00u;
static uint8_t mcp2515_txb0_reattempt_counter = 0x00u;
static uint8_t mcp2515_txb1_reattempt_counter = 0x00u;
static uint8_t mcp2515_txb2_reattempt_counter = 0x00u;
static uint8_t msg_received = 0x00u;
static uint8_t rxbf0_received = 0x00u;
static uint8_t rxbf1_received = 0x00u;
#endif

#if CURRENT_NODE == NODE2
static uint8_t node1_rx_buf = 0x00;
static uint8_t spi_rx_invalid_flag = 0x00;  // Flag to indicate invalid rx
static uint8_t spi_rx_flag = 0x00;  // Flag to indicate an rx has occured
static char debug_msg_1[16] = {'R','x',':',' '};
static char debug_msg_2[16] = {'R','x',':',' ', '0','x'};
static char button1_state = 0x00;
static char button2_state = 0x00;
// The following are for the display
#define NUM_OF_CHAR_USED_PER_LINE   5
#define BUTTON_STATE_CHAR           4
static char button1_msg[NUM_OF_CHAR_USED_PER_LINE] = {'B','1',':',' '};
static char button2_msg[NUM_OF_CHAR_USED_PER_LINE] = {'B','2',':',' '};
static const char spi_rx_invalid_msg[] = "INVALID MSG!";
#define LEN_OF_INV_MSG              12  // Not including null-terminator character
#endif


// Test functions
static char hex_to_char(uint8_t hex_num);


/******************************************************************************
 * Interrupt Routine
 * 
 * TODO: 
 */
void __interrupt() isr(void){
    
    //-------------------------------------------------------------------------
    // INTERRUPTS HANDLED BY BOTH NODES
    //-------------------------------------------------------------------------
    // SPI interrupt to indicate a byte transfer has completed...
    if(MSSP_IF_BIT && MSSP_INT_ENABLE_BIT) {
        // Successful byte TXd and RXd
        spi_transfer_complete_flag = 0x01;  // Set the transf flag
        
        CLEAR_MSSP_IFLAG;
    }
    
    // ~INT pin interrupt handling from MCP2515
    // For now, I'll try just setting a flag here and doing the full event handle
    // in main()
    if(INT0_FLAG && INT0_ENABLE_BIT){
        mcp2515_interrupt_event = 0x01u;
        
        CLEAR_INT0_FLAG;
    }
    
    
    //-------------------------------------------------------------------------
    // INTERRUPTS HANDLED BY NODE1 ONLY
    //-------------------------------------------------------------------------
#if CURRENT_NODE == NODE1
    // CCP1 used to set 100ms transmission rate period
    if(CCP1_IF_BIT && CCP1_INT_ENABLE_BIT){
        // On every other compare match, transmit!
        if(tmr_100ms_next){
            // Reset flag
            tmr_100ms_next = 0x00;
            
            // Set flag to indicate ready to transmit
            node1_ready_to_tx = 0x01;
            
            CLEAR_CCP1_IF;
            
        } else{
            // Set flag so next interrupt, we transmit
            tmr_100ms_next = 0x01;
            node1_ready_to_tx = 0x00; // Just make sure this flag is still cleared
            
            CLEAR_CCP1_IF;
        }
    }
    
    // CCP2 used to trigger ADC conversion on AN0 (RA0)
    if(CCP2_IF_BIT && CCP2_INT_ENABLE_BIT) {
        CLEAR_CCP2_IF;
    }
    
    // ADC interrupt, assuming right justified ADC Result
    if(ADC_IF && ADC_INTERRUPT_ENABLE_BIT) {
        eng_spd_pot_reading = ADRES;
        
        ADC_CLEAR_IF;
    }
        
#endif
    
#if CURRENT_NODE == NODE2
    //-------------------------------------------------------------------------
    // INTERRUPTS HANDLED BY NODE2 ONLY
    //-------------------------------------------------------------------------
    
    
#endif
    
    return;
}


/******************************************************************************
 * Main Routine
 */
void main(void) {
    
    // Regardless of which node, the following code should be compiled...
    can_init_default();     // Initializes SPI Master mode and the MCP2515
    
#if CURRENT_NODE == NODE1
    /* ------------------------------------------------------------------------
     * Code for Node 1
     * Node 1 will 
     * 
     *      1) send a message every 100ms indicating the state of  two
     *      switches it is connected to (byte 0), engine speed (bytes 1 & 2),
     *      tx_counter (bytes 3 & 4), and ack_counter (bytes 5 & 6).
     * 
     *      2) await an acknowledge message from Node 2 and if an acknowledge
     *      was received, Node 1 will increment a counter.
     * 
     *      3) respond to Node 2's command message to toggle Node 1's LED
     * 
     * TODO: INCLUDE TX TIMESTAMPS
     */
    
    //<editor-fold defaultstate="collapsed" desc="NODE 1 INITIALIZATION">
    // Set up I/O for buttons
    NODE1_BUTTON1_TRIS;
    NODE1_BUTTON2_TRIS;
    
    // Configure TIMER1 and CCP1 to trigger a transmission every 100ms (10Hz)
    Timer1_Init_Default();
    CCP1_Compare_Init_Default(DEFAULT_CONFIG_PERIOD_50ms);
    
    // Configure ADC and CCP2 to sample at 100Hz (10ms period)
    Timer3_Init_Default();
    CCP2_Compare_Init_Default(DEFAULT_CONFIG_PERIOD_10ms);
    adc_init_default();
    
    
    // Once done with all other initializations, turn on Timer1 and Timer3 and
    // enable all unmasked interrupts like from the MSSP module
    TMR1_ON;
    TMR3_ON;
    ENABLE_PERIPHERAL_INTERRUPTS;
    ei();
    // </editor-fold>
    
    // Node1's arbitration ID will always be the same since there's only one message
    // here from it. I'll also always set the DLC field to 8 bytes.
    node1_state_msg.arb_field.sid = NODE1_SID;
    node1_state_msg.arb_field.eid = NODE1_EID;
    node1_state_msg.arb_field.exide = 1u;   // Always extended frame format!
    node1_state_msg.arb_field.rtr = 0u;     // Always a data frame and not a remote frame
    
    node1_state_msg.ctrl_field.dlc = 0x8u;  // 8 data bytes in message
    
    // TX Node while(1) loop which will include event handling
    while(1){
        
        // <editor-fold defaultstate="collapsed" desc="READY TO TX">
        if(node1_ready_to_tx) {
            // Construct state message
            // Button states (byte 0)
            node1_state_msg.data_field.data0 = (uint8_t) NODE1_CONSTRUCT_BUTTON_BYTE;
            // Engine Speed (bytes 1 & 2) --> recall that J1939 will be Intel byte order (i.e., LSB first)
            // but the individual bytes themselves are big-endian
            node1_state_msg.data_field.data1 = (uint8_t) (eng_spd_pot_reading & 0x00FF);
            node1_state_msg.data_field.data2 = (uint8_t) ((eng_spd_pot_reading & 0xFF00) >> 8u);
            // tx_counter (bytes 3 & 4)
            node1_state_msg.data_field.data3 = (uint8_t) (tx_counter & 0x00FF);
            node1_state_msg.data_field.data4 = (uint8_t) ((tx_counter & 0xFF00) >> 8u);
            // ack_counter (bytes 5 & 6)
            node1_state_msg.data_field.data5 = (uint8_t) (ack_counter & 0x00FF);
            node1_state_msg.data_field.data6 = (uint8_t) ((ack_counter & 0xFF00) >> 8u);
            // Last byte --> just nothing I guess
            node1_state_msg.data_field.data7 = 0x00u;
            
            // Send message
            can_send(&node1_state_msg);
            
            // Clear event flag
            node1_ready_to_tx = 0x00;
        }
        // </editor-fold>
        
        // <editor-fold defaultstate="collapsed" desc="MCP2515 ~INT HANDLE">
        if(mcp2515_interrupt_event) {            
            // Read CANINTF from the MCP2515
            // TODO: See if ICOD would also be useful here or ... ?
            mcp2515_cmd_read(MCP2515_CANINTF, &mcp2515_intf_buf);
            
            
            // Check the various flags...
            // <editor-fold defaultstate="collapsed" desc="Message Error">
            if(mcp2515_intf_buf & MCP2515_MERR) {      // Message error? --> i.e., message failed to send
                // Check whether we are in one-shot mode or not
                mcp2515_cmd_read(MCP2515_CANCTRL, &mcp2515_canctrl_buf);
                mcp2515_one_shot_mode = mcp2515_canctrl_buf & MCP2515_ONE_SHOT_MODE;
                
                // If in one-shot mode, We will re-attempt three times at most for each TX buffer
                // if a message error has occurred (totaling three attempts per message);
                // If not in one-shot mode, we will let the MCP2515 handle things and
                // just clear the MERR flag
                // NOTE: The TXERR bit in each TXBnCTRL register automatically gets
                //      reset whenever another re-attempt to transmit has been made
                //      for that buffer, so no need to reset it manually here
                if(mcp2515_one_shot_mode) {
                    // Figure out for which TX Buffer the error has occurred...
                    mcp2515_cmd_read(MCP2515_TXB0CTRL, &mcp2515_txb0ctrl_buf);
                    mcp2515_cmd_read(MCP2515_TXB1CTRL, &mcp2515_txb1ctrl_buf);
                    mcp2515_cmd_read(MCP2515_TXB2CTRL, &mcp2515_txb2ctrl_buf);

                    if(mcp2515_txb0ctrl_buf & MCP2515_TXBnCTRL_TXERR) {
                        mcp2515_txb0_reattempt_counter++;
                        
                        if(mcp2515_txb0_reattempt_counter < 4) {
                            can_send(&node1_state_msg); // Re-attempt transmission
                        } else {
                            // TODO...
                        }
                        
                    }
                    if(mcp2515_txb1ctrl_buf & MCP2515_TXBnCTRL_TXERR) {
                        
                        mcp2515_txb1_reattempt_counter++;
                        
                        if(mcp2515_txb1_reattempt_counter < 4) {
                            can_send(&node1_state_msg); // Re-attempt transmission
                        } else {
                            // TODO...
                        }
                        
                    }
                    if(mcp2515_txb2ctrl_buf & MCP2515_TXBnCTRL_TXERR) {
                        
                        mcp2515_txb2_reattempt_counter++;
                        
                        if(mcp2515_txb2_reattempt_counter < 4) {
                            can_send(&node1_state_msg); // Re-attempt transmission
                        } else {
                            // TODO...
                        }
                        
                    }
                    
                }
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_MERR, 0x00u);
            }
            // </editor-fold>
            
            // <editor-fold defaultstate="collapsed" desc="Error Flag">
            if(mcp2515_intf_buf & MCP2515_ERRI) {       // Error shown from EFLG
                // Read the EFLG register to determine which error has occurred
                mcp2515_cmd_read(MCP2515_EFLG, &mcp2515_eflg_buf);
                
                if(mcp2515_eflg_buf & MCP2515_EFLG_RX1OVR) {
                    // TODO...
                }
                if(mcp2515_eflg_buf & MCP2515_EFLG_RX0OVR) {
                    // TODO...
                }
                if(mcp2515_eflg_buf & MCP2515_EFLG_TXBO) {
                    // TODO...
                }
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_EFLG, 0x00u);
            }
            // </editor-fold>
            
            // <editor-fold defaultstate="collapsed" desc="TX Successful!">
            if(mcp2515_intf_buf & MCP2515_TX2I) {
                // TODO... Not sure if there's anything more needed here...
                tx_counter++;
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_TX2I, 0x00u);
            }
            if(mcp2515_intf_buf & MCP2515_TX1I) {
                // TODO... Not sure if there's anything more needed here...
                tx_counter++;
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_TX1I, 0x00u);
            }
            if(mcp2515_intf_buf & MCP2515_TX0I) {
                // TODO... Not sure if there's anything more needed here...
                tx_counter++;
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_TX0I, 0x00u);
            }
            // </editor-fold>
            
            // <editor-fold defaultstate="collapsed" desc="Received Message!">
            if(mcp2515_intf_buf & MCP2515_RX1I) {
                rxbf1_full = 0x01u;  // Indicate this receive buffer has a message
                can_receive(&node1_rxbf0_msg, &node1_rxbf1_msg);    // Read the message
                rxbf1_received = 0x01u;     // Indicate message received event has occured
                msg_received = 0x01u;
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_RX1I, 0x00u);
            }
            if(mcp2515_intf_buf & MCP2515_RX0I) {
                rxbf0_full = 0x01;  // Indicate this receive buffer has a message
                can_receive(&node1_rxbf0_msg, &node1_rxbf1_msg);    // Read the message
                rxbf0_received = 0x01u;
                msg_received = 0x01u;
                
                // Clear flag to reset interrupt condition
                mcp2515_cmd_write_bit(MCP2515_CANINTF, MCP2515_RX0I, 0x00u);
            }

            // </editor-fold>
            
            // Clear event flag
            mcp2515_interrupt_event = 0x00u;
        }
        // </editor-fold>
        
        // <editor-fold defaultstate="collapsed" desc="MESSAGE RECEIVED">
        if(msg_received) {
            /* Node 1 is only expecting to deal with two kinds of received messages:
             * 
             *      1) Acknowledge message from Node 2
             *      2) LED Toggle Command message from Node 2
             * 
             */
            
            if(rxbf0_received) {
                if( (node1_rxbf0_msg.arb_field.sid == NODE2_SID) && (node1_rxbf0_msg.arb_field.eid == NODE2_EID_ACK) ) {
                    ack_counter++;
                    rxbf0_received = 0x00u; // Clear event flag
                } else if( (node1_rxbf0_msg.arb_field.sid == NODE2_SID) && (node1_rxbf0_msg.arb_field.eid == NODE2_EID_LED_CMD) ) {
                    NODE1_TOGGLE_LED;
                    rxbf0_received = 0x00u; // Clear event flag
                }
            }
            
            if(rxbf1_received) {
                if( (node1_rxbf1_msg.arb_field.sid == NODE2_SID) && (node1_rxbf1_msg.arb_field.eid == NODE2_EID_ACK) ) {
                    ack_counter++;
                    rxbf1_received = 0x00u; // Clear event flag
                } else if( (node1_rxbf1_msg.arb_field.sid == NODE2_SID) && (node1_rxbf1_msg.arb_field.eid == NODE2_EID_LED_CMD) ) {
                    NODE1_TOGGLE_LED;
                    rxbf1_received = 0x00u; // Clear event flag
                }
            }
            
            msg_received = 0x00u;   // Clear event flag
            
        }
        // </editor-fold>
        
    }
    
#endif
    
#if CURRENT_NODE == NODE2
    /* ------------------------------------------------------------------------
     * Code for Test RX Node
     * The Test RX Node will update a display indicating the state of the two
     * switches connected to the TX Node. It obtains this state through the
     * SPI message bytes sent by the TX Node every 100ms.
     * 
     * TODO: INCLUDE TX AND RX TIMESTAMPS
     */
    // Initialize SPI mode of MSSP module and LCD display
    LCD_Init_ECE376();
    SPI_Init_Slave_Default();
    
    // Now enable all unmasked interrupts, such as from MSSP
    ENABLE_PERIPHERAL_INTERRUPTS;
    ei();
    
    LCD_set_cursor_position(1,1);   // Row 1, position 1 (NOT 0 indexed)
    char init_success_msg[] = "Init success!";
    for(uint8_t i=0; i<13; i++) LCD_write_data_byte_4bit(init_success_msg[i]);
    
    // Just wait a couple more seconds...
    __delay_ms(2000);
    LCD_clear_display();
    
    // RX Node while(1) loop
    while(1){
        
        if(spi_rx_flag) {   // If a message has been received
            
#ifdef TX_RX_DEBUG
            
            // Print hexadecimal representation of received message
            LCD_set_cursor_position(1,1);
            
            // Top line message -- binary representation of rx msg
            for(int i=0; i<8; i++){
                debug_msg_1[4+i] = (node1_rx_buf & (1u << (7-i))) ? '1' : '0';
            }
            
            // Bottom line message -- hex representation of rx msg
            debug_msg_2[6] = hex_to_char((node1_rx_buf & 0xF0) >> 4u);
            debug_msg_2[7] = hex_to_char(node1_rx_buf & 0x0F);
            
            // Now put messages onto display!
            LCD_set_cursor_position(1,1);
            for(uint8_t i=0; i<12; i++){
                LCD_write_data_byte_4bit(debug_msg_1[i]);
            }
            LCD_set_cursor_position(2,1);
            for(uint8_t i=0; i<8; i++){
                LCD_write_data_byte_4bit(debug_msg_2[i]);
            }
            
            
            spi_rx_flag = 0x00;
            
            
#else
            
            if(spi_rx_invalid_flag) {   // An invalid message was received
                
                LCD_clear_display();
                
                // Print out invalid message
                LCD_set_cursor_position(1,1);
                for(uint8_t i=0; i<LEN_OF_INV_MSG; i++){
                    LCD_write_data_byte_4bit(spi_rx_invalid_msg[i]);
                }
                
                spi_rx_flag = 0x00; // Reset rx flag
                spi_rx_invalid_flag = 0x00u;
                
            } else {    // A valid message was received
                
                // Update button states
                button1_state = (node1_rx_buf & (1u << NODE1_BYTE_BUTTON1_BIT_LOC)) ? '1' : '0';
                button2_state = (node1_rx_buf & (1u << NODE1_BYTE_BUTTON2_BIT_LOC)) ? '1' : '0';
                button1_msg[BUTTON_STATE_CHAR] = button1_state;
                button2_msg[BUTTON_STATE_CHAR] = button2_state;

                // Update display
                // Line 1
                LCD_set_cursor_position(1,1);   // Row 1, position 1 (NOT 0 indexed)
                for(uint8_t i=0; i<NUM_OF_CHAR_USED_PER_LINE; i++){
                    LCD_write_data_byte_4bit(button1_msg[i]);
                }
                // Line 2
                LCD_set_cursor_position(2,1);
                for(uint8_t i=0; i<NUM_OF_CHAR_USED_PER_LINE; i++){
                    LCD_write_data_byte_4bit(button2_msg[i]);
                }

                spi_rx_flag = 0x00; // Reset rx flag
                
            }
            
#endif
        }
        
    }
    
#endif
           
    return;
}


/* Function: hex_to_char
 * ------------------------------------------------------------------------
 * This is to help with printing out messages in their raw hex form. I use it
 * in the debug sections of code. You give it a 4-bit hex number (if the number
 * is greater than 0xF, then you will receive back a ' ' character) and it
 * returns the ASCII character corresponding to this number.
 * 
 * Parameters:
 *      - uint8_t hex_num --> This should be a 4-bit number so that is fits
 *                            into one hex numeral.
 * 
 * Returns:
 *      - char --> This char is the ASCII value of the hex numeral parameter.
 *
 */
static char hex_to_char(uint8_t hex_num){
    switch(hex_num){
        
        case 0x0:
            return '0';
            
        case 0x1:
            return '1';
            
        case 0x2:
            return '2';
            
        case 0x3:
            return '3';
            
        case 0x4:
            return '4';
            
        case 0x5:
            return '5';
            
        case 0x6:
            return '6';
            
        case 0x7:
            return '7';
            
        case 0x8:
            return '8';
            
        case 0x9:
            return '9';
            
        case 0xA:
            return 'A';
            
        case 0xB:
            return 'B';
            
        case 0xC:
            return 'C';
            
        case 0xD:
            return 'D';
            
        case 0xE:
            return 'E';
            
        case 0xF:
            return 'F';
            
        default:
            return ' ';
        
    }
}
