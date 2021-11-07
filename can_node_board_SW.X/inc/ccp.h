/* 
 * File: ccp.h
 * Author: Abdullah Almosalami
 * Comments: This, along with the accompanying .c file, is meant to
 *           configure the CCP module of the PIC18F4620 to work with the
 *           Timer1 peripheral so that I can generate periodic interrupts.
 * Revision history: 0.1
 */

/* ----------------------------------------------------------------------------
 * Brief Description of the CCP (Capture, Compare, PWM) Peripheral
 * ----------------------------------------------------------------------------
 * The PIC18F4620 has two CCP modules, and each module has a 16-bit Capture
 * register, a 16-bit Compare register, and a PWM Master/Slave Duty Cycle
 * register. The CCP modules are configured through their respective CCPxCON
 * registers. For the purpose of this project, the CCP module is used to reset
 * Timer1 whenever Timer1 reaches the value placed in the Compare register. On
 * this reset, an interrupt flag is set, and if enabled, an interrupt is
 * generated. This is called a Special Event Trigger. The Compare register is
 * called CCPR, so a write to CCPR sets this compare value. Also, the CCP module
 * can be configured to do additional things when this event occurs, like
 * toggle a pin (the corresponding CCPx pin) and/or start an A/D conversion.
 */


#ifndef CCP_HEADER
#define	CCP_HEADER

#include <xc.h>
#include <stdint.h>


// Definitions
// COMMENT: For now, I am just using the CCP2 module.
// CCP2CON - CCP2 Control Register
#define CCPxM           0x0F    // CCPx Module Mode Select bits;
#define CCP1M_DEFAULT   (CCP1CON = 0x0B)  // 1011 for Compare mode /w Special
                                          // Trigger Event
#define CCP2M_DEFAULT   (CCP2CON = 0x0B)

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


// Function declarations

// CCP2
void CCP1_Init_Default(uint16_t comp_val);
void CCP2_Init_Default(uint16_t comp_val);


#endif	/* CCP_HEADER */

