/*
 * File:   mssp_spi_driver.c
 * Author: Abdullah Almosalami
 *
 * Created on October 24, 2021, 1:10 AM
 */


#include <xc.h>
#include "mssp_spi.h"
#include "lcd_driver.h"


// Global variables relevant to SPI
uint8_t receive_byte = 0x00;    // Used by ISR when SPI mode is slave and receive has occurred
static enum spi_actor_t spi_actor = SPI_MASTER;
uint8_t spi_transfer_complete_flag = 0x00;
uint8_t manual_transfer = 0x00; // Flag to indicate when we are manually (i.e., using the functions below) transferring data; may be used in ISR or debugging


/* Function: SPI_Init_Master_Default
 * ---------------------------------
 * SPI Master mode 0,0 (CKP=0, CKE=1) and no interrupts enabled
 * because I will just poll SSPIF bit to indicate when byte transfer
 * op is done. The SS pin is specified in the SPI_MASTER or MASTER_CS macros!
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void SPI_Init_Master_Default(void){
    // First disable MSSP module to configure
    SPI_DISABLE;
    
    // Set mode to Master (0,0) sample at midpoint /w SCK = FOSC/16
    SSPCON1 = SSPCON1_SSPM_MASTER_FOSC16 | SSPCON1_CKP_LOW;
    SSPSTAT = SSPSTAT_CKE_TX_ACTIVE_IDLE | SSPSTAT_SMP_MID;
    
    // Set I/O pins for SPI Master
    TRIS_SPI_MASTER;
    
    // Enable module and reset all flags
    SPI_ENABLE;
    CLEAR_WCOL_FLAG;
    CLEAR_SSPOV_FLAG;
    CLEAR_BF_FLAG;     
    CLEAR_MSSP_IFLAG;
    
    // Make sure CS line is HIGH (inactive)
    SPI_MASTER_CS_HIGH;
    
    spi_actor = SPI_MASTER;  // Set flag to master mode
            
    // Enable SSPIF interrupt
    MSSP_ENABLE_INTERRUPT;
    ENABLE_PERIPHERAL_INTERRUPTS;
    
}

/* Function: SPI_Init_Slave_Default
 * ---------------------------------
 * SPI Slave mode 0,0 (CKP=0, CKE=1) with SSPIF interrupt enabled as
 * part of receiving packages. The SS pin is specified in the SPI_MASTER
 * or MASTER_CS macros!
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void SPI_Init_Slave_Default(void){
    // First disable MSSP module to configure
    SPI_DISABLE;
    
    // SSPCON1 config --> CKP bit and the SSPM bits --> I will only allow use with the SS pin for now (RA5)
    SSPCON1 |= SSPCON1_CKP_LOW | SSPCON1_SSPM_SLAVE_SS_EN;
    // SSPSTAT config --> CKE bit and SMP bit --> SMP bit must be cleared (i.e., sample at midpoint)
    SSPSTAT |= SSPSTAT_CKE_TX_ACTIVE_IDLE | SSPSTAT_SMP_SLAVE;

    // Set I/O pins for SPI Slave
    TRIS_SPI_SLAVE;
    
    // Enable module and reset all flags
    SPI_ENABLE;
    CLEAR_WCOL_FLAG;
    CLEAR_SSPOV_FLAG;
    CLEAR_BF_FLAG;     
    CLEAR_MSSP_IFLAG;
    spi_actor = SPI_SLAVE;  // Set the slave_mode global variable flag
    
    // Enable SSPIF interrupt
    MSSP_ENABLE_INTERRUPT;
    ENABLE_PERIPHERAL_INTERRUPTS;
}

/* Function: SPI_Init
 * ---------------------------------
 * This function is a generalized initialize function for which the
 * user will specify the required configuration information. The SS
 * pin is specified in the SPI_MASTER or MASTER_CS macros!
 * 
 * Parameters:
 *      . clock_pol - Indicates the idle state of the clock, 0=LOW, 1=HIGH
 *      . clock_tx_pha - Indicates when data is transmitted - on idle-to-active edge (0) or active-to-idle edge(1)
 *      . smp_bit - Indicates when data is sampled - in the middle of a clock cycle (0) or at the end of the clock (1)
 *                  MUST BE CLEARED FOR SLAVE MODE.
 *      . fosc_div - Should either be 4, 16, or 24 and indicates how much the system clock is divided by to give SCK
 *      . slave0_or_mater1 - Indicates whether we are initializing in slave mode (0) or master mode (1)
 * 
 * Returns: none
 */
