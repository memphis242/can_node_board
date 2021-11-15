/* 
 * File: mccp2515.h
 * Author: Abdullah Almosalami
 * Comments: This is and the accompanying source file are for interfacing with
 *           the MCP2515 CAN Controller IC. This is where part of the magic
 *           happens and is a central part of the project!
 * Revision history: Ver. 0.1
 * 
 * NOTE! Along with my own reading of the datasheet and some app notes, references
 * I used for the development of this header and the source file was the
 * following git repos:
 *      - https://github.com/spirilis/mcp2515.git
 *      - https://github.com/autowp/arduino-mcp2515.git
 * I have my own take on the driver files for this IC but these were very
 * helpful. I should also add that my intent went beyond just making a CAN
 * node; I want to also send specific J1939 messages.
 * 
 */


// <editor-fold defaultstate="collapsed" desc="OVERVIEW OF THE MCP2515 CAN CONTROLLER">
/* *****************************************************************************
 * Overview of the MCP2515 CAN Controller IC
 * *****************************************************************************
 * Every CAN node requires a processor/microcontroller, a CAN Controller (either
 * as a separate IC or integrated as a peripheral on the processor/microcontroller)
 * to handle much of the CAN Physical & Data Link layers, and a CAN Transceiver
 * to deliver the required differential signal on the CANH and CANL lines. For
 * this project, the PIC18F4620 will be the microcontroller, the MCP2515 will be
 * the CAN Controller, and the MCP2562 will be the CAN transceiver.
 * 
 * The MCP2515 implements CAN 2.0B with a maximum transmission rate of 1 Mb/s.
 * (Note, if this was a CAN FD Controller, we'd have separate max Arbitration Rate
 * and Data Transmission Rate). It can handle both Standard and Extended frame
 * formats. It has two receive buffers (and to accompany that six 29-bit filters
 * and two 29-bit masks) and three transmission buffers. You interface to the
 * controller via SPI, that can operate at a maximum of 10MHz in modes 0,0 or
 * 1,1 (i.e., clock polarity 0 or 1, clock data phase 0 or 1). It has several
 * pins that may be used as external interrupts, like a configurable ~INT pin
 * along with two ~RXxBF pins to indicate a receive buffer is filled, or those
 * pins may be set as I/O (controlled over SPI). It also has three ~TXxRTS pins
 * for requested to send a frame held in a transmission buffer (which can also be
 * done over SPI). It also has OSC1 and OSC2 pins, much like a microcontroller,
 * where you can either configure the IC to use an external crystal or external
 * clock, with a maximum clock frequency of 40MHz. Finally, the controller
 * interfaces with a transceiver via its TXCAN and RXCAN pins, which go to
 * corresponding pins on a typical CAN transceiver.
 * 
 * For this project, I intend to use the extended frame format, with the bus
 * running at 250kbps (although, at some point I will make this more configurable),
 * since I intend to work on J1939 CAN buses that are typical on off-road vehicles.
 * I intend to operate SPI at 2.5MHz and have a 40MHz crystal delivering the
 * oscillation for the IC's clock. The choice of 40MHz (the max) was so that it
 * gives me a lot more flexibility and accuracy in terms of bit-timing, especially
 * to achieve up to 1Mbps. There are a lot more details and design choices made,
 * and the datasheet is decently chunky, so please contact me if you have any
 * questions!
 */
// </editor-fold>


#ifndef MCP2515_HEADER
#define	MCP2515_HEADER


#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>


// <editor-fold defaultstate="collapsed" desc="DEFINITIONS - Registers and Macros">
#ifndef EXEC_FAIL
#define EXEC_FAIL       0
#define EXEC_SUCCESS    1
#endif

#ifndef _XTAL_FREQ
// 40MHz --> Defining this will allow use of __delay_ms()
#define _XTAL_FREQ  40000000u
#endif



// <editor-fold defaultstate="collapsed" desc="CANCTRL & CANSTAT">
/* Control Register - CANCTRL - Register 10-1 in Datasheet
 * NOTE: It appears the register is repeated (?) or that all addresses in
 *       the range 0xXF map to the same register. So for example, 0x0F, 0x1F,
 *       ... all map to the Control Register.
 * Default/POR: 100 0 0 1 11
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |.............REQOP...................|...ABAT....|....OSM....|...CLKEN...|.......CLKPRE..........|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains bits to
 *      - control requests of operation mode REQOP[2:0]
 *          * 000 = Normal Operation mode
 *          * 001 = Sleep mode
 *          * 010 = Loopback mode (potentially used for debugging)
 *          * 011 = Listen-Only mode
 *          * 100 = Configuration mode (default POR state)
 *          * NOTE: The operation mode doesn't change immediately upon writing
 *                  to these bits - there are conditions that have to be met first.
 *                  You check the status register to actually check what the op
 *                  mode is currently at.
 *      - a bit to abort pending transmissions ABAT
 *      - a bit to enable/disable One-Shot Mode
 *      - a bit to enable/disable CLOKOUT pin (when disabled, CLKOUT is floating)
 *      - two bits to control prescalar of CLKOUT
 *          * 00 = F_CLKOUT = System Clock / 1
 *          * 01 = "" / 2
 *          * 10 = "" / 4
 *          * 11 = "" / 8
 * 
 */
#define MCP2515_CANCTRL                     0x0F    // could also do 0x1F, 0x2F, ..., 0xEF
#define MCP2515_CANCTRL_REQOP_BITS          0xE0
#define MCP2515_CANCTRl_REQOP_NORMAL        0x00
#define MCP2515_CANCTRl_REQOP_SLEEP         0x20
#define MCP2515_CANCTRl_REQOP_LOOPBACK      0x40
#define MCP2515_CANCTRl_REQOP_LISTEN_ONLY   0x60
#define MCP2515_CANCTRl_REQOP_CONFIG        0x80
#define MCP2515_ABORT_TRANSMISSION          0x10
#define MCP2515_ONE_SHOT_MODE               0x08
#define MCP2515_CLKOUT_ENABLE               0x04
#define MCP2515_CLKOUT_PRESCALE_1           0x00
#define MCP2515_CLKOUT_PRESCALE_2           0x01
#define MCP2515_CLKOUT_PRESCALE_4           0x02
#define MCP2515_CLKOUT_PRESCALE_8           0x03

/* Status Register - CANSTAT - Register 10-2 in Datasheet
 * NOTE: It appears the register is repeated (?) or that all addresses in
 *       the range 0xXE map to the same register. So for example, 0x0E, 0x1E,
 *       ... all map to the Status Register.
 * Default/POR: 100 0 000 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |.............OPMOD...................|...undef...|...........ICOD....................|...undef...|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains bits to
 *      - indicate current operation mode OPMOD[2:0]
 *          * 000 = Normal Operation mode
 *          * 001 = Sleep mode
 *          * 010 = Loopback mode (potentially used for debugging)
 *          * 011 = Listen-Only mode
 *          * 100 = Configuration mode (default POR state)
 *      - indicate which interrupt has occured. If multiple, the highest
 *        priority (ERR > WAK > TXB0 > TX1 > TXB2 > RXB0 > RXB1) is shown.
 *        Also note that the CANINTF register contains the interrupt flag bits
 *        for the eight interrupt sources.
 *          * 000 = No interrupt
 *          * 001 = Error interrupt
 *          * 010 = Wake-up interrupt
 *          * 011 = TXB0 interrupt
 *          * 100 = TXB1 interrupt
 *          * 101 = TXB2 interrupt
 *          * 110 = RXB0 interrupt
 *          * 111 = RXB1 interrupt
 * 
 */
