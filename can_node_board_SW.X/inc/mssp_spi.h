/* 
 * File: mssp_spi_driver.h
 * Author: Abdullah Almosalami
 * Comments: This is meant to set up the uC as either a master slave in SPI communication using the MSSP module within the PIC.
 * Revision history: 0.1
 */

/* There are four registers specific to SPI operation:
 *      - SSPCON1 - Control Register
 *      - SSPSTAT - Status Register
 *      - SSPBUF - Tx/Rx Buffer
 *      - SSPSR - Shift Register for the SDI and SDO lines (not directly accessible)
 * SSPCON1 is R/W while only the upper two bits of SSPSTAT are R/W.
 *      - In Rx operation, the SSPSR and SSPBUF form a double-buffered receiver,
 *        which allows the SSPBUF to be read in before the 8 clocks fill up SSPSR.
 *        Once SSPSR is filled, it is immediately copied into SSPBUF and the BF
 *        flag is set in SSPSTAT along with SSPIF.
 *      - In Tx operation, the SSPBUF is NOT double-buffered, and a write to SSPBUF
 *        will also write to SSPSR.
 * 
 * Furthermore, for SPI modes, the CKP bit (clock polarity at idle) in SSPCON1[4] and the CKE bit (clock phase) in SSPSTAT[6] should be:
 *      - SPI Mode (0,0) -- clock idle LOW, data sampled on rising edge -- CKP = 0, CKE = 1
 *      - SPI Mode (0,1) -- clock idle LOW, data sampled on falling edge -- CKP = 0, CKE = 0
 *      - SPI Mode (1,0) -- clock idle HIGH, data sampled on falling edge -- CKP = 1, CKE = 1
 *      - SPI Mode (1,1) -- clock idle HIGH, data sampled on rising edge -- CKP = 1, CKE = 0
 * NOTE that (x,y) don't match (CKP,CKE)! CKP does = x but CKE is opposite of y. This is because y typically denotes the edge that
 * data is sampled on (0 = idle --> active, 1 = active --> idle) whereas CKE denotes the edge that data is TRANSMITTED on.
 * 
 * NOTE! In master mode, as stated in section "17.3.5 MASTER MODE", "... data is transmitted/received as
 * soon as the SSPBUF register is written to." So to start any transfer, just write to SSPBUF. Even if
 * you just intend to receive, just write to it nonetheless.
 * 
 */

 
#ifndef MSSP_SPI_HEADER
#define	MSSP_SPI_HEADER

#include <xc.h>
#include <stdint.h>


#ifndef _XTAL_FREQ
#define _XTAL_FREQ                  40000000U   // 40MHz --> Defining this will allow use of __delay_ms())
#endif

// The following are used for return values of some functions as a means of indicating whether or not they executed successfully
#ifndef EXEC_SUCCESS
#define EXEC_SUCCESS    1u
#define EXEC_FAIL       0u
#endif

// <editor-fold defaultstate="collapsed" desc="DEFINITIONS">
// Definitions
// NOTE: The xx_xx_BIT macros are for holding a constant with a 1 at the location of the bit
//       The xx_xx_BIT_LOC macros are for what you'd use with the left shift << to get to the correct bit location
// SSPCON1 Register
#define SSPCON1_WCOL_BIT            0x80        // (MUST BE CLEARED IN SOFTWARE) Write Collision Detect bit --> 1 = SSPxBUF register is written while it is still transmitting, 0 = no collision
#define SSPCON1_SSPOV_BIT           0x40        // (MUST BE CLEARED IN SOFTWARE) Receive Overflow bit --> 1 = In slave mode, if new byte is received while the SSPBUF is still holding previous data
#define SSPCON1_SSPEN_BIT           0x20        // MSSP Enable bit
#define SSPCON1_CKP_BIT             0x10        // Clock Polarity bit
#define SSPCON1_CKP_BIT_LOC         4u          // bit 4 of the register
#define SSPCON1_SSPM_BITFIELD       0x0F        // MSSP Mode Select --> 0=SPI Master FOSC/4; 1=SPI Master FOSC/16, 2=SPI Master FOSC/64; 3=SPI Master TMR2/2; 4=SPI Slave SS pin enabled; 5=SPI Slave SS pin disabled, I/O

#define SSPCON1_SSPM_MASTER_FOSC4   0x00        // SPI Master mode, clock = FOSC/4 --> SSPM = 0b0000 --> This should result in 10MHz SCK
#define SSPCON1_SSPM_MASTER_FOSC16  0x01        // SPI Master mode, clock = FOSC/16 --> SSPM = 0b0001 --> This should result in 2.5MHz SCK
#define SSPCON1_SSPM_MASTER_FOSC64  0x02        // SPI Master mode, clock = FOSC/64 --> SSPM = 0b0010 --> This should result in 625kHz SCK
#define SSPCON1_SSPM_SLAVE_SS_EN    0x04        // Slave mode with ~SS pin RA5 used
#define SSPCON1_CKP_LOW             0x00        // Clock polarity = LOW at idle
#define SSPCON1_CKP_HIGH            0x10        // Clock polarity = HIGH at idle
#define SPI_ENABLE                  SSPCON1bits.SSPEN = 1
#define SPI_DISABLE                 SSPCON1bits.SSPEN = 0
#define CLEAR_WCOL_FLAG             SSPCON1bits.WCOL = 0
#define CLEAR_SSPOV_FLAG            SSPCON1bits.SSPOV = 0

