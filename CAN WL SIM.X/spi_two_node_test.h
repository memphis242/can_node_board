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
#define BUTTON1_PIN                         PORTEbits.RE0
#define BUTTON2_PIN                         PORTEbits.RE1
#define BUTTON1_TRIS                        TRISEbits.RE0 = 1u;
#define BUTTON2_TRIS                        TRISEbits.RE1 = 1u;
#define SPI_TX_BYTE_BUTTON1_BIT_LOC         0u
#define SPI_TX_BYTE_BUTTON2_BIT_LOC         1u



#endif	/* SPI_TWO_NODE_TEST_HEADER */