#define MCP2515_CANSTAT                 0x0E    // could also do 0x1E, 0x2E, ..., 0xEE
#define MCP2515_CANSTAT_OPMODE_BITS     0xE0
typedef enum {
    MCP2515_OPMODE_NORMAL = 0u,
    MCP2515_OPMODE_SLEEP = 1u,
    MCP2515_OPMODE_LOOPBACK = 2u,
    MCP2515_OPMODE_LISTEN = 3u,
    MCP2515_OPMODE_CONFIG = 4u,
} opmode_t;
#define MCP2515_CANSTAT_ICOD_BITS       0x0E    // ICOD = "Interrupt CODe"
// </editor-fold>

#define MCP2515_MSG_BUFF_SIZE_BYTES     13u     // 13 bytes that actually go into a message

// <editor-fold defaultstate="collapsed" desc="TX-RELATED">
/* ****************************************************************************
 * Transmit-Related
 * ****************************************************************************
 * There are three transmit buffers, each occupying 14 bytes of SRAM.
 *      - The first byte is TXBnCTRL, which is the control register associated
 *        with each transmit buffer. It determines conditions of transmission
 *        and indicates the status of the message transmission.
 *      - Five bytes are used for the Standard and Extended Identifiers, as well
 *        bits for RTR and DLC bits
 *      - The remaining 8 bytes are for the data bytes
 * In addition, there is a TXRTSCTRL - ~TXnRTS Pin Control & Status register -
 * for dealing with the ~TXnRTS pins (setting them as digital inputs or as
 * request to send).
 * The minimum required for a transmission is TXBnSIDH, TXBnSIDL, and DLC.
 * TXREQ can be set via 1) SPI Command SPI RTS, 2) ~TXnRTS pin, 3) SPI Write to TXBnCTRL
 * 
 * GENERAL STEPS FOR TRANSMISSION:
 * (FILL A TX BUFFER <--> CONFIGURE NECESSARY CTRL) --> SET CORRESPONDING TXREQ BIT
 *      --> Controller awaits bus to be available --> Controller looks at priority
 *      of buffers (set in TXBnCTRL::TXP) --> Transmits
 * 
 *          --> If successful
 *              --> Controller clears TXREQ bit
 *              --> If interrupt was enabled for TX, generate interrupt (TXnIE in CANINTE)
 *              --> Regardless of interrupt enable, set the TXnIF flag
 * 
 *          --> If fail
 *              --> Due to Error
 *                  --> Set TXERR bit in TXBnCTRL
 *                  --> If interrupt was enabled for ERR, generate interrupt (MERRE in CANINTE)
 *                  --> Regardless of interrupt enable, set the MEERF flag
 *              --> Due to Arbitration Loss
 *                  --> Set MLOA bit in TXBnCTRL
 *                  --> Depending on configuration 
 *              --> NOTE: If in One-Shot mode, a fail will cause the RTS bit to be cleared,
 *                        no matter the cause. Otherwise, the message is still loaded in
 *                        the buffer and will attempt to retransmit unless you abort the
 *                        transmission. Other CAN Controller ICs have more options for number
 *                        of re-attempts allowed, but not this controller.
 * 
 *      --> You should clear the corresponding flag bit before attempting another TX
 *      --> If a message was aborted by setting the ABAT bit, you need to clear it
 *          before attempting to send again!
 * 
*/
/* Transmit Buffer n Control Register - TXBnCTRL - Register 3-1 in Datasheet
 * NOTE: There are three transmit buffers, 0, 1, and 2.
 * Default/POR: 0 0 0 0 0 0 00
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....undef...|....ABTF....|....MLOA...|...TXERR...|....TXREQ..|....undef..|.........TXP...........|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains bits for:
 *      (R)- Indicate message was aborted or not ABTF (this is not ABAT in CANCTRL!)
 *      (R)- Indicate message arbitration was lost when transmit was attempted MLOA
 *      (R)- Indicate a bus error occured when transmit was attempted TXERR
 *      (R/W)- Indicate whether a message is pending transmission, or you can write
 *             and request transmission. This is automatically cleared upon successful
 *             transmit. Clearing this before a transmit occurs aborts the message.
 *      (R/W)- Bits for message buffer priority TXP
 *          * 11 = Highest priority
 *          * 10 = High intermediate
 *          * 01 = Low intermediate
 *          * 00 = Lowest priority
 * 
 */
#define MCP2515_TXB0CTRL            0x30
#define MCP2515_TXB1CTRL            0x40
#define MCP2515_TXB2CTRL            0x50
// The following apply to all the TXB Control Registers
#define MCP2515_TXBnCTRL_ABTF       0x40
#define MCP2515_TXBnCTRL_MLOA       0x20
#define MCP2515_TXBnCTRL_TXERR      0x10
#define MCP2515_TXBnCTRL_TXREQ      0x08
#define MCP2515_TXBnCTRL_TXP        0x03

/* ~TXRTSCTRL Pin Control & Status Register - TXRTSCTRL - Register 3-2 in Datasheet
 * Default/POR: 0 0 x x x 0 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....undef...|....undef...|....B2RTS..|...B1RTS...|....B0RTS..|..B2RTSM...|..B1RTSM...|..B0RTSM...|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains bits for:
 *      (R)- B2RTS: Pin state of ~TX2RTS --> if digital input, shows state; if RTS, reads as 0
 *      (R)- B1RTS: Pin state of ~TX1RTS --> if digital input, shows state; if RTS, reads as 0
 *      (R)- B0RTS: Pin state of ~TX0RTS --> if digital input, shows state; if RTS, reads as 0
 *      (R/W)- B2RTSM: ~TX2RTS Pin mode; 1 = RTS, 0 = Digital Input
 *      (R/W)- B1RTSM: ~TX1RTS Pin mode; 1 = RTS, 0 = Digital Input
 *      (R/W)- B0RTSM: ~TX0RTS Pin mode; 1 = RTS, 0 = Digital Input
 */
#define MCP2515_TXRTSCTRL                   0x0D
// some more todo...
#define MCP2515_TXRTSCTRL_TX2RTS_MODE_RTS   0x04
#define MCP2515_TXRTSCTRL_TX1RTS_MODE_RTS   0x02
#define MCP2515_TXRTSCTRL_TX0RTS_MODE_RTS   0x01
#define MCP2515_TXRTSCTRL_TXnRTS_MODE_DI    0x00

