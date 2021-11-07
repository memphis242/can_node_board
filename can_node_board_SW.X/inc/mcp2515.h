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


// <editor-fold defaultstate="collapsed" desc="DEFINITIONS - Registers and Macros">

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
#define MCP2515_CANSTAT_ICOD_BITS       0x0E    // ICOD = "Interrupt CODe"


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


/* ****************************************************************************
 * Receive-Related
 * ****************************************************************************
 * 
 * 
*/

// Filters
// Filter 0
#define MCP2515_RXF0SIDH    0x00
#define MCP2515_RXF0SIDL    0x01
#define MCP2515_RXF0EID8    0x02
#define MCP2515_RXF0EID0    0x03



// </editor-fold>


// Enumerations
typedef enum { TXB0, TXB1, TXB2 } txbuf_t;
typedef enum { RXB0, RXB1 } rxbuf_t;
typedef enum { RX_MASK0, RX_MASK1 } rx_mask_t;
typedef enum { RX_FILT0, RX_FILT1, RX_FILT2, RX_FILT3, RX_FILT4, RX_FILT5 } rx_filt_t;
typedef enum { MCP2515_OPTION_ROLLOVER } mcp_2515_options_t;


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
    uint16_t sid;   // bits 10:0 --> 0x000 to 0x3FF
    uint8_t ide;    // 0 = standard, 1 = extended
    uint32_t eid;   // bits 17:0 --> 0x00000 to 0x1FFFF
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
} can_msg_data_field;   // Can also just use array

typedef struct {
    can_msg_arb_field arb_field;
    can_msg_ctrl_field ctrl_field;
    can_msg_data_field data_field;
} can_msg;


// Functions
void can_spi_command(uint8_t cmd);
uint8_t can_spi_query(uint8_t query);
void can_read_reg(uint8_t reg, uint8_t * rxbuf);
void can_read_successive_reg(uint8_t start_reg, uint8_t * rxbuf, uint8_t len);
void can_write_reg(uint8_t reg, uint8_t value);
void can_write_successive_reg(uint8_t start_reg, uint8_t * txbuf, uint8_t len);
void can_write_bit(uint8_t reg, uint8_t mask, uint8_t val);
/* Use the below functions at border between PIC and MCP2515. As in to say...
 *      - For a transmit, you may construct the message first using the structs
 *        defined above, which are more intuitive to work with, and then translate
 *        those structs into uint32_t id and data bytes to be placed into uint8_t * txb_buf
 * 
 *      - For a receive, you get the message into the uint8_t * rxb_buf, translate
 *        that into structs to work with internally more intuitively.
 * This is what the can_compose_msg_xx and can_parse_msg_xx functions below are for
 * --> the translating to and from structs.
 */
void can_write_txbuf(txbuf_t txb,  uint8_t * mcp2515_tx_buf, uint8_t len);
void can_read_rxbuf(rxbuf_t rxb,  uint8_t * mcp2515_rx_buf, uint8_t len);

void can_init(void);
void can_set_baud_rate(uint32_t baudrate, uint8_t propsec, uint8_t syncjump);
void can_compose_msg_std(can_msg * msg, uint8_t * mcp2515_tx_buf);
void can_parse_msg_std(can_msg * msg, uint8_t * mcp2515_rx_buf);
void can_compose_msg_ext(can_msg * msg, uint8_t * mcp2515_tx_buf);
void can_parse_msg_ext(can_msg * msg, uint8_t * mcp2515_rx_buf);

uint8_t can_send(can_msg * msg);    // TODO: Include priority at some point...
uint8_t can_remot_frame(can_msg_arb_field arb_field);
uint8_t can_tx_cancel(void);
uint8_t can_tx_available(void);
uint8_t can_receive(can_msg * msg);
uint8_t can_rx_pending(void);
uint8_t can_rx_setmask(rx_mask_t mask_id, uint32_t mask, uint8_t is_extended);
uint8_t can_rx_setfilter(rx_filt_t filt_id, uint32_t filter);
uint8_t can_rx_mode(void);
uint8_t can_mcp2515_config_options(mcp_2515_options_t option, uint8_t val);
uint8_t can_read_error(uint8_t reg);
uint8_t can_clear_bus_error(void);

#endif	/* MCP2515_HEADER */

