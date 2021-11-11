/* 
 * File: ccp.h
 * Author: Abdullah Almosalami
 * Comments: This, along with the accompanying .c file, is meant to be my
 *           implementation of driving the CCP peripheral to either operate
 *           in Compare, Capture, or PWM modes with Timer1 or Timer3. I've
 *           still got whatever's on my TODO to implement, and I am mostly
 *           implementing the functionality as I go along with my projects/
 *           assignments. You'll find that based on the project, the
 *           CCPx_Init_Default function changes.
 * 
 * TODO:
 *      1- Make the initialization more generic to configure CCPCONx and T3CON
 *         based on passed arguments
 *      2- PWM Mode
 *      3- Functions for reconfiguration/mode changing
 * 
 * Revision history: 0.1
 * Started: Some time at the end of OCT 2021
 */

/* ----------------------------------------------------------------------------
 * Brief Description of the CCP (Capture, Compare, PWM) Peripheral
 * ----------------------------------------------------------------------------
 * The PIC18F4620 has two CCP modules, and each module has a 16-bit Capture
 * register, a 16-bit Compare register, and a PWM Master/Slave Duty Cycle
 * register. The CCP modules are configured through their respective CCPxCON
 * registers.
 *
 * COMPARE MODE
 * For the purpose of this project, the CCP module is used to reset
 * Timer1 whenever Timer1 reaches the value placed in the Compare register. On
 * this reset, an interrupt flag is set, and if enabled, an interrupt is
 * generated. This is called a Special Event Trigger. The Compare register is
 * called CCPR, so a write to CCPR sets this compare value. Also, the CCP module
 * can be configured to do additional things when this event occurs, like
 * toggle a pin (the corresponding CCPx pin) and/or start an A/D conversion.
 * 
 * CAPTURE MODE
 * In Capture mode, the CCPR register-pair captures the 16-bit value of TMR1 or
 * TMR3 when an event occurs on the corresponding CCPx pin, which may include
 * every rising edge, every falling edge, every 4th rising edge, and every 16th
 * rising edge. This is selected by the mode select bits CCPxM. When a capture
 * is made, the CCPxIF is set and the usual peripheral interrupt stuff is noted...
 * Aside from setting the CCPxM bits for configuring mode, the corresponding CCPx
 * pin should be set as an input (RC2 for CCP1, RC1 or RB3 for CCP2). Just like
 * in compare mode, the timer associated with the CCPx module is set in T3CON.
 * 
 * NOTE: When changing capture modes, make sure to disable the CCPx interrupt as
 *       mode changes could trigger a false interrupt.
 */


#ifndef CCP_HEADER
#define	CCP_HEADER

#include <xc.h>
#include <stdint.h>


// <editor-fold defaultstate="collapsed" desc="DEFINITIONS: REGISTERS & MACROS">

// REGISTER OUTLINE
/* CCPxCON - CCPx Control Register -  Register 15-1 in Datasheet
 * Default/POR: 00 00 0000
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |..........undef..........|.........DCxB..........|.....................CCPxM.....................|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) - DCxB : PWM Duty Cycle bit 1 and bit 0 for CCPx Module
 *              Lower LSbs of the 10-bit PWM duty cycle, with the upper eight in CCPRxL
 *      
 *      (R/W) - CCPxM : CCPx Module Mode Select bits
 *              0000 = disabled
 *              0001 = reserved
 *              0010 = Compare mode, toggle output on match (CCPxIF bit is set)
 *              0011 = reserved
 *              0100 = Capture mode, every falling edge
 *              0101 = Capture mode, every rising edge
 *              0110 = Capture mode, every 4th rising edge
 *              0111 = Capture mode, every 16th rising edge
 *              1000 = Compare mode, initialize CCPx pin low, on compare match, set HIGH
 *              1001 = "" but instead HIGH to LOW
 *              1010 = Compare mode, generate software interrupt on compare match
 *              1011 = Compare mode, trigger special event, reset timer, CCPx match starts A/D conversion
 *              11xx = PWM Mode
 * 
 */

#define CCPxM           0x0F    // CCPx Module Mode Select bits;
#define CCP1M_COMPARE_DEFAULT   (CCP1CON = 0xB)  // 1011 for Compare mode /w Special
                                                 // Trigger Event