/* The actual buffer content registers are outlined below. Note that:
 *      - TXB0 takes up 0x30 to 0x3D, with 0x31 --> 0x3D containing the frame-specific bytes
 *      - TXB1 takes up 0x40 to 0x4D, ...
 *      - TXB2 takes up 0x50 to 0x5D, ...
 */
// TXB0
#define MCP2515_TXB0SIDH        0x31
#define MCP2515_TXB0SIDL        0x32
#define MCP2515_TXB0EID8        0x33
#define MCP2515_TXB0EID0        0x34
#define MCP2515_TXB0DLC         0x35
#define MCP2515_TXB0D0          0x36
#define MCP2515_TXB0D1          0x37
#define MCP2515_TXB0D2          0x38
#define MCP2515_TXB0D3          0x39
#define MCP2515_TXB0D4          0x3A
#define MCP2515_TXB0D5          0x3B
#define MCP2515_TXB0D6          0x3C
#define MCP2515_TXB0D7          0x3D
// TXB1
#define MCP2515_TXB1SIDH        0x41
#define MCP2515_TXB1SIDL        0x42
#define MCP2515_TXB1EID8        0x43
#define MCP2515_TXB1EID0        0x44
#define MCP2515_TXB1DLC         0x45
#define MCP2515_TXB1D0          0x46
#define MCP2515_TXB1D1          0x47
#define MCP2515_TXB1D2          0x48
#define MCP2515_TXB1D3          0x49
#define MCP2515_TXB1D4          0x4A
#define MCP2515_TXB1D5          0x4B
#define MCP2515_TXB1D6          0x4C
#define MCP2515_TXB1D7          0x4D
// TXB2
#define MCP2515_TXB2SIDH        0x51
#define MCP2515_TXB2SIDL        0x52
#define MCP2515_TXB2EID8        0x53
#define MCP2515_TXB2EID0        0x54
#define MCP2515_TXB2DLC         0x55
#define MCP2515_TXB2D0          0x56
#define MCP2515_TXB2D1          0x57
#define MCP2515_TXB2D2          0x58
#define MCP2515_TXB2D3          0x59
#define MCP2515_TXB2D4          0x5A
#define MCP2515_TXB2D5          0x5B
#define MCP2515_TXB2D6          0x5C
#define MCP2515_TXB2D7          0x5D

/* TXBnSIDH Transmit Buffer n Standard Identifier Register HIGH - Register 3-3 in Datasheet
 * Addresses 0x31, 0x41, 0x51
 * Default/POR: xxxx xxxx
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....SID10...|....SID9....|....SID8...|...SID7....|....SID6...|...SID5....|...SID4....|...SID3....|
 * +-------------------------------------------------------------------------------------------------+
 *
 * TXBnSIDL Transmit Buffer n Standard Identifier Register LOW - Register 3-4 in Datasheet
 * Addresses 0x32, 0x42, 0x52
 * Default/POR: xxx 0 x 0 xx
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....SID2....|....SID1....|....SID0...|...undef...|...EXIDE...|...undef...|...EID17...|...EID16...|
 * +-------------------------------------------------------------------------------------------------+
 * 
 * TXBnEID8 Transmit Buffer n Extended Identifier 8 Register HIGH - Register 3-5 in Datasheet
 * Addresses 0x33, 0x43, 0x53
 * Default/POR: xxxx xxxx
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....EID15...|....EID14...|....EID13..|...EID12...|....EID11..|...EID10...|...EID9....|...EID8....|
 * +-------------------------------------------------------------------------------------------------+
 * 
 * TXBnEID0 Transmit Buffer n Extended Identifier 0 Register LOW - Register 3-6 in Datasheet
 * Addresses 0x34, 0x44, 0x54
 * Default/POR: xxxx xxxx
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....EID7....|....EID6....|....EID5...|...EID4....|....EID3...|...EID2....|...EID1....|...EID0....|
 * +-------------------------------------------------------------------------------------------------+
 * 
 * TXBnDLC Transmit Buffer n Data Length Code Register - Register 3-7 in Datasheet
 * Addresses 0x35, 0x45, 0x55
 * Default/POR: 0 x 00 xxxx
 * NOTE: For J1939 messages, RTR is ALWAYS 0 (i.e., there are no Remote Frames)
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....undef...|....RTR....|....undef..|...undef....|....DLC3...|...DLC2....|...DLC1....|...DLC0....|
 * +-------------------------------------------------------------------------------------------------+
 * 
 * And then come 8 registers for the data bytes...
 * 
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="RX-RELATED">
/* ****************************************************************************
 * Receive-Related
 * ****************************************************************************
 * There are two receive buffers, RXB0 and RXB1, similarly laid out in memory 
 * like the transmission buffers. The RXB0 registers start from 0x60 and go to
 * 0x6D, with 0x60 being RXB0CTRL and 0x61 to 0x6D being RXB0SIDH, SIDL, EID8,
 * EID0, DLC, D0, D1, ..., D7. Likewise, RXB1 starts at 0x70 and goes through to
 * 0x7D. By the way, the register map stops at 0x7F (i.e., 128 registers total).
 * 
 * In addition, however, there are also acceptance filters and masks associated
 * with the receive buffers. Also, before a message is transferred into either
 * buffer, incoming messages are assembled by the Message Assembly Buffer (MAB).
 * Only when the message in the MAB meets filter criteria for a buffer will the
 * message be transferred over. When such a transfer occurs, the RXnIF bit in
 * CANINTF is set, and this bit must be cleared by the uC in order to allow
 * another message to be received into the buffer.
 * 
 * RXB0 has one mask and two acceptance filters associated with it - I will call
 * these RX_MASK0 and RXFILT0 and RXFILT1 respectively. RXB1 has one mask and
 * four acceptance filters associated with it - I will call these RX_MASK1 and
 * RXFILT2, RXFILT3, RXFILT4, and RXFILT5. When a message is received, the
 * RXBnCTRL[3:0} bits will indicate the acceptance filter number that enabled
 * reception and whether the received message is a Remote Transfer Request. It
 * should be noted that if a message matches filters for both RXB0 and RXB1,
 * RXB0 has higher priority and only it gets loaded with the message.
 * 
 * GENERAL STEPS FOR RECEPTION:
 * SoF detected --> Load MAB (if invalid message, generate error frame and go
 * back to start)
 *      --> Meets filter criteria for RXB0?
 *          Y --> Is RX0IF = 0?
 *              Y --> Move message into RXB0 --> Set RX0IF --> Set FILHIT0 based
 *                    on which filter hit matched --> If RX0IE is set, generate
 *                    interrupt on ~INT pin and set CANSTAT::ICOD accordingly
 *                    --> If B0BFM and B0BFE are set, pull ~RXBF0 low
 *                    --> Back to start
 *              N --> Is BUKT (RXB0CTRL[2]) set?
 *                    Y --> Is RX1IF = 0?
 *                          Y --> Move message into RXB1 and go through similar
 *                                checks as outlined for RXB0
 *                          N --> Generate overflow error : set RX1OVR (EGLF[7]),
 *                                and generate interrupt on ~INT if ERRIE
 *                    N --> Generate overflow error and ... ""
 * 
 * 
 */
