/*
 * File:   main.c
 * Author: Abdullah Almosalami
 *
 * Created on October 21, 2021, 11:30 PM
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
#include "inc/mssp_spi.h"
#include "inc/lcd_driver.h"
#include "inc/timer.h"
#include "inc/ccp.h"
#include "spi_test.h"


#ifndef _XTAL_FREQ
#define _XTAL_FREQ                  40000000U   // 40MHz --> Defining this will allow use of __delay_ms())
#endif


// I will use the following to make it more convenient to write both the code
// the Test TX Node and the Test RX Node in the same file
#define TX_NODE         0u
#define RX_NODE         1u
//#define CURRENT_NODE    TX_NODE
#define CURRENT_NODE    RX_NODE

//#define TX_RX_DEBUG


// Extra defines
#define ENABLE_PERIPHERAL_INTERRUPTS    (INTCONbits.PEIE = 1u)
#define DISABLE_PERIPHERAL_INTERRUPTS   (INTCONbits.PEIE = 0u)
// External variables
extern uint8_t receive_byte;    // Used by ISR when SPI mode is slave and receive has occurred
extern uint8_t slave_mode;  // Flag to indicate current mode --> 0 = master, 1 = slave
extern uint8_t transfer_complete_flag;
extern uint8_t manual_transfer; // Flag to indicate when we are manually (i.e., using the functions below) transferring data; may be used in ISR or debugging

#if CURRENT_NODE == TX_NODE
static volatile uint8_t tmr_100ms_next = 0x00;  // This is used to indicate when 100ms has passed
static uint8_t spi_tx_test_msg = 0x00;  // This holds the test byte to be sent over SPI
static uint8_t spi_rx_msg_buf = 0x00u;
static volatile uint8_t spi_ready_to_tx = 0x00;
#endif

#if CURRENT_NODE == RX_NODE
static uint8_t spi_rx_msg_buf = 0x00;
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
    
//    LATDbits.LATD3 = 1u;
    
    /* ****************************************************
     * MSSP SPI INTERRUPT
     * ****************************************************
     */ 
    if(MSSP_IF_BIT && MSSP_INT_ENABLE_BIT) {
        // Successful byte TXd and RXd
        transfer_complete_flag = 0x01;  // Set the transf flag
        
#if CURRENT_NODE == RX_NODE
        spi_rx_msg_buf = SSPBUF;
        spi_rx_flag = 0x01;
        
        // Check validity of message
        if((spi_rx_msg_buf & SPI_ID_BITS) != SPI_TX_NODE_ID){   // Wrong ID
            SSPBUF = SPI_RX_NODE_FAIL_MSG;
            spi_rx_invalid_flag = 0x01; // Set flag
        } else {
            SSPBUF = SPI_RX_ACK_MSG;
        }
        
#endif
        
        CLEAR_MSSP_IFLAG;
    }
    
    /* ****************************************************
     * CCP1 INTERRUPT
     * ****************************************************
     */
#if CURRENT_NODE == TX_NODE
    if(CCP1_IF_BIT && CCP1_INT_ENABLE_BIT){
        
        // On every other compare match, transmit!
        if(tmr_100ms_next){
            // Reset flag
            tmr_100ms_next = 0x00;
            
            // Set flag to indicate ready to transmit
            spi_ready_to_tx = 0x01;
            
            CLEAR_CCP1_IF;
            
        } else{
            // Set flag so next interrupt, we transmit
            tmr_100ms_next = 0x01;
            spi_ready_to_tx = 0x00; // Just make sure this flag is still cleared
            
            CLEAR_CCP1_IF;
        }
    }
#endif
    
//    LATDbits.LATD3 = 0u;
    
    return;
}


/******************************************************************************
 * Main Routine
 */
void main(void) {
    
#if CURRENT_NODE == TX_NODE
    /**************************************************************************
     * Code for Test TX Node
     * The Test TX Node will send a byte every 100ms indicating the state of 
     * two switches it is connected to. The Test RX Node should then update
     * its display indicating these states.
     * 
     * TODO: INCLUDE TX TIMESTAMPS
     */
    
    SPI_Init_Master_Default();  // REMEMBER RD2 IS BEING USED AS CS
    // Use RE0 and RE1 as button inputs
    BUTTON_PORT = 0x00;
    BUTTON1_TRIS;
    BUTTON2_TRIS;
    BUTTON_ADCON;
    
//    // Use RA0 as analog input (AN0); all other analog channel pins set to digital
//    ADCON1bits.PCFG = 0xE;
    
    // Configure TIMER1 and CCP2 to trigger a transmission every 100ms
    Timer1_Init_Default(DEFAULT_CONFIG_PERIOD_50ms);
    
    // Once done with all other initializations, turn on Timer1 and enable all
    // unmasked interrupts like from the MSSP module
    Timer1_Enable();
    ENABLE_PERIPHERAL_INTERRUPTS;
    ei();
    
    // TX Node while(1) loop
    while(1){

        if(spi_ready_to_tx){
            
            spi_tx_test_msg = 0x00;
            
            // Set up I/O signals in byte
            // Buttons 1 and 2
            spi_tx_test_msg |= (BUTTON1_PIN << SPI_TX_BYTE_BUTTON1_BIT_LOC);
            spi_tx_test_msg |= (BUTTON2_PIN << SPI_TX_BYTE_BUTTON2_BIT_LOC);
            
            // Place node ID in byte
            spi_tx_test_msg |= SPI_TX_NODE_ID;
            
            // Send byte away!
            SPI_Send_Byte(spi_tx_test_msg);
            
            
            // TODO: NEED TO SORT OUT ACKNOWLEDGE BEHAVIOR...
//            // Check for acknowledge byte
//            SPI_Receive_Byte(&spi_rx_msg_buf);
//            // If fail to acknowledge...
//            if(spi_rx_msg_buf != SPI_RX_NODE_ACK_MSG) {
//                
//#ifdef TX_RX_DEBUG
//                SPI_Send_Byte(spi_rx_msg_buf);
//#else
//                
//                // Try to send the message again...
//                SPI_Send_Byte(spi_tx_test_msg);
//                
//                // Check again for acknowledge...
//                SPI_Receive_Byte(&spi_rx_msg_buf);
//                if(spi_rx_msg_buf != SPI_RX_NODE_ACK_MSG) {
//                    // Show on display "ERR: Failed to get ACK!"
//                }
//                
//#endif
//            }
            
            spi_ready_to_tx = 0x00; // Reset ready-to-send flag for next 100ms            
        }
        
    }
    
#endif
    
#if CURRENT_NODE == RX_NODE
    /**************************************************************************
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
                debug_msg_1[4+i] = (spi_rx_msg_buf & (1u << (7-i))) ? '1' : '0';
            }
            
            // Bottom line message -- hex representation of rx msg
            debug_msg_2[6] = hex_to_char((spi_rx_msg_buf & 0xF0) >> 4u);
            debug_msg_2[7] = hex_to_char(spi_rx_msg_buf & 0x0F);
            
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
                button1_state = (spi_rx_msg_buf & (1u << SPI_TX_BYTE_BUTTON1_BIT_LOC)) ? '1' : '0';
                button2_state = (spi_rx_msg_buf & (1u << SPI_TX_BYTE_BUTTON2_BIT_LOC)) ? '1' : '0';
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
 * --------------------------------------
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
