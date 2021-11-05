///*
// * File:   main.c
// * Author: Abdullah Almosalami
// *
// * Created on October 21, 2021, 11:30 PM
// */
//
//
//// <editor-fold defaultstate="collapsed" desc="PIC18F4620 Configuration Bit Settings">
//// CONFIG1H
//#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
//#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
//
//// CONFIG2L
//#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
//#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
//#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)
//
//// CONFIG2H
//#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
//#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
//
//// CONFIG3H
//#pragma config CCP2MX = PORTBE  // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
//#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
//#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
//#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
//
//// CONFIG4L
//#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
//#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
//#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
//
//// CONFIG5L
//#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
//#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
//#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
//#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)
//
//// CONFIG5H
//#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
//#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)
//
//// CONFIG6L
//#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
//#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
//#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
//#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)
//
//// CONFIG6H
//#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
//#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
//#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
//
//// CONFIG7L
//#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
//#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
//#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
//#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
//
//// CONFIG7H
//#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
//// </editor-fold>
//
//
//#include <xc.h>
//#include <stdint.h>
//#include "inc/mssp_spi_driver.h"
//#include "inc/lcd_driver.h"
//#include "inc/timer.h"
//#include "inc/ccp.h"
//#include "spi_two_node_test.h"
//
//
//#ifndef _XTAL_FREQ
//#define _XTAL_FREQ                  40000000U   // 40MHz --> Defining this will allow use of __delay_ms())
//#endif
//
//// Extra defines
//#define ENABLE_PERIPHERAL_INTERRUPTS    (INTCONbits.PEIE = 1u)
//#define DISABLE_PERIPHERAL_INTERRUPTS   (INTCONbits.PEIE = 0u)
//
///******************************************************************************
// * Interrupt Routine
// * 
// * TODO: 
// */
//void __interrupt() isr(void){
//    
////    LATDbits.LATD3 = 1u;
//    
//    /* ****************************************************
//     * MSSP SPI INTERRUPT
//     * ****************************************************
//     */ 
//    if(MSSP_IF_BIT && MSSP_INT_ENABLE_BIT) {
//        // Successful byte TXd and RXd
//        transfer_complete_flag = 0x01;  // Set the transf flag
//        
//#if CURRENT_NODE == RX_NODE
//        spi_rx_msg_buf = SSPBUF;
//        spi_rx_flag = 0x01;
//        
//        // Check validity of message
//        if((spi_rx_msg_buf & SPI_ID_BITS) != SPI_TX_NODE_ID){   // Wrong ID
//            SSPBUF = SPI_RX_NODE_FAIL_MSG;
//            spi_rx_invalid_flag = 0x01; // Set flag
//        } else {
//            SSPBUF = SPI_RX_ACK_MSG;
//        }
//        
//#endif
//        
//        CLEAR_MSSP_IFLAG;
//    }
//    
//    /* ****************************************************
//     * CCP2 INTERRUPT
//     * ****************************************************
//     */
//#if CURRENT_NODE == TX_NODE
//    if(CCP2_IF_BIT && CCP2_INT_ENABLE_BIT){
//        
//        // On every other compare match, transmit!
//        if(tmr_100ms_next){
//            // Reset flag
//            tmr_100ms_next = 0x00;
//            
//            // Set flag to indicate ready to transmit
//            spi_ready_to_tx = 0x01;
//            
//            CLEAR_CCP2_IF;
//            
//        } else{
//            // Set flag so next interrupt, we transmit
//            tmr_100ms_next = 0x01;
//            spi_ready_to_tx = 0x00; // Just make sure this flag is still cleared
//            
//            CLEAR_CCP2_IF;
//        }
//    }
//#endif
//    
////    LATDbits.LATD3 = 0u;
//    
//    return;
//}
//
//
///******************************************************************************
// * Main Routine
// */
//void main(void) {
//
//    return;
//}
//
