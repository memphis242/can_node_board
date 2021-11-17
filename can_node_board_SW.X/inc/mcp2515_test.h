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
#define NODE1_BUTTONS_PORT                         PORTE
#define NODE1_BUTTON1_PIN                         PORTEbits.RE0
#define NODE1_BUTTON2_PIN                         PORTEbits.RE1
#define NODE1_BUTTON1_TRIS                        TRISEbits.RE0 = 1u
#define NODE1_BUTTON2_TRIS                        TRISEbits.RE1 = 1u
#define NODE1_ENG_SPD_POT                         PORTAbits.RA0
#define NODE1_BUTTON_ADCON                        (ADCON1bits.PCFG = 0xA)

// Test outputs
#define NODE1_LED                  LATDbits.LATD3
#define NODE1_TOGGLE_LED           (NODE1_LED ^= 0x1u)

// Node message formats
// For this test, Node 1 has only one type of message, so just one ID is needed
#define NODE1_SID                            0x001u
#define NODE1_EID                            0x00000u
// For this test, Node 2 will have two types of messages, so two IDs are needed
// I'll use the same SID but two EIDs depending on the message
#define NODE2_SID                            0x040u
#define NODE2_EID_ACK                        0x3FABCu
#define NODE2_EID_LED_CMD                    0x01010u

// Node 1 Frame Data
#define NODE1_BUTTON_BYTE                   0u  // i.e., the first byte in the CAN
                                                // message will be for the button states
#define NODE1_ENG_SPD_BYTES                  1u       // Node 1 will include an "engine speed" signal contained in bytes 1 and 2
#define NODE1_ACK_COUNTER_BYTES              3u       // Node1 will include an ack counter with each message in bytes 3 and 4
#define NODE1_TRANSMIT_COUNTER_BYTES         5u       // Node 1 will include a transmit counter with each message in bytes 5 and 6

#define NODE1_BYTE_BUTTON1_BIT_LOC           0u
#define NODE1_BYTE_BUTTON2_BIT_LOC           1u
#define NODE1_CONSTRUCT_BUTTON_BYTE          ((NODE1_BUTTON1_PIN<<NODE1_BYTE_BUTTON1_BIT_LOC) | (NODE1_BUTTON2_PIN<<NODE1_BYTE_BUTTON2_BIT_LOC))

// Node 2 Frame Data
//#define NODE2_ACK_MSG                        0x3Bu    // This will be in byte 0
//#define NODE2_LED_CMD_MSG                    0x10u    // This will be in byte 1

#endif	/* MCP2515_TEST_HEADER */

