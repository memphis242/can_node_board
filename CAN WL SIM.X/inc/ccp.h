/* 
 * File: ccp.h
 * Author: Abdullah Almosalami
 * Comments: This, along with the accompanying .c file, is meant to
 *           configure the CCP module of the PIC18F4620.
 * Revision history: 0.1
 */

#ifndef CCP_HEADER
#define	CCP_HEADER

#include <xc.h>
#include <stdint.h>


// Definitions
//CCP2CON - CCP2 Control Register
#define CCP2M       0x0F    // CCPx Module Mode Select bits; 1010 for Compare mode w/o affecting CCP2 pin, 1011 for A/D

#define CCP2M_DEFAULT   CCP2CON = 0x05

// Interrupt-Related
#define CCP2_IF_BIT                 PIR2bits.CCP2IF
#define CCP2_INT_ENABLE_BIT         PIE2bits.CCP2IE
#define ENABLE_CCP2_INTERRUPT       CCP2_INT_ENABLE_BIT = 1u
#define CLEAR_CCP2_IF               PIR2bits.CCP2IF = 0u;


// Function declarations

// CCP2
void CCP2_Init_Default(void);
void CCP2_Compare_Val(uint16_t comp_val);



#endif	/* CCP_HEADER */