/* Receive Buffer 0 Control Register - RXB0CTRL - Register 4-1 in Datasheet
 * NOTE: There are two receive buffers, RXB0 and RXB1.
 * Default/POR: 0 00 0 0 00 0
 * +--------------- ---------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....undef...|.........RXM............|...undef...|....RXRTR..|...BUKT....|...BUKT1...|..FILHIT0..|
 * +-------------------------------------------------------------------------------------------------+
 * In this register, we have,
 *      (R/W) - RXM : Receive Buffer Operating mode bits
 *              * 11 = Turns masks/filters off, receives all messages
 *              * 10, 01 = Reserved
 *              * 00 = Receives all valid messages that meet filter criteria
 *
 *      (R) - RXRTR : Received Remote Transfer Request
 *      (R/W) - BUKT : Rollover enable --> BUKT1 is a copy of BUKT, used internally by MCP2515
 *      (R) - FILHIT0 : Filter Hit bit (indicates which acceptance filter enabled reception of message)
 *              * 1 = Acceptance filter 1 RXF1 --> for me, RX_FILT1
 *              * = = Acceptance filter 0 RXF0 --> for me, RX_FILT0
 * 
 */
/* Receive Buffer 1 Control Register - RXB1CTRL - Register 4-2 in Datasheet
 * NOTE: There are two receive buffers, RXB0 and RXB1.
 * Default/POR: 0 00 0 0 000
 * +--------------- ---------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....undef...|.........RXM............|...undef...|....RXRTR..|.............FILHIT................|
 * +-------------------------------------------------------------------------------------------------+
 * In this register, we have,
 *      (R/W) - RXM : Receive Buffer Operating mode bits
 *              * 11 = Turns masks/filters off, receives all messages
 *              * 10, 01 = Reserved
 *              * 00 = Receives all valid messages that meet filter criteria
 *
 *      (R) - RXRTR : Received Remote Transfer Request
 *      (R) - FILHIT : Filter Hit bit (indicates which acceptance filter enabled reception of message)
 *              * 101, 100, 011, 010 for RXF5, RXF4, RXF3, and RXF2 (RX_FILT5, RX_FILT4, RX_FILT3, RX_FILT2)
 *              * 001, 000 for RXF1 and RXF0 only if BUKT is set in RXB0CTRL and a message came through from RXB0's side
 * 
 */
#define MCP2515_RXB0CTRL                    0x60
#define MCP2515_RXB1CTRL                    0x70
#define MCP2515_RXBnCTRL_RXM                0x60
#define MCP2515_RXBnCTRL_RXM_RX_ALL         0x00
#define MCP2515_RXBnCTRL_RXM_RX_FILT        0x60
#define MCP2515_RXB0CTRL_BUKT               0x04
#define MCP2515_RX_ROLLOVER                 0x04
#define MCP2515_RXBnCTRL_RTR                0x08
#define MCP2515_RXB0CTRL_FILHIT0            0x01
#define MCP2515_RXB0_WHICH_FILT(rxb0_reg)    (rxb0_reg & 0x01)
#define MCP2515_RXB1CTRL_FILHIT             0x07

/* ~RXnBF Pin Control and Status Register - BFPCTRL - Register 4-3 in Datasheet
 * Default/POR: 0 0 0000 00
 * +--------------- ---------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....undef...|...undef....|....B1BFS..|...B0BFS...|...B1BFE...|...B0BFE...|...B1BFM...|...B0BFM...|
 * +-------------------------------------------------------------------------------------------------+
 * In this register, we have,
 *      (R/W) - B1BFS : ~RX1BF Pin state bit (digital output mode only)
 *      (R/W) - B0BFS : ~RX0BF Pin state bit (digital output mode only)
 *      (R/W) - B1BFE : ~RX1BF Pin function enable bit
 *                      * 1 = pin function is enabled, op mode is determined by B1BFM bit
 *                      * 0 = pin function disabled, pin goes high-impedance
 *      (R/W) - B0BFE : "" same but for ~RX0BF
 *      (R/W) - B1BFM : Pin is used as an interrupt (1) or digital output (0)
 *      (R/W) - B0BFM : ""
 * 
 */
#define MCP2515_BFPCTRL                     0x0D
// not yet planning on using ~RXnBF pins as outputs, so I'll leave def's and macros out for that for now...
#define MCP2515_BFPCTRL_RXnBF_ENABLE_BITS   0x0C
#define MCP2515_BFPCTRL_RXnBF_MODE_BITS     0x03
#define MCP2515_BFPCTRL_B1BFE               0x08
#define MCP2515_BFPCTRL_B0BFE               0x04
#define ENABLE_RX1BF_PIN                    0x08
#define ENABLE_RX0BF_PIN                    0x04
#define MCP2515_BFPCTRL_B1BFM               0x02
#define MCP2515_BFPCTRL_B0BFM               0X01
#define SET_RX1BF_AS_INTERRUPT              0x02
#define SET_RX0BF_AS_INTERRUPT              0x01
#define SET_RXnBF_DEFAULT                   0x0F


/* As stated earlier, RXB0 content goes from 0x61 to 0x6D, and RXB1 content goes from 0x71 to 0x7D
 * Refer to the comments I made earlier laying out the registers for TXBnSIDH, ..., TXBnD7; RXBn's
 * are the same.
 */
// RXB0
#define MCP2515_RXB0SIDH        0x61
#define MCP2515_RXB0SIDL        0x62
#define MCP2515_RXB0EID8        0x63
#define MCP2515_RXB0EID0        0x64
#define MCP2515_RXB0DLC         0x65
#define MCP2515_RXB0D0          0x66
#define MCP2515_RXB0D1          0x67
#define MCP2515_RXB0D2          0x68
#define MCP2515_RXB0D3          0x69
#define MCP2515_RXB0D4          0x6A
#define MCP2515_RXB0D5          0x6B
#define MCP2515_RXB0D6          0x6C
#define MCP2515_RXB0D7          0x6D
// RXB1
#define MCP2515_RXB1SIDH        0x71
#define MCP2515_RXB1SIDL        0x72
#define MCP2515_RXB1EID8        0x73
#define MCP2515_RXB1EID0        0x74
#define MCP2515_RXB1DLC         0x75
#define MCP2515_RXB1D0          0x76
#define MCP2515_RXB1D1          0x77
#define MCP2515_RXB1D2          0x78
#define MCP2515_RXB1D3          0x79
#define MCP2515_RXB1D4          0x7A
#define MCP2515_RXB1D5          0x7B
#define MCP2515_RXB1D6          0x7C
#define MCP2515_RXB1D7          0x7D

/* How the Filters & Masks Interact
 * The masks determine which bits in the filter are actually compared to a message.
 * If a mask bit is 1, the corresponding filter bit WILL be compared.
 * If a mask bit is 0, the correspodning filter bit will be IGNORED.
 */