// SSPSTAT Register
#define SSPSTAT_SMP_BIT             0x80        // 0 = Input sampled in middle of clock cycle, 1 = Input sampled at end of clock cycle
#define SSPSTAT_SMP_BIT_LOC         7u          // bit7 of the register
#define SSPSTAT_CKE_BIT             0x40        // 0 = Tx on idle to active clock edge, 1 = Tx on active to idle clock edge
#define SSPSTAT_CKE_BIT_LOC         6u          // bit6 of the register
#define SSPSTAT_BF_BIT              0x01        // 0 = Rx not complete or SSPBUF is empty, 1 = Rx complete or SSPBUF is full

#define SSPSTAT_CKE_TX_IDLE_ACTIVE  0x00        // Clock select bit so that Tx on idle to active
#define SSPSTAT_CKE_TX_ACTIVE_IDLE  0x40        // Clock select bit so that Tx on active to idle
#define SSPSTAT_SMP_MID             0x00        // Sample occurs at middle of data output time
#define SSPSTAT_SMP_END             0x80        // Sample occurs at emd of data output time
#define SSPSTAT_SMP_SLAVE           SSPSTAT_SMP_MID     // Slave must sampled at midpoint of cycle
#define CLEAR_BF_FLAG               SSPBUF      // This causes the SSPBUF to be read which clears the BF flag in SSPSTAT to allow for next Tx/Rx

// I/O for MSSP SPI
#define ADCON_SPI_MASTER            // There are no analog input channels on PORTC pins for the PIC18F4620 package
#define ADCON_SPI_SLAVE             // There are also no analog input channels for any of the SPI slave pins (RA5 has not analog channel multiplexed)
#define TRIS_SPI_MASTER             TRISC |= 0x10; TRISC &= ~(0x28); TRISDbits.RD2 = 0;  // This is what TRISC needs to be set to. RC5 = SDO and RC3 = SCK outputs, RC4 = SDI input (RC4 would be automatically config by MSSP)
#define TRIS_SPI_SLAVE              TRISC |= 0x18; TRISC &= ~(0x20); TRISA |= 0x20       // Same as TRIS_MASTER except SCK (RC3) is input and CS line is RA5
#define MASTER_CS                   LATDbits.LATD2     // Chip Select line is in Master Mode (can use other I/O too)
#define SPI_MASTER_CS_LOW               MASTER_CS = 0      // Activate the CS line
#define SPI_MASTER_CS_HIGH              MASTER_CS = 1      // De-Activate the CS line

// MSSP Interrupt-Related
#define MSSP_IF_BIT                 PIR1bits.SSPIF
#define MSSP_INT_ENABLE_BIT         PIE1bits.SSPIE
#define MSSP_ENABLE_INTERRUPT       PIE1bits.SSPIE = 1
#define ENABLE_BCLIE_INTERRUPT      PIE2bits.BCLIE = 1
#define CLEAR_MSSP_IFLAG            PIR1bits.SSPIF = 0      // Clear interrupt flag for MSSP
#define CLEAR_BCLIE_IFLAG           PIR2bits.BCLIF = 0;
// </editor-fold>


enum spi_actor_t { SPI_MASTER, SPI_SLAVE };
enum spi_mode_t { SPI_MODE_00, SPI_MODE_01, SPI_MODE_10, SPI_MODE_11 };   // Standard (x,y) terminology, where x is clock polarity and y is what edge is for reading data (phase)


// Function declarations
void SPI_Init_Master_Default(void);     // I'll default to SPI 0,0,  mid-SMP, FOSC/16, interrupt from SSPIF enabled to indicate when transmit has completed
void SPI_Init_Slave_Default(void);      // I'll default to SPI 0,0, mid-SMP, SS pin as RA5, interrupt from SSPIF enabled to indicate receive
uint8_t SPI_Init(uint8_t clock_pol, uint8_t clock_tx_pha, uint8_t smp_bit, uint8_t fosc_div, enum spi_actor_t spi_actor_type);

void SPI_Disable(void);

void SPI_Transfer_Byte(uint8_t tx, uint8_t * rx);
void SPI_Transfer_Packet(uint8_t * tx_pack, uint8_t * rx_pack, uint16_t pack_size);
void SPI_Send_Byte(uint8_t tx);
void SPI_Send_Packet(uint8_t * tx_pack, uint16_t tx_size);
void SPI_Receive_Byte(uint8_t * rx);
void SPI_Receive_Packet(uint8_t * rx_pack, uint16_t rx_size);

/* The following are functions that do not automatically enable/disable the CS
 * line, which would make it easier for an application to successively clock
 * in cycles and control the CS line separately instead of /w each byte.
 */
void SPI_Transfer_Byte_without_CS(uint8_t tx, uint8_t * rx);
void SPI_Transfer_Packet_without_CS(uint8_t * tx_pack, uint8_t * rx_pack, uint16_t pack_size);


#endif	/* MSSP_DRIVER_HEADER */

