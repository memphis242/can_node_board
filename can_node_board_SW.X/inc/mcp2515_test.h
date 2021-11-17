/* 
 * File: mcp2515_test.h
 * Author: Abdullah Almosalami
 * Comments: This is meant to hold some defines for the testing
 * Revision history: 0.1
 */

 
#ifndef MCP2515_TEST_HEADER
#define	MCP2515_TEST_HEADER

#include <xc.h>


// Definitions
// Test inputs
#define NODE1_BUTTON_PORT                         PORTE
#define NODE1_BUTTON1_PIN                         PORTEbits.RE0
#define NODE1_BUTTON2_PIN                         PORTEbits.RE1
#define NODE1_BUTTON1_TRIS                        TRISEbits.RE0 = 1u
#define NODE1_BUTTON2_TRIS                        TRISEbits.RE1 = 1u
#define NODE1_ENG_SPD_POT                         PORTAbits.RA0
#define NODE1_BUTTON_ADCON                        (ADCON1bits.PCFG = 0xA)

// Test outputs
#define NODE1_BUTTONS_LED                  LATDbits.LATD3

// Node message formats
#define NODE1_BUTTON_BYTE                   0u  // i.e., the first byte in the CAN
                                                // message will be for the button states
#define NODE1_BYTE_BUTTON1_BIT_LOC           0u
#define NODE1_BYTE_BUTTON2_BIT_LOC           1u
#define NODE1_CONSTRUCT_BUTTON_BYTE(b1,b2)   (((b1)<<NODE1_BYTE_BUTTON1_BIT_LOC) | ((b2)<<NODE1_BYTE_BUTTON2_BIT_LOC))
#define NODE1_SID                            0x001u
#define NODE1_EID                            0x00000u
#define NODE2_SID                            0x040u
#define NODE2_EID                            0x3FABCu
#define NODE2_ACK_MSG                        0x3Bu    // This will be used as an "acknowledge" msg from a receiver
#define NODE2_RX_FAIL_MSG                    0x2Au    // This will be used as a "failed to receive" msg from a receiver
#define NODE1_ACK_COUNTER_BYTES              1u       // Node1 will include an ack counter with each message in bytes 1 and 2
#define NODE1_TRANSMIT_COUNTER_BYTES         3u       // Node 1 will include a transmit counter with each message in bytes 3 and 4
#define NODE1_ENG_SPD_BYTES                  5u       // Node 1 will include an "engine speed" signal contained in bytes 5 and 6

#endif	/* MCP2515_TEST_HEADER */