// Filters
/* NOTE! The filter and mask registers can only be modified in Configuration mode.
 * NOTE: The filter & mask register layout is exactly the same as the RXBn register 
 * layout except for the CTRL register and data bytes, which is not surprising.
 * Hence, each filter and mask register-set is four registers.
 */
// Filter 0 --> RXB0
#define MCP2515_RXF0SIDH    0x00
#define MCP2515_RXF0SIDL    0x01
#define MCP2515_RXF0EID8    0x02
#define MCP2515_RXF0EID0    0x03
// Filter 1 --> RXB0
#define MCP2515_RXF1SIDH    0x04
#define MCP2515_RXF1SIDL    0x05
#define MCP2515_RXF1EID8    0x06
#define MCP2515_RXF1EID0    0x07
// Filter 2 --> RXB1
#define MCP2515_RXF2SIDH    0x08
#define MCP2515_RXF2SIDL    0x09
#define MCP2515_RXF2EID8    0x0A
#define MCP2515_RXF2EID0    0x0B
// Filter 3 --> RXB1
#define MCP2515_RXF3SIDH    0x10
#define MCP2515_RXF3SIDL    0x11
#define MCP2515_RXF3EID8    0x12
#define MCP2515_RXF3EID0    0x13
// Filter 4 --> RXB1
#define MCP2515_RXF4SIDH    0x14
#define MCP2515_RXF4SIDL    0x15
#define MCP2515_RXF4EID8    0x16
#define MCP2515_RXF4EID0    0x17
// Filter 4 --> RXB1
#define MCP2515_RXF5SIDH    0x18
#define MCP2515_RXF5SIDL    0x19
#define MCP2515_RXF5EID8    0x1A
#define MCP2515_RXF5EID0    0x1B

// Masks
// Mask 0 --> RXB0
#define MCP2515_RXM0SIDH    0x20
#define MCP2515_RXM0SIDL    0x21
#define MCP2515_RXM0EID8    0x22
#define MCP2515_RXM0EID0    0x23
// Mask 1 --> RXB1
#define MCP2515_RXM1SIDH    0x24
#define MCP2515_RXM1SIDL    0x25
#define MCP2515_RXM1EID8    0x26
#define MCP2515_RXM1EID0    0x27

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="BIT-TIMING RELATED">
/* The CNF1, CNF2, and CNF3 registers are those that set the bit-timing for the
 * device (and also how you set the bus rate). I'll briefly summarize what
 * bit-timing is and how you might want to configure these registers, but there's
 * more detail to it and I would at least refer to the MCP2515 datasheet (I also
 * think the MCP2518FD datasheet's section on bit-timing is pretty good).
 * 
 * ----------------------------------------------------------------------------
 * BIT-TIMING QUICK-NOTES
 * ---------------------------------------------------------------------------- 
 * Bit-timing has to do with how the fact that CAN is asynchronous - there is no
 * clock line. So how do we know when a bit ends or starts? Well, all nodes agree
 * to a rate, such as 250kbps. Ok, but how do we know all the clocks are in phase?
 * Aha! This is where bit-timing comes in. In general, asynchronous comm controllers
 * sync up on an data-line edge, and for CAN, that's the SoF 1->0 transition. The
 * syncing is done using a PLL, and the controller sets its clock phase forward
 * or backward a certain amount of time to sync up. As a part of this, every bit
 * time (i.e., the period of the bus rate) is split up into many sections called
 * Time Quanta (TQ). This depends largely on what the controller's system clock
 * is at - the higher its freq, the more sections can be split up, and in the end,
 * the more resolution in adjustment can be done in a given bit-time. For the
 * MCP2515, TQ = 2 * (BRP[5:0] + 1) / FOSC, where BRP[5:0] is in the CNF1 register.
 * The controller bit-rate is in turn set by how many TQ's the controller assumes
 * for a given bit-time. So for example, if TQ = 200ns and we set 20 quanta per
 * bit time, that gives us 4us per bit, which amounts to 250kbps.
 * 
 * Now these individual time-quanta are grouped together into four main segments:
 * a sync segment (always 1 TQ), a propagation segment, a phase 1 segment, and a
 * phase 2 segment. I will call these NSYNC, NPROPSEG, NPH1, and NPH2. NSYNC is,
 * as stated earlier, when the controller syncs up its clock with the SoF transition.
 * NPROPSEG is for giving enough time for a bit to be sent through the bus (you
 * want to at least give enough time for 2 * (t_txd_rxd + t_bus), where t_txd_rxd
 * is the tx to rx loop delay of the transceiver and t_bus is the amount of time
 * it takes for a signal to propagate to the other side of the bus --> you can
 * roughly estimate this to be [5 ns/m of wire] x [the length of the wire]).
 * NPH1 gives a little more time for propagation delays and right after NPH1 is
 * when the bit is actually sampled by the controller. It is often reported in
 * CAN configurations what the nominal sample point (NSMP) % is, which is to say at 
 * what point in the bit-time does the sample point occur, and obviously this is
 * (NSYNC + NPROPSEC + NPH1) / (NT), where I'll use NT to designate the total
 * number of TQ per bit time. Indeed it is usually this percentage that is
 * specified, and accordingly you compute the amount of TQ to give to NPH1, after
 * determining a minimum for NPROPSEG based on the propagation delay calculations
 * you made earlier. Finally, NPH2 is the tail-half of this bit-time grouping, and
 * gives enough time to controllers to process before beginning another message.
 * As part of the NPH2 segment, we have a SJW section, which is also used to
 * re-synchronize based on misalignments detected during this bit.
 * 
 * It is generally recommended to make Fosc as high as possible to make TQ small
 * and allow for finer resolution and adjustment. It is also recommended to
 * maximize SJW and minimize BRP. Also, all nodes on the bus should agree on
 * NSMP %.
 * 
 * Note that for the MCP2515, the maximum number of TQ per bit-time is 25, where
 * NSYNC = 1TQ, NPROPSEG = 8TQ, NPH1 = 8TQ, AND NPH2 = 8TQ.
 * 
 * ----------------------------------------------------------------------------
 * I will show my calculations for 250kbps bus rate example, with a nominal
 * sample point of 70% and 2m of bus length. Assume Fosc = 40MHz. Assume you
 * are using the MCP2562 transceiver, which has a t_tx_rx of 235ns.
 * 
 * 250kbps means a bit time of 4us. I'll chose NT to be 20 (i.e., 20 TQ per bit-time).
 * That means TQ = 200ns. With Fosc = 40MHz, BRP = 3 (see calc below for TQ in CNF1).
 * Ok, this sets the bit-rate.
 * To get NSMP to be 70%, NPROPSEG + NPH1 needs to be 13. With 2m of bus length,
 * t_bus ~ 10ns. t_tx_rx is given as 235ns. This makes 2 * (t_tx_rx + t_bus) to
 * be around 490ns. This is the worst-case propagation delay. So, NPROPSEG needs
 * to be at least 3TQ. I'll choose it to be 5TQ, which sets NPH1 = 8TQ. This leaves
 * over NPH2 = 6TQ to complete the 20TQ for this bit-time split. So overall,
 * 
 * BRP: 3       NPROPSEG = 5TQ      NPS1 = 8TQ      NPS2 = 6TQ
 * 
 * RECALL that for PRSEG, PHSEG1, and PHSEG2, subtract 1 from the values you
 * calculate, because the MCP2515 takes the register value and adds 1 (see
 * the calculations within the registers below). This is so that if you enter 0
 * for any of those times, the lowest the actual value goes is 1TQ. So:
 * 
 * BRP: 3       PRSEG: 4        PHSEG1 = 7      PHSEG2 = 5
 * ----------------------------------------------------------------------------
 * So, with that, it becomes straightforward to configure CNF1, CNF2, and CNF3.
 * 
 */