uint8_t SPI_Init(uint8_t clock_pol, uint8_t clock_tx_pha, uint8_t smp_bit, uint8_t fosc_div, enum spi_actor_t spi_actor_type){
    // First disable MSSP module to configure
    SPI_DISABLE;
    SSPCON1 = 0x00;
    SSPSTAT = 0x00;
    
    if(spi_actor_type == SPI_MASTER){
        // Master mode
        
        // SSPCON1 config --> CKP bit and the SSPM bits
        SSPCON1 |= (clock_pol << SSPCON1_CKP_BIT_LOC);
        switch(fosc_div){
            case 4u:
                SSPCON1 |= SSPCON1_SSPM_MASTER_FOSC4;
                break;
                
            case 16u:
                SSPCON1 |= SSPCON1_SSPM_MASTER_FOSC16;
                break;
                
            case 64u:
                SSPCON1 |= SSPCON1_SSPM_MASTER_FOSC64;
                break;
                
            default:
                return EXEC_FAIL;   // Later on, might make specific return code for "Failing to set SPI FOSC"
        }
        
        // SSPSTAT config --> CKE bit and SMP bit
        SSPSTAT |= (clock_tx_pha << SSPSTAT_CKE_BIT_LOC) | (smp_bit << SSPSTAT_SMP_BIT_LOC);
    
        // Set I/O pins for SPI Master
        TRIS_SPI_MASTER;
        
        // Make sure CS line is HIGH (inactive)
        SPI_MASTER_CS_HIGH;
        
        spi_actor = SPI_MASTER;
        
    } else {
        // Slave mode 
        
        // SSPCON1 config --> CKP bit and the SSPM bits --> I will only allow use with the SS pin for now (RA5)
        SSPCON1 |= (clock_pol << SSPCON1_CKP_BIT_LOC) | SSPCON1_SSPM_SLAVE_SS_EN;
        // SSPSTAT config --> CKE bit and SMP bit --> SMP bit must be cleared (i.e., sample at midpoint)
        SSPSTAT |= (clock_tx_pha << SSPSTAT_CKE_BIT_LOC) | SSPSTAT_SMP_SLAVE;
        
        // Set I/O pins for SPI Slave
        TRIS_SPI_SLAVE;
        
        spi_actor = SPI_SLAVE;
    }
    
    
    // Enable module and reset all flags
    SPI_ENABLE;
    CLEAR_WCOL_FLAG;
    CLEAR_SSPOV_FLAG;
    CLEAR_BF_FLAG;     
    CLEAR_MSSP_IFLAG;
    
    // Enable SSPIF interrupt
    MSSP_ENABLE_INTERRUPT;
    ENABLE_PERIPHERAL_INTERRUPTS;
    
    return EXEC_SUCCESS;
}

/* Function: SPI_Disable
 * ---------------------------------
 * Disables the MSSP module, which disables SPI.
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void SPI_Disable(void){
    SPI_DISABLE;
}


// COMMUNICATION
// Use the xx_Transfer functions for duplex communication
// Use xx_Send or xx_Receive for just one-way communication where you don't care what goes the other way

/* Function: SPI_Transfer_Byte
 * ---------------------------------
 * Sends a byte tx over the SDO line and also takes whatever is received
 * over SDI and puts it into the rx parameter.
 * 
 * Parameters:
 *      . tx - The byte to be sent.
 *      . rx - The buffer to hold the received byte.
 * 
 * Returns: none
 */
void SPI_Transfer_Byte(uint8_t tx, uint8_t * rx){
    manual_transfer = 0x01; // Indicate the manual transfer is beginning.
    spi_transfer_complete_flag = 0x00;
    
    if(spi_actor == SPI_MASTER) SPI_MASTER_CS_LOW;  // If in Master Mode...
    SSPBUF = tx;
    while(!spi_transfer_complete_flag);     // Wait until data ready variable flag is set
//    while(!PIR1bits.SSPIF)
    if(spi_actor == SPI_MASTER) SPI_MASTER_CS_HIGH;  // If in Master Mode...MASTER_CS_HIGH;    
    *rx = SSPBUF;
    
    manual_transfer = 0x00; // Indicate the manual transfer is ending.
    spi_transfer_complete_flag = 0x00;  // Reset transfer complete flag
    
    return;
}

/* Function: SPI_Transfer_Packet
 * ---------------------------------
 * Uses SPI_Transfer_Byte to send 1 or more bytes over SDO as well
 * as receive 1 or more bytes into the rx_data buffer parameter.
 * 
 * Parameters:
 *      . tx_pack - The packet to be sent.
 *      . rx_pack - The buffer to hold the received packet.
 *      . pack_size - The size of the packet to be sent/received.
 * 
 * Returns: none
 */
void SPI_Transfer_Packet(uint8_t * tx_pack, uint8_t * rx_pack, uint16_t pack_size){

    for(uint16_t i=0; i<pack_size; i++){
        SPI_Transfer_Byte(tx_pack[i], &rx_pack[i]);
    }
}

/* Function: SPI_Send_Byte
 * ---------------------------------
 * Sends a byte tx over the SDO line without caring about any received byte.
 * 
 * Parameters:
 *      . tx - The byte to be sent
 * 
 * Returns: none
 */
