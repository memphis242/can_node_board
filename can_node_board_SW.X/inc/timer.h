/* 
 * File: timer.h
 * Author: Abdullah Almosalami
 * Comments: This header file, and its accompanying .c file, are meant to
 *           configure the Timerx modules on the PIC18F4620
 * Revision history: 0.1
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_HEADER
#define	TIMER_HEADER

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#ifndef ENABLE_PERIPHERAL_INTERRUPTS
#define ENABLE_PERIPHERAL_INTERRUPTS    (INTCONbits.PEIE = 1u)
#define DISABLE_PERIPHERAL_INTERRUPTS   (INTCONbits.PEIE = 0u)
#endif


// Definitions
// TIMER1 MODULE
// T1CON - Timer1 Control Register
#define T1CON_RD16_BIT      0x80    // 16-Bit Read/Write Mode Enable bit; 0 = enables R/W in two 8-bit ops, 1 = one 16-bit op
#define T1CON_T1RUN_BIT     0x40    // Timer1 System Clock Status bit; 0 = clock derived from another source, 1 = Timer1 oscillator
#define T1CON_T1CKPS_BITS   0x30    // Timer1 Input Clock Prescale Select bits; 11 = 1:8, 10 = 1:4, 01 = 1:2, 00 = 1:1
#define T1CON_T1OSCEN_BIT   0x08    // Timer1 Oscillator Enable bit
#define T1CON_T1SYNC_BIT    0x04    // Timer1 External Clock Input Synchronization bit; see datasheet
#define T1CON_TMR1CS_BIT    0x02    // Timer1 Clock Source Select bit; 1 = external clock, 0 = internal clock (FOSC/4)
#define T1CON_TMR1ON_BIT    0x01    // Timer1 On bit

#define TMR1_16BIT_RW       T1CON |= T1CON_RD16_BIT
#define TMR1_PRESCAL_8      T1CON |= 0x30
#define TMR1_PRESCAL_4      do{ T1CON |= 0x20; T1CON &= ~0x10; } while(0);
#define TMR1_PRESCAL_2      do{ T1CON &= ~0x20; T1CON |= 0x10; } while(0);
#define TMR1_PRESCAL_1      T1CON &= ~0x30
#define TMR1_OSC_OFF        T1CON &= ~0x08
#define TMR1_OSC_ON         T1CON |= 0x08
#define TMR1_INT_OSC        T1CON &= ~0x02
#define TMR1_ON             T1CON |= T1CON_TMR1ON_BIT
#define TMR1_OFF            T1CON &= ~T1CON_TMR1ON_BIT
#define TMR1_DEFAULT        T1CON = 0x80        // 16-bit, prescaler of 1, tmr1 osc off, internal clock, tmr1 off


// TIMER3 MODULE
// T3CON - Timer3 Control Register
#define T3CON_RD16_BIT      0x80
// TODO
#define T3CON_T3CCP_BITS    0x48    // Timer3 and Timer1 CCPx Enable bits --> BITS NOT NEXT TO EACH OTHER!
#define TMR3_DEFAULT        T3CON = 0x88u   // 16-bit, prescaler of 1, CCP1 with Timer1, CCP2 with Timer3, internal clock, tmr3 off
#define TMR3_PRESCALE_8     T3CON |= 0x30u
#define T3CON_TMR3ON_BIT    0x01u
#define TMR3_ON             T3CON |= T3CON_TMR3ON_BIT
#define TMR3_OFF            T3CON &= ~T3CON_TMR3ON_BIT


// Interrupt-related
#define TMR1_ENABLE_INTERRUPT   (PIE1bits.TMR1IE = 1u)
#define TMR1_ENABLE_BIT         (PIE1bits.TMR1IE)
#define TMR3_ENABLE_INTERRUPT   (PIE2bits.TMR3IE = 1u)
#define TMR3_ENABLE_BIT         (PIE2bits.TMR3IE)
#define TMR1_IF                 PIR1bits.TMR1IF
#define CLEAR_TMR1_IF           (TMR1_IF = 0u)
#define TMR3_IF                 (PIR2bits.TMR3IF)
#define CLEAR_TMR3_IF           (PIR2bits.TMR3IF = 0u)


// Some CCPRx values for 100ns instruction cycle, /8 prescalar
#define DEFAULT_CONFIG_PERIOD_50ms      62500u
#define DEFAULT_CONFIG_PERIOD_10ms      12500u


// Function declaration

// Timer1
void Timer1_Init_Default(void);
void Timer3_Init_Default(void);

void Timer1_Enable(void);
void Timer1_Disable(void);
void Timer3_Enable(void);
void Timer3_Disable(void);




#endif	/* TIMER_HEADER */