/* CNF1 Configuration Register 1 - Register 5-1 in Datasheet
 * Address 0x2A
 * Default/POR: 00 000000
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...........SJW...........|..........................BRP..........................................|
 * +-------------------------------------------------------------------------------------------------+
 * This registers contains:
 *      (R/W) - SJW[1:0] : Synchronization Jump Width bits
 *                      * 11 = 4 TQ
 *                      * 10 = 3 TQ
 *                      * 01 = 2 TQ
 *                      * 00 = 1 TQ
 *      (R/W) - BRP[5:0] : Baud Rate Prescaler bits
 *                      * TQ = 2 x (BRP[5:0] + 1) / FOSC
 * 
 * CNF2 Configuration Register 2 - Register 5-2 in Datasheet
 * Address 0x29
 * Default/POR: 0 0 000 0 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...BTLMODE..|...SAM......|................PHSEG1.............|..............PRSEG................|
 * +-------------------------------------------------------------------------------------------------+
 * This registers contains:
 *      (R/W) - BTLMODE : PS2 Bit Time length (PS2 is the same as NPH2)
 *                      * 1 = length of PS2 is determined by the PHSEG2 bits of CNF3
 *                      * 0 = length of PS2 is the greater of PS1 and IPT (2TQ)
 *      (R/W) - SAM : Sample bit configuration
 *                      * 1 = Bus line is sampled three times
 *                      * 0 = Bus line is sampled once
 *      (R/W) - PHSEG1 : PS1 length bits    (PHSEG1[2:0] + 1) x TQ
 *      (R/W) - PRSEG : Propagation Segment length bits (PRSEG[2:0] + 1) x TQ
 * 
 * CNF3 Configuration Register 3 - Register 5-3 in Datasheet
 * Address 0x28
 * Default/POR: 00 0 0 0 000
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |....SOF.....|...WAKFIL...|..............undef................|.............PHSEG2................|
 * +-------------------------------------------------------------------------------------------------+
 * This registers contains:
 *      (R/W) - SOF : Start-of-Frame signal bit -- if CLKEN, 1 = CLKOUT pin is used for SOF
 *      (R/W) - WAKFIL : Wake-up Filter bit enable
 *      (R/W) - PHSEG2[2:0] : PS2 length bits (PHSEG2[2:0] + 1) x TQ -- minimum 2TQ
 * 
 */

#define MCP2515_CNF1            0x2A
#define MCP2515_CNF2            0x29
#define MCP2515_CNF3            0x28

#define CNF1_SJW_BITS                0xC0
#define CNF1_SET_SJW_4TQ             0xC0
#define CNF1_SET_SJW_3TQ             0x80
#define CNF1_SET_SJW_2TQ             0x40
#define CNF1_SET_SJW_1TQ             0x00
#define CNF1_BRP_BITS                0x3F
#define CNF1_SET_BRP(x)              (0x3F & (x-1))

#define CNF2_BTLMODE_BITS            0x80
#define CNF2_SET_BTLMODE_CNF3        0x80
#define CNF2_SAM_BITS                0x40
#define CNF2_SET_SAM(x)              (x << 6u)
#define CNF2_PHSEG1_BITS             0x38
#define CNF2_SET_PHSEG1(x)           (x << 3u)
#define CNF2_PRSEG_BITS              0x07
#define CNF2_SET_PROPSEG(x)          (x)

#define CNF3_PHSEG2_BITS             0x07
#define CNF3_SET_PS2(x)              (x)
// don't care about SoF or Wake-up filter for now

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ERROR-RELATED">
/* There are five main types of errors in CAN:
 *  1- CRC Error - When the CRC computed on the received frame does not match the
 *                 CRC field of the frame
 *  2- ACK Error - In the ACK field of a message, the transmitting node checks if
 *                 if the ACK bit bit - which is always sent out as recessive with
 *                 the expectation that receiving nodes pull the lines dominant to
 *                 acknowledge - contains a dominant bit. If not, the transmitting
 *                 node assumes no node has received the message and an error occurs.
 *  3- Form Error - If a dominant bit is detected for any of the normally recessive
 *                  bits (EoF, IFS, ACK DEL, CRC DEL) ...
 *  4- Bit Error - If a transmitting node detects a bit level opposite to what it
 *                 sent (other than arbitration loss in the arbitration field)
 *  5- Stuff Error - If more than six consecutive bits of the same polarity are
 *                   detected. Normally, a tx node will add a complement bit after
 *                   5 consecutive bits to follow the stuffing rule and all receiving
 *                   nodes expect this complemented bit and ignore it.
 * 
 * If any of these errors are detected, as part of the CAN protocol, an error frame
 * is generated and the typical error-response on the bus will be observed. Usually,
 * a node will re-attempt its message if an error occurs. If too many error frames
 * in a row keep happening, either on the tx or rx side, then CAN has some rules
 * for this and each node takes on what are called error states. There are three:
 * Error-active, Error-passive, and Bus-off. Moving in between states generally
 * depends on internal counters in each CAN Controller - Receive Error Counter (REC)
 * and Transmit Error Counter (TEC). The way they increment/decrement is specified
 * in IS0-11898.
 * 
 * Upon reset, the MCP2515 starts in the Error-active state, where it is able to
 * send dominant error frames if it detects an error. Once REC or TEC pass 127,
 * the node goes into Error-passive state where it can only send passive error frames.
 * Once REC or TEC pass 255, the node goes into the Bus-off state, where it dettaches
 * itself from the bus, and only after 128 occurrences of 11 consecutive recessive bits
 * will it go back to Error-active.
 * 
 * To this end, the MCP2515 has three registers:
 *      1- TEC at 0x1C
 *      2- REC at 0x1D
 *      3- EFLG at 0x2D
 * 
 * EFLG Error Flag Register - Register 6-3 in Datasheet
 * Address 0x2D
 * Default/POR: 0 0 0 0 0 0 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |..RX1OVR....|...RX0OVR...|....TXBO...|...TXEP....|....RXEP...|...TXWAR...|....RXWAR..|...EWARN...|
 * +-------------------------------------------------------------------------------------------------+
 * This registers contains:
 *      (R/W) - RX1OVR : Sets when a valid message is received but both receive buffers are full (CANTINTF is 1)
 *                       Must be reset by MCU, hence it is R/W.
 *      (R/W) - RX0OVR : ""
 *      (R) - TXBO : Bus-off state flag
 *      (R) - TXEP : Error-passive state flag from TEC
 *      (R) - RXEP : Error-passive state flag from REC
 *      (R) - TXWAR : Transmit Error warning flag -- TEC > 96
 *      (R) - RXWAR : Receive Error warning flag -- REC > 96
 *      (R) - EWARN : Error Warning flag -- when either TEC or REC > 96
 */