void SPI_Send_Byte(uint8_t tx){
    manual_transfer = 0x01; // Indicate the manual transfer is beginning.
    spi_transfer_complete_flag = 0x00;
    
    if(spi_actor == SPI_MASTER) SPI_MASTER_CS_LOW;  // If in Master Mode...
    SSPBUF = tx;
    while(!spi_transfer_complete_flag);     // Wait until data ready variable flag is set
    if(spi_actor == SPI_MASTER) SPI_MASTER_CS_HIGH;  // If in Master Mode...
    spi_transfer_complete_flag = 0x00;  // Reset transfer_complete_flag
    
    manual_transfer = 0x00; // Indicate the manual transfer is ending.
    spi_transfer_complete_flag = 0x00;
}

/* Function: SPI_Send_Packet
 * ---------------------------------
 * Sends a packet over SDO without caring about any received packet.
 * 
 * Parameters:
 *      . tx_pack - The byte to be sent
 *      . tx_size - The buffer to hold the received byte
 * 
 * Returns: none
 */
void SPI_Send_Packet(uint8_t * tx_pack, uint16_t tx_size){
    uint16_t i = 0;
    for(i=0; i<tx_size; i++){
        SPI_Send_Byte(tx_pack[i]);
    }
}

/* Function: SPI_Receive_Byte
 * ---------------------------------
 * Receives a byte over SDI without caring about any transmitted byte.
 * 
 * Parameters:
 *      . rx - The buffer to hold the received byte
 * 
 * Returns: none
 */
void SPI_Receive_Byte(uint8_t * rx){
    
    SPI_Transfer_Byte(0x00, rx);
    
}

/* Function: SPI_Receive_Packet
 * ---------------------------------
 * Receives a packet over SDI without caring about any transmitted packet.
 * 
 * Parameters:
 *      . rx_pack - The buffer to hold the received packet.
 *      . rx_size - The size of the receive buffer.
 * 
 * Returns: none
 */
void SPI_Receive_Packet(uint8_t * rx_pack, uint16_t rx_size){
    uint16_t i = 0;
    for(i=0; i<rx_size; i++){
        SPI_Receive_Byte(&rx_pack[i]);
    }
}

/* TODO
 * Write code to check the SSPOV bit when in slave mode
 */


// Functions that do not automatically enable/disable CS, as described in header

/* Function: SPI_Transfer_Byte_without_CS
 * ---------------------------------
 * Same as SPI_Transfer_Byte except without enabling/disabling the CS line.
 * That needs to be done before and after this function gets called. This
 * should allow the application to more easily make use of sequential reading
 * from SPI devices.
 * 
 * Parameters:
 *      . tx - The byte to be sent.
 *      . rx - The buffer to hold the received byte.
 * 
 * Returns: none
 */
void SPI_Transfer_Byte_without_CS(uint8_t tx, uint8_t * rx){
    
    manual_transfer = 0x01; // Indicate the manual transfer is beginning.
    spi_transfer_complete_flag = 0x00;
    
    SSPBUF = tx;
    while(!spi_transfer_complete_flag);     // Wait until data ready variable flag is set  
    *rx = SSPBUF;
    
    manual_transfer = 0x00; // Indicate the manual transfer is ending.
    spi_transfer_complete_flag = 0x00;  // Reset transfer complete flag
    
}

/* Function: SPI_Transfer_Packet_without_CS
 * ---------------------------------
 * Same as SPI_Transfer_Packet except without enabling/disabling the CS line.
 * That needs to be done before and after this function gets called. This
 * should allow the application to more easily make use of sequential reading
 * from SPI devices.
 * 
 * Parameters:
 *      . tx - The byte to be sent.
 *      . rx - The buffer to hold the received byte.
 * 
 * Returns: none
 */
void SPI_Transfer_Packet_without_CS(uint8_t * tx_pack, uint8_t * rx_pack, uint16_t pack_size){
    
    for(uint16_t i=0; i<pack_size; i++){
        SPI_Transfer_Byte(tx_pack[i], &rx_pack[i]);
    }
    
}



//// Debug pin
//    TRISDbits.RD3 = 0u;
//    LATDbits.LATD3 = 0u;
//    
//    di();
//    SPI_Init_Master_Default();
//    Timer1_Init_Default(DEFAULT_CONFIG_PERIOD_50ms);
//    Timer1_Enable();
//    ENABLE_PERIPHERAL_INTERRUPTS;
//    ei();
//    
//    // Test message
//    uint8_t tx_data[14] = {0xA2, 0x27, 0xFE, 0x11, 0x3C, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x21, 0xAE, 0xF1};
//    uint8_t rx_data[14];
//    uint16_t message_size = 14U;
//    
//    while(1){
//        
////        SPI_Write(0xA2, &RX_result);
//        SPI_Transfer_Packet(tx_data, rx_data, message_size);
//
//        __delay_ms(500);
//    }