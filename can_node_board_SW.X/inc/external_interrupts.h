/* 
 * File: external_interrupts.h
 * Author: Abdullah Almosalami
 * Comments: This is meant to be used to set up the INT0, INT1, and INT2 external
 *           interrupt pins.
 * Revision history: 0.1
 * Date: 11:13 AM on November 11,2021
 * 
 */

/* OVERVIEW
 * The PIC18F4620 has external interrupt pins on RB0, RB1, and RB2 --> INT0, INT1,
 * and INT2. These are edge-triggered interrupts, configured through the INTEDGx
 * bit in INTCON2. When the right edge appears on these pins, the corresponding
 * interrupt flag INTxIF is set, and an interrupt can be generated. These pins
 * can be used to wake up the processor from Idle or Sleep mods if they are
 * enabled/unmasked (INTxIE). If GIE is also enabled, then the processor will
 * branch to the corresponding interrupt vector location.
 */


#ifndef EXTERNAL_INTERRUPTS_HEADER
#define	EXTERNAL_INTERRUPTS_HEADER

#include <xc.h>
#include <stdint.h>


// DEFINITIONS
/* INTCON1 - Interrupt Control Register 1 -  Register 10-1 in Datasheet
 * Default/POR: 0 0 0 0 0 0 0 x
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |..GIE/GIEH..|..PEIE/GIEL.|...TMR0IE..|...INT0IE..|....RBIE...|...TMR0IF..|...INT0IF..|...RBIF....|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) GIE/GIEH - When IPEN = 0, enables all unmasked interrupts.
 *                       When IPEN = 1, enables all high-priority interrupts
 *      (R/W) PEIE/GIEL - When IPEN = 0, enables all unmasked peripheral interrupts
 *                        When IPEN = 1, enables all low-priority peripheral interrupts
 *      (R/W) TMR0IE
 *      (R/W) INT0IE
 *      (R/W) RBIE
 *      (R/W) TMR0IF
 *      (R/W) INT0IF
 *      (R/W) RBIF
 * 
 */
/* INTCON2 - Interrupt Control Register 2 -  Register 10-2 in Datasheet
 * Default/POR: 1 111 0 1 0 1
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...~RBPU....|................INTEDG..............|...undef...|...TMR0IP..|...undef...|...RBIP....|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) RBPU - PORTB Pullup Enable    1 = disable, 0 = enable
 *      (R/W) INTEDG - External Interrupt x Edge Select bit --> 1 = rising, 0 = falling
 *                     INTEDG0:INTEDG1:INTEDG2
 *      (R/W) TMR0IP - TMR0 Overflow Interrupt Priority bit --> 1 = high, 0 = low
 *      (R/W) RBIP - RB Port Change Interrupt Priority bit  --> ""
 * 
 */
/* INTCON3 - Interrupt Control Register 3 -  Register 10-3 in Datasheet
 * Default/POR: 11 0 00 0 00
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...INT2IP...|...INT1IP...|...undef...|...INT2IE..|...INT1IE..|...undef...|...INT2IF..|...INT1IF..|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) INTxIP - INTx External Interrupt Priority bit --> 1 = high, 0 = low
 *      (R/W) INTxIE - INTx External Interrupt Enable bit
 *      (R/W) INTxIF - INTx External Interrupt Flag bits
 * 
 */

#define INT0_RISING_EDGE        (INTCON2bits.INTEDG0 = 1u)
#define INT1_RISING_EDGE        (INTCON2bits.INTEDG1 = 1u)
#define INT2_RISING_EDGE        (INTCON2bits.INTEDG2 = 1u)
#define INT0_FALLING_EDGE       (INTCON2bits.INTEDG0 = 0u)
#define INT1_FALLING_EDGE       (INTCON2bits.INTEDG1 = 0u)
#define INT2_FALLING_EDGE       (INTCON2bits.INTEDG2 = 0u)
#define INTx_RISING_EDGE        (INTCON2 |= (0x70))     // All on rising edge
#define INTx_FALLING_EDGE       (INTCON2 &= ~(0x70))    // All on falling edge

#define INT0_ENABLE     (INTCONbits.INT0IE = 1u)
#define INT0_ENABLE_BIT (INTCONbits.INT0IE)
#define INT0_DISABLE    (INTCONbits.INT0IE = 0u)
#define INT1_ENABLE     (INTCON3bits.INT1IE = 1u)
#define INT1_ENABLE_BIT (INTCON3bits.INT1IE)
#define INT1_DISABLE    (INTCON3bits.INT1IE = 0u)
#define INT2_ENABLE     (INTCON3bits.INT2IE = 1u)
#define INT2_ENABLE_BIT (INTCON3bits.INT2IE)
#define INT2_DISABLE    (INTCON3bits.INT2IE = 0u)

#define INT0_FLAG           INTCONbits.INT0IF
#define INT1_FLAG           INTCON3bits.INT1IF
#define INT2_FLAG           INTCON3bits.INT2IF
#define CLEAR_INT0_FLAG     (INT0_FLAG = 0u)
#define CLEAR_INT1_FLAG     (INT1_FLAG = 0u)
#define CLEAR_INT2_FLAG     (INT2_FLAG = 0u)

#define TRIS_INTx       ADCON1bits.PCFG = 0xF; (TRISB |= 0x07)
#define TRIS_INT0       ADCON1bits.PCFG = 0xF; (TRISB |= 0x01)
#define TRIS_INT1       ADCON1bits.PCFG = 0xF; (TRISB |= 0x02)
#define TRIS_INT2       ADCON1bits.PCFG = 0xF; (TRISB |= 0x04)


#ifndef TEST_BIT_SET

// Some bit-related macros
#define TEST_BIT_SET(byte,bit)          (byte & (1u << bit))
#define TEST_BIT_CLEAR(byte,bit)        (~byte & (1u << bit))
#define CLEAR_BIT(byte,bit)             (byte &= ~(1u << bit))
#define SET_BIT(byte,bit)               (byte |= (1u << bit))

#endif


// ENUMS
typedef enum { EXT_INT_INT0 = 1u, EXT_INT_INT1 = 2u, EXT_INT_INT2 = 4u } external_interrupts_t;
typedef enum { FALLING_EDGE, RISING_EDGE } external_interrupt_edge_t;


// FUNCTION-PROTOTYPES
void external_interrupts_init_default(void);
void external_interrupts_init(external_interrupts_t which_pin, external_interrupt_edge_t trigger_edge);

#endif	/* EXTERNAL_INTERRUPTS_HEADER */