#define MCP2515_EFLG            0x2D
#define MCP2515_EFLG_RX1OVR     0x80
#define MCP2515_EFLG_RX0OVR     0x40
#define MCP2515_EFLG_TXBO       0x20
#define MCP2515_EFLG_TXEP       0x10
#define MCP2515_EFLG_RXEP       0x08
#define MCP2515_EFLG_TXWARN     0x04
#define MCP2515_EFLG_RXWARN     0x02
#define MCP2515_EFLG_EARN       0x01

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="INTERRUPT-RELATED">

/* For interrupts, as you've probably seen by now if you've gone through the
 * previous comments, we have CANINTE for enabling on ~INT pin and CANINTF for
 * the flags.
 * 
 * CANINTE CAN Interrupt Enable Register - Register 7-1 in Datasheet
 * Address 0x2B
 * Default/POR: 0 0 0 0 0 0 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...MERRE....|....WAKIE...|...ERRIE...|...TX2IE...|...TX1IE...|...TX0IE...|...RX1IE...|...RX0IE...|
 * +-------------------------------------------------------------------------------------------------+
 * 
 * CANINTF CAN Interrupt Flag Register - Register 7-2 in Datasheet
 * Address 0x2C
 * Default/POR: 0 0 0 0 0 0 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...MERRF....|....WAKIF...|...ERRIF...|...TX2IF...|...TX1IF...|...TX0IF...|...RX1IF...|...RX0IF...|
 * +-------------------------------------------------------------------------------------------------+
 * 
 */

#define MCP2515_CANINTE         0x2B
#define MCP2515_CANINTF         0x2C
// For either register, the interrupt positions are the same, so:
#define MCP2515_MERR            0x80
#define MCP2515_WAKI            0x40
#define MCP2515_ERRI            0x20
#define MCP2515_TX2I            0x10
#define MCP2515_TX1I            0x08
#define MCP2515_TX0I            0x04
#define MCP2515_RX1I            0x02
#define MCP2515_RX0I            0x01
#define MCP2515_CANINTE_DEFAULT (MCP2515_ERRI | MCP2515_TX2I | MCP2515_TX1I | MCP2515_TX0I | MCP2515_RX1I | MCP2515_RX0I)

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="SPI-RELATED">
/* The MCP2515 accepts 9 different SPI instructions:
 * 1- RESET -- 1100 0000 : 0xC0 -- Resets internal registers and goes into Config mode
 * 
 * 2- READ -- 0000 0011 : 0x03 -- Reads data from the register beginning at the selected address
 *      The general flow of this operation goes:
 *          Set ~CS low --> Send READ instruction --> Send 8-bit address -->
 *          Read in addresses (sequential reading supported) --> Terminate with ~CS high
 * 
 * 3- READ RX BUFFER -- 1001 0nm0 : 0x9X -- Reads a receive buffer. This is equiv. to
 *                                          the READ instruction but without having to
 *                                          send the address. So just
 *                                          SEND READ RX BUFFER --> Read in bytes --> Terminate
 * 
 *                          nm: 00 = RXB0 starting at RXB0SIDH (0x61)
 *                              01 = RXB0 starting at RXB0D0 (0x66)
 *                              10 = RXB1 starting at RXB1SIDH (0x71)
 *                              11 = RXB1 starting at RXB1D0 (0x76)
 * 
 * 4- WRITE -- 0000 0010 : 0x02 -- Writes data to the register starting at the given address.
 *              CS LOW --> Send WRITE --> Send address --> Send bytes --> CS HIGH
 * 
 * 5- LOAD TX BUFFER -- 0100 0abc : 0x4X -- Writes to a TX buffer. Similar idea to READ RX BUFFER.
 * 
 *                          abc: 000 = TXB0 starting at TXB0SIDH (0x31)
 *                               001 = TXB0 starting at TXB0D0 (0x36)
 *                               010 = TXB1 starting at TXB1SIDH (0x41)
 *                               011 = TXB1 starting at TXB1D0 (0x46)
 *                               100 = TXB2 starting at TXB2SIDH (0x51)
 *                               101 = TXB2 starting at TXB2D0 (0x56)
 * 
 * 6- RTS - 1000 0nnn : 0x8X -- Instructs controller to begin message tx for the specific TX buffer
 *                              nnn: left bit is for TXB2, middle TXB1, right for TXB0 (can do multiple)
 * 
 * 7- READ STATUS -- 1010 0000 : 0xA0 -- Polls several status-type bits, including
 *                7      6      5               4     3                2      1               0
 *              RX0IF--RX1IF--TXREQ for TXB0--TX0IF--TXREQ for TXB1--TX1IF--TXREQ for TXB2--TX2IF
 * 
 * 8- RX STATUS -- 1011 0000 : 0xB0 -- Polls for filter match and message type of received message
 *                                     (see datasheet for details)
 * 
 * 9- BIT MODIFY -- 0000 0101 : 0x05 -- Similar to WRITE, however there is an additional mask sent to state which
 *                                      bits will be allowed to change and which won't. For example,
 *                                          MASK: 00110101
 *                                          DATA: xx10x0x1
 *                                          REG:  uu10u0u1  (u = unchanged)
 *                                      Also note that multiple bits can be modified, not just one.
 *                                      Also note that not all registers are bit-modifiable in this way. It is
 *                                      mainly the control registers (except for CANSTAT).
 *                            
 *                              START --> BIT MODIFY --> Address --> Mask --> Data
 * 
 */

#define MCP2515_SPI_RESET               0xC0
#define MCP2515_SPI_READ                0x03
#define MCP2515_SPI_READ_RXBUF(X)       (0x90 | ((X) << 1))   // Use with below enum spi_read_rxb_inst_t
typedef enum { SPI_READ_RXB0_ID, SPI_READ_RXB0_D, SPI_READ_RXB1_ID, SPI_READ_RXB1_D } spi_read_rxb_inst_t;
#define MCP2515_SPI_WRITE               0x02
#define MCP2515_SPI_LOAD_TXBUF(X)       (0x40 | (X))  // Use with below enum spi_load_txb_inst_t
typedef enum { SPI_LOAD_TXB0_ID, SPI_LOAD_TXB0_D, SPI_LOAD_TXB1_ID, SPI_LOAD_TXB1_D, SPI_LOAD_TXB2_ID, SPI_LOAD_TXB2_D } spi_load_txb_inst_t;
#define MCP2515_SPI_RTS(X)              (0x8 | (X)) // Use with txbuf_t enum
#define MCP2515_SPI_READ_STATUS         0xA0
#define MCP2515_SPI_READ_STATUS_RX0IF   (1u << 0u)
#define MCP2515_SPI_READ_STATUS_RX1IF   (1u << 1u)
#define MCP2515_SPI_READ_STATUS_TXREQ0  (1u << 2u)
#define MCP2515_SPI_READ_STATUS_TX0IF   (1u << 3u)
#define MCP2515_SPI_READ_STATUS_TXREQ1  (1u << 4u)
#define MCP2515_SPI_READ_STATUS_TX1IF   (1u << 5u)
#define MCP2515_SPI_READ_STATUS_TXREQ2  (1u << 6u)
#define MCP2515_SPI_READ_STATUS_TX2IF   (1u << 7u)
#define MCP2515_SPI_RX_STATUS           0xB0
// need a LOT of #defines for this instruction...
#define MCP2515_SPI_BIT_MODIFY          0x05

