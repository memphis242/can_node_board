/* 
 * File: spi_two_node_test.h
 * Author: Abdullah Almosalami
 * Comments: This is meant to hold some defines for the testing
 * Revision history: 0.1
 */

 
#ifndef SPI_TWO_NODE_TEST_HEADER
#define	SPI_TWO_NODE_TEST_HEADER

#include <xc.h>


// Definitions
// Test inputs
#define BUTTON_PORT                         PORTE
#define BUTTON1_PIN                         PORTEbits.RE0
#define BUTTON2_PIN                         PORTEbits.RE1
#define BUTTON1_TRIS                        TRISEbits.RE0 = 1u
#define BUTTON2_TRIS                        TRISEbits.RE1 = 1u
#define BUTTON_ADCON                        (ADCON1bits.PCFG = 0xA)

// SPI TX Node message format
#define SPI_TX_BYTE_BUTTON1_BIT_LOC         0u
#define SPI_TX_BYTE_BUTTON2_BIT_LOC         1u
#define SPI_ID_BITS                         0xC0u   // bits 7 and 6 will be used as IDs for now
#define SPI_TX_NODE_ID                      0x00u
#define SPI_RX_NODE_ID                      0x40u

// SPI specific messages
#define SPI_RX_ACK_MSG                      0x3Fu    // This will be used as an "acknowledge" msg from a receiver
#define SPI_RX_NODE_ACK_MSG                 (SPI_RX_NODE_ID | SPI_RX_ACK_MSG)
#define SPI_RX_FAIL_MSG                     0x2Au    // This will be used as a "failed to receive" msg from a receiver
#define SPI_RX_NODE_FAIL_MSG                (SPI_RX_NODE_ID | SPI_RX_FAIL_MSG)

#endif	/* SPI_TWO_NODE_TEST_HEADER */

