/*
 * File:   main.c
 * Author: Abdullah Almosalami
 *
 * Created on October 21, 2021, 11:30 PM
 */



// PIC18F4620 Configuration Bit Settings
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


#include <xc.h>
#include <stdint.h>
#include "mssp_spi_driver.h"
#include "lcd_driver.h"
#include "timer.h"
#include "ccp.h"
#include "spi_two_node_test.h"


#ifndef _XTAL_FREQ
#define _XTAL_FREQ                  40000000U   // 40MHz --> Defining this will allow use of __delay_ms())
#endif


extern uint8_t receive_byte;    // Used by ISR when SPI mode is slave and receive has occurred
extern uint8_t slave_mode;  // Flag to indicate current mode --> 0 = master, 1 = slave
extern uint8_t transfer_complete_flag;
extern uint8_t manual_transfer; // Flag to indicate when we are manually (i.e., using the functions below) transferring data; may be used in ISR or debugging

static volatile uint8_t tmr_100ms_next = 0x00;
static uint8_t spi_tx_test_message = 0x00;
static volatile uint8_t spi_ready_to_tx = 0x00;

// Interrupt Routine
void __interrupt() isr(void){
        
    if(MSSP_IF_BIT && MSSP_INT_ENABLE_BIT) {
        // Successful byte TXd and RXd
        transfer_complete_flag = 0x01;  // Set the transf flag
        
        CLEAR_MSSP_IFLAG;
    }
    
    if(CCP2_IF_BIT && CCP2_INT_ENABLE_BIT){
        // On every other compare match, transmit!
        if(tmr_100ms_next){
            // Reset flag
            tmr_100ms_next = 0x00;
            
            // Set flag to indicate ready to transmit
            spi_ready_to_tx = 0x01;
            
            CLEAR_CCP2_IF;
            
        } else{
            // Set flag so next interrupt, we transmit
            tmr_100ms_next = 0x01;
            
            CLEAR_CCP2_IF;
        }
        
        
    }
    
    return;
}

// Main Routine
void main(void) {
    
    // Code for Test TX Node
    SPI_Init_Master_Default();
    // Use RD0 and RD1 as button inputs
    PORTE = 0x00;
    BUTTON1_TRIS;
    BUTTON2_TRIS;
    
    // Use RA0 as analog input (AN0); all other analog channel pins set to digital
    ADCON1bits.PCFG = 0xE;
    
    // Configure TIMER1 and CCP2 to trigger a transmission every 100ms
    Timer1_Init_Default(DEFAULT_CONFIG_PERIOD_50ms);
    
    
    // Once done with all other initializations, turn on Timer1 and enable all unmasked interrupts
    Timer1_Enable();
    ei();
    
    
    // Code for Test RX Node
    
    while(1){
        
        
        
    }
           
    return;
}