// </editor-fold>

// Related GPIO
#define RX0BF_FLAG          INT0_FLAG
#define RX1BF_FLAG          INT1_FLAG
#define MCP2515_RESET_FLAG  INT2_FLAG

// </editor-fold>


// Enumerations
typedef enum { TXB0 = 1u, TXB1 = 2u, TXB2 = 4u } txbuf_t;
typedef enum { RXB0, RXB1 } rxbuf_t;
typedef enum { RX_MASK0, RX_MASK1 } rx_mask_t;
typedef enum { RX_FILT0, RX_FILT1, RX_FILT2, RX_FILT3, RX_FILT4, RX_FILT5 } rx_filt_t;
typedef enum { MCP2515_OPTION_ROLLOVER } mcp_2515_options_t;
//typedef enum { RESET, READ, READ_RX_BUFFER, WRITE, LOAD_TX_BUFFER, RTS, READ_STATUS, RX_STATUS, BIT_MODIFY } mcp2515_commands_t;


// Structures
// Below is a really nice useful comment from mcp2515.c from the spirilis repo
/* 
 *  CAN message ID chart
 *  +-----------------------------------------------------------------------------------------------+
 *  |31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0|
 *  +-----------------------------------------------------------------------------------------------+
 *    8  4  2  1  8  4  2  1  8  4  2  1  8  4  2  1  8  4  2  1  8  4  2  1  8  4  2  1  8  4  2  1
 *  
 *                                                                 +--------------------------------+
 *                                                                 | CAN Standard ID                |  < CAN 2.0B Standard Message
 *                                                                 +--------------------------------+
 *                                                                 | SIDH                  | SIDL   |  < MCP2515 Register Assignments
 *                                                                 +--------------------------------+
 *
 *           +--------------------------------+-----------------------------------------------------+
 *           | CAN Standard ID                | CAN Extended ID                                     |  < CAN 2.0B Extended Message
 *           +-----------------------+--------+-----+-----------------------+-----------------------+
 *           | SIDH                  | SIDL   | SIDL| EID8                  | EID0                  |  < MCP2515 Register Assignments
 *           +-----------------------+--------+-----+-----------------------+-----------------------+
 *
 */
typedef struct {
    uint16_t sid;   // bits 10:0 --> 0x000 to 0x7FF
    uint8_t exide;    // 0 = standard, 1 = extended
    uint32_t eid;   // bits 17:0 --> 0x00000 to 0x3FFFF
    uint8_t rtr;    // 0 = data frame, 1 = remote frame (for J1939, rtr always = 0)
} can_msg_arb_field;

typedef struct {
    uint8_t dlc;    // 4-bit to represent # of messages --> above 8 and this will need more logic...
} can_msg_ctrl_field;

typedef struct {
    uint8_t data0;
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint8_t data4;
    uint8_t data5;
    uint8_t data6;
    uint8_t data7;
} can_msg_data_field;   // Can also just use a uint8_t array of length 8

typedef struct {
    can_msg_arb_field arb_field;
    can_msg_ctrl_field ctrl_field;
    can_msg_data_field data_field;
} can_msg;


// Functions
void can_init_default(void); //
void can_set_baud_rate(uint32_t baudrate, uint8_t propsec, uint8_t syncjump);

opmode_t mcp2515_current_opmode(void); //
void mcp2515_config_mode(void); //
void mcp2515_normal_mode(void); // 
void mcp2515_cmd_reset(void); //
uint8_t mcp2515_cmd_read_status(void); //
uint8_t mcp2515_cmd_rx_status(void); //
void mcp2515_cmd_read(uint8_t reg_address, uint8_t * buf); //
void mcp2515_cmd_read_sequential(uint8_t start_reg_addr, uint8_t * rxbuf, uint8_t len); //
void mcp2515_cmd_write(uint8_t reg_address, uint8_t val); //
void mcp2515_cmd_write_sequential(uint8_t start_reg_addr, uint8_t * txbuf, uint8_t len); //
void mcp2515_cmd_write_bit(uint8_t reg_address, uint8_t mask, uint8_t val); //
void mcp2515_cmd_read_rx_buf(rxbuf_t rxb, uint8_t * rx_buf); //
void mcp2515_cmd_load_tx_buf(txbuf_t txb, uint8_t * tx_buf); //
void mcp2515_cmd_rts(txbuf_t txb); //

/* Use the below functions at border between PIC and MCP2515. As in to say...
 *      - For a transmit, you may construct the message first using the structs
 *        defined above, which are more intuitive to work with, and then translate
 *        those structs into uint32_t id and data bytes to be placed into uint8_t * tx_buf
 * 
 *      - For a receive, you get the message into the uint8_t * rx_buf, translate
 *        that into structs to work with internally more intuitively.
 * This is what the can_compose_msg_xx and can_parse_msg_xx functions below are for
 * --> the translating to and from structs.
 */
void can_compose_msg_std(can_msg * msg, uint8_t * mcp2515_tx_buf);
void can_parse_msg_std(can_msg * msg, uint8_t * mcp2515_rx_buf);
void can_compose_msg_ext(can_msg * msg, uint8_t * mcp2515_tx_buf);
void can_parse_msg_ext(can_msg * msg, uint8_t * mcp2515_rx_buf);

// This will be the highest level API layer
uint8_t can_send(can_msg * msg);    // TODO: Include priority at some point...
// The following macro constants are what will be returned by can_receive...
#define MCP2515_RECEIVED_BUF0       0x1u
#define MCP2515_RECEIVED_BUF1       0x2u
#define MCP2515_NOTHING_RECEIVED    0x0u
uint8_t can_receive(can_msg * msg_buf0, can_msg * msg_buf1);
//uint8_t can_remote_frame(can_msg_arb_field arb_field);
//uint8_t can_tx_cancel(void);
//uint8_t can_tx_available(void);
//uint8_t can_rx_pending(void);
//uint8_t can_rx_setmask(rx_mask_t mask_id, uint32_t mask, uint8_t is_extended);
//uint8_t can_rx_setfilter(rx_filt_t filt_id, uint32_t filter);
//uint8_t can_rx_mode(void);
//uint8_t can_mcp2515_config_options(mcp_2515_options_t option, uint8_t val);
//uint8_t can_read_error(uint8_t reg);
//uint8_t can_clear_bus_error(void);

#endif	/* MCP2515_HEADER */