#define CCP2M_COMPARE_DEFAULT   (CCP2CON = 0xB)  // Likewise, but I believe Special
                                                 // Trigger Event also starts A/D
#define CCP1M_CAPTURE_DEFAULT   (CCP1CON = 0x5)  // Capture mode, every rising edge of the CCP1 pin
#define CCP2M_CAPTURE_DEFAULT   (CCP2CON = 0x5)  // Capture mode, every rising edge of the CCP2 pin

// T3CON since it sets which Timer operates with which CCP module
/* T3CON - Timer3 Control Register -  Register 14-1 in Datasheet
 * Default/POR: 0 0 00 0 0 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....RD16....|....T3CCP2..|........T3CKPS.........|...T3CCP1..|..~T3SYNC..|..TMR3CS...|...TMR3ON..|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) RD16 : 16-Bit R/W Mode enable
 *      (R/W) T3CCP2:T3CCP1 : Timer3 and Timer1 to CCPx Enable --> CAREFUL! THE BITS ARE NOT NEXT TO EACH OTHER!
 *                      1x = Timer3 is the capture/compare source for both CCP modules
 *                      01 = Timer3 --> CCP2, Timer1 --> CCP1
 *                      00 = Timer1 is the capture/compare source for both CCP modules
 *      (R/W) T3CKPS : Timer3 Input Clock prescale select bits
 *                      11 = 1:8
 *                      10 = 1:4
 *                      01 = 1:2
 *                      00 = 1:1
 *      (R/W) ~T3SYNC : Timer3 External Clock Input Synchronization Control bit (ignore if not using Timer as fosc)
 *      (R/W) TMR3CS : Timer3 Clock Source select bit
 *                      1 = Either TMR1 or T13CKI external clock input
 *                      0 = FOSC/4
 *      (R/W) TMR3ON : Timer3 On bit
 * 
 */
#define T3CON_T3CCP_BITS                0x48    // Timer3 and Timer1 CCPx Enable bits --> BITS NOT NEXT TO EACH OTHER!
#define TMR1_FOR_CCPx                   (T3CON &= ~T3CON_T3CCP_BITS)
#define TMR3_FOR_CCPx                   (T3CON |= T3CON_T3CCP_BITS)
#define TMR1_FOR_CCP1_TMR3_FOR_CCP2     (T3CON &= ~(1u << 3); (T3CON |= (1u << 6))

// Interrupt-Related
#define CCP1_IF_BIT                 PIR1bits.CCP1IF
#define CCP2_IF_BIT                 PIR2bits.CCP2IF
#define CCP1_INT_ENABLE_BIT         PIE1bits.CCP1IE
#define CCP2_INT_ENABLE_BIT         PIE2bits.CCP2IE
#define ENABLE_CCP1_INTERRUPT       (CCP1_INT_ENABLE_BIT = 1u)
#define ENABLE_CCP2_INTERRUPT       (CCP2_INT_ENABLE_BIT = 1u)
#define CLEAR_CCP1_IF               (CCP1_IF_BIT = 0u)
#define CLEAR_CCP2_IF               (CCP2_IF_BIT = 0u)

#ifndef ENABLE_PERIPHERAL_INTERRUPTS
#define ENABLE_PERIPHERAL_INTERRUPTS    (INTCONbits.PEIE = 1u)
#define DISABLE_PERIPHERAL_INTERRUPTS   (INTCONbits.PEIE = 0u)
#endif

// Macros
#define CCP1_SET_COMP_VAL(x)    (CCPR1 = x)
#define CCP2_SET_COMP_VAL(x)    (CCPR2 = x)

// </editor-fold>


// Enumerations
typedef enum { TMR1_CCP1, TMR1_CCPx, TM3_CCP2, TM3_CCPx} tmr_ccp_pair_t;


// Function declarations

// CCP2
void CCP1_Compare_Init_Default(uint16_t comp_val);
void CCP2_Compare_Init_Default(uint16_t comp_val);
void CCP1_Capture_Init_Default(void);
void CCP2_Capture_Init_Default(void);

#endif	/* CCP_HEADER */

