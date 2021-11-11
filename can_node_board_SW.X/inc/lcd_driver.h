/* 
 * File: lcd_driver.h
 * Author: Abdullah Almosalami
 * Comments: This is meant for the popular 1602A 16x2 LCD Board that contains
 *           the HITACHI HD44780U Driver. Also, PORTD is used for the data
 *           lines, and RC0, RC1, and RC2 for RS, R/W, and E respectively.
 * 
 * NOTE: As I have come to discover, even displays claiming to utilize the
 *       HITACHI HD44780U Driver will have differences in their initilization
 *       sequence and the amount of time you wait between different operations.
 *       As annoying as this is, it must be dealt with and so the code is meant
 *       to reflect several possible displays I might use...
 * 
 * Revision: 0.1
 * 
 * TODO:
 *      1- Need to write functions for sending multiple bytes
 *      2- Need to find appropriate initialization sequence for other LCDs
 *      3- Need to write function that takes in an integer and converts that to 
 *        needed string value to display
 *      4- Same as the previous point but with a double
 *      5- Initialization routines should probably return an EXEC_SUCCESS or
 *         EXEC_FAIL. Set that!
 */

/* Pin descriptions for 1602A Board
 * Control Pins:
 * RS   -   Selects registers --> 0: Instruction (write), Busy Flag + Address Counter (read); 1: Data register (for r/w)
 * R/~W -   Selects read or write --> 0: Write; 1: Read
 * E    -   Enable pin
 * 
 * Data Pins:
 * DB4 to DB7   -   Bidirectional tristate data bus pins. Used for data transfer. DB7 is used as Busy flag. Are used in 4-bit operation.
 * DB0 to DB3   -   Bidirectional tristate data bus pins. Used for data transfer. NOT used during 4-bit operation.
 * 
 * Other Pins:
 * Vdd/Vss  -   Power/GND
 * VO       -   Contrast adjust pin -- Place 10k pot for adjustable contrast -- 10k: Min contrast, 0k: Max contrast
 * A/K      -   Anode/Cathode for LCD backlight
 */

/* 1602A Board Description
 * Overview:
 * The main driver IC on the board is the HITACHI HD44780U, which also pairs /w another driver IC for 16x2 character operation.
 * There are two 8-bit registers: an instruction register (IR) and a data register (DR).
 *      - The IR stores instruction codes and address info for the data RAM (DDRAM) and character generator RAM (CGRAM).
 *      - The DR stores data to be written into/read out of DDRAM or CGRAM.
 * There is a Busy Flag (BF). When it is 1, the HDD44780U is in internal operation mode and the next instruction will not be accepted. When
 * RS = 0 and R/W = 1, BF is output onto DB7. The next instruction must be written only after ensuring BF = 0.
 * There is also an Address Counter (AC) the is used by the MPU for looking into DDRAM and CGRAM. When an address in an instruction is sent
 * into an IR, that address is copied into the AC, and after a read/write, the AC is automatically incremented by 1 or decremented by 1,
 * and the AC contents are output to DB0 - DB6 whenever RS=0 and R/W = 1.
 * 
 * DDRAM, CGROM, and CGRAM:
 * DDRAM is 80 x 8 bits, or 80 bytes (characters). The parts of DDRAM not used for the display can be used as general-purpose RAM.
 * Since the LCD display on the board is 16x2, that's 32 characters. If shifting operation is not used (i.e., scrolling through characters
 * to show more than 16x2), then DDRAM address 0x00 to 0x0F are for line 1, 0x40 to 0x4F are for line 2.
 * CGROM is used for making the dots to actually display a given character on the LCD. It can be for 5x8 or 5x10 dots per character,
 * depending on the display. It can do 208 5x8 dot character patterns or 32 5x10 ones. You can also define more if you'd like using CGRAM.
 * Refer to the HD44780 datasheet for more information.
 * 
 * Interface to uC:
 * It is possible to use the HD44780U in 4-bit or 8-bit mode - i.e., you can send data in two 4-bit operations or one 8-bit operation.
 * On power-reset, the HD44780U executes instructions internally to set the default configuration as follows:
 *      - Display clear
 *      - Function set:             DL = 1 (8-bit); N = 0 (1-line); F = 0 (5x8 character font)
 *      - Display on/off control:   D = 0 (display off); C = 0 (cursor off); B = 0 (blinking off)
 *      - Entry mode set:           I/D = 1 (increment by 1); S = 0 (no shift)
 * During this time, the BF is set. This takes about 10ms after Vdd rises to 4.5V.
 * There are four categories of instructions:
 *      1- Designate HD44780U functions, such as display format, data length, etc.
 *      2- Set internal RAM addresses
 *      3- Perform data transfer to internal RAM
 *      4- Perform miscellaneous functions
 * It is best practice to poll the BF (i.e., DB7 when RS = 0 and R/W = 1) to ensure an instruction has been executed.
 * 
 * Other:
 * The cursor is always positioned at wherever the AC pointed to in DDRAM. Recall the AC is always automatically incremented/decremented
 * with a write or read into DDRAM.
 */


// To ensure file isn't included more than once 
#ifndef LCD_DRIVER_HEADER
#define	LCD_DRIVER_HEADER

#ifndef _XTAL_FREQ
// 40MHz --> Defining this will allow use of __delay_ms()
/* The implementation of the initialization routines as well as ennable toggle
 * and writing/reading from the display makes heavy use of __delay_ms() and
 *  __delay_us()
 */
#define _XTAL_FREQ  40000000u
#endif


// Includes
#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

// <editor-fold defaultstate="collapsed" desc="DEFINITIONS">
// Definitions
//Pin Setups
#define DISP_RS                     LATCbits.LATC0
#define DISP_RW                     LATCbits.LATC1
//#define DISP_E                      LATCbits.LATC2
#define DISP_E                      LATDbits.LATD1
#define DISP_CTRL_LINES_DIR         TRISC &= 0xFC; TRISD &= ~(1u << 1)  // RC0, RC1, RD1 outputs
#define DISP_DATA_LINES             PORTD
#define DISP_DATA_DIR_WRITE_4BIT    (TRISD &= 0x0Fu)
#define DISP_DATA_DIR_READ_4BIT     (TRISD |= 0xF0u)
#define DISP_DATA_DIR_WRITE_8BIT    (TRISD = 0x00u)
#define DISP_DATA_DIR_READ          (TRISD = 0xFFu)
#define DISP_BF                     PORTDbits.RD7

#define DISP_E_HIGH     (DISP_E = 1u)
#define DISP_E_LOW      (DISP_E = 0u)
#define DISP_RS_IR      (DISP_RS = 0u)
#define DISP_RS_DR      (DISP_RS = 1u)
#define DISP_RW_WRITE   (DISP_RW = 0u)
#define DISP_RW_READ    (DISP_RW = 1u)

#define NUM_OF_LINES                            2u
#define NUM_OF_CHAR_PER_LINE                    16u
#define DDRAM_SIZE                              80u
#define DDRAM_16x2_NO_SHIFT_LINE1_START_ADDR    0x00u
#define DDRAM_16x2_NO_SHIFT_LINE2_START_ADDR    0x40u

#ifndef EXEC_FAIL
#define EXEC_FAIL       0
#define EXEC_SUCCESS    1
#endif

// Instruction Codes -- RS = 0 (IR), R/W = 0 (write)
// NOTE: Typically, instructions execute in 37us to 40us assuming fosc ~ 250kHz, except for RETURN_HOME, which takes 1.52ms.
//       In addition, there will be around 4us (tADD) from when the BF flag line goes low to when the AC is incremented/decremented.
#define CLEAR_DISPLAY                   0x01u    // Clears display and sets DDRAM address 0 in AC
#define RETURN_HOME                     0x02u    // Sets DDRAM address 0 in AC /wo changing DDRAM contents or display

#define ENTRY_MODE_SET_ID_INC           0x06u    // (I/D) Sets AC to increment /w each write into DDRAM
#define ENTRY_MODE_SET_NO_SHIFT         0x05u    // (S) Sets the dispaly to not have shift with I/D
#define ENTRY_MODE_SET_INC_NOSHIFT      0x06u    // Combines previous two
#define ENTRY_MODE_SET_DEFAULT          0x06u    // INC, no shift

#define DISPLAY_ONOFF_CTRL_ALL_OFF          0x08u    // Display off, cursor off, blinking off
#define DISPLAY_ONOFF_CTRL_DISP             0x04u    // D bit position
#define DISPLAY_ONOFF_CTRL_DISP_ON          0x0Cu    // Display (D) on
#define DISPLAY_ONOFF_CTRL_DISP_OFF         0x08u    // Display off
#define DISPLAY_ONOFF_CTRL_CURS             0x02u    // C bit position
#define DISPLAY_ONOFF_CTRL_CURS_ON          0x0Au    // Cursor (C) on
#define DISPLAY_ONOFF_CTRL_CURS_OFF         0x08u    // Cursor off
#define DISPLAY_ONOFF_CTRL_BLINK            0x01u    // B bit position
#define DISPLAY_ONOFF_CTRL_BLINK_ON         0x09u    // Blinking (B) on
#define DISPLAY_ONOFF_CTRL_BLINK_OFF        0x08u    // Blinking off
#define DISPLAY_ONOFF_CTRL_DON_CON_BOFF     0x0Eu    // Display on, cursor on, blinking off
#define DISPLAY_ONOFF_CTRL_DON_COFF_BOFF    0x0A    // Display on, cursor and blink off
#define DISPLAY_ONOFF_CTRL_DEFAULT          0x08u    // All offs

#define CURSOR_DISP_SHIFT_ON            0x18u    // S/C = 1; Shift on
#define CURSOR_DISP_SHIFT_RIGHT         0x14u    // R/L = 1; Shift will be to the right
#define CURSOR_DISP_SHIFT_LEFT          0x18u    // R/L = 0; Shift will be to the left
#define CURSOR_DISP_SHIFT_TYPICAL       0x10u    // Shift off; S/C = 0, R/L = 0

#define FUNCTION_SET_DL_8BIT            0x30u    // DL = 1; 8-bit operation
#define FUNCTION_SET_DL_4BIT            0x20u    // DL = 0; 4-bit operation
#define FUNCTION_SET_N_2LINE            0x28u    // N = 1; 2-line operation
#define FUNCTION_SET_N_1LINE            0x20u    // N = 0; 1-line operation
#define FUNCTION_SET_F_5x10             0x24u    // F = 1; 5x10 dot characters
#define FUNCTION_SET_F_5x8              0x20u    // F = 0; 5x8 dot characters
#define FUNCTION_SET_8BIT_2LINE_5x8     0x38u    // DL=1, N=1, F=0
#define FUNCTION_SET_4BIT_2LINE_5x8     0x28u    // DL=0, N=1, F=0
#define FUNCTION_SET_DL_BIT             4u       // DL is bit 4
#define FUNCTION_SET_DEFAULT            0x30u    // DL = 1 (8-bit), N = 0 (1-line), F = 0 (5x8 font)

// Only use the following if addr is a literal and not a variable! If addr is a variable, use the function disp_set_xx_addr() instead
#define SET_CGRAM_ADDRESS(addr)         (0x40u | addr)   // Only use if addr is a literal and not a variable! addr is 6-bit for CGRAM
#define SET_DDRAM_ADDRESS(addr)         (0x80u | addr)   // Only use if addr is a literal and not a variable! addr is 7-bit for DDRAM
#define SET_CGRAM_ADDR_INSTR            0x60u            // This is the location of the 1 in this instruction; the bits following will be the 6-bit address; OR this          
#define SET_DDRAM_ADDR_INSTR            0x80u            // This is the location of the 1 in this instruction; the bits following will be the 7-bit address; OR this          

// Some macros
#define TEST_BIT_SET(byte,bit)          (byte & (1u << bit))
#define TEST_BIT_CLEAR(byte,bit)        (~byte & (1u << bit))
#define CLEAR_BIT(byte,bit)             (byte &= ~(1u << bit))
#define SET_BIT(byte,bit)               (byte |= (1u << bit))

/* Non-instruction operation
 * If RS = 0 (IR) and RW = 1 (read), the data lines will have DB7 = BF, and DB6 to DB0 = AC
 * If RS = 1 (DR) and RW = 0 (write), the data lines represent what is to be written in DDRAM/CGRAM at wherever AC points to, after which AC
 *          is incremented/decremented depending on the I/D bit, which is configured in the "ENTRY_MODE_SET_xx" instructions.
 * If RS = 1 (DR) and RW = 1 (read), the data lines will read whatever from DDRAM or CGRAM whatever AC is currently pointed at.
 */
// </editor-fold>


/* Enumerated constants
 * The following is used to indicate what display is currently being driven.
 * Even if all these displays claim to use the HD44780 display driver IC, in my
 * experience, they still differ wildly in what they need to get properly
 * initialized as well as in the wait times...
 * Links for each display type:
 *  - QAPASS_EBAY: https://www.ebay.com/itm/16X2-1602LCD-Blue-Yellow-HD44780-Character-Display-Module-5V-for-Arduino/182723001237?hash=item2a8b23bb95:m:mLKfB9cKobTfoeiuRzByfFA
 *  - QAPASS_AMAZON: https://www.amazon.com/dp/B019D9TYMI?psc=1&ref=ppx_yo2_dt_b_product_details
 *  - ADAFRUIT_STANDARD_16x2: https://www.adafruit.com/product/181
 *  - ADAFRUIT_STANDARD_20x4: https://www.adafruit.com/product/198
 */
enum lcd_display_t {
    QAPASS_EBAY,
    QAPASS_AMAZON,
    ADAFRUIT_STANDARD_16x2,
    ADAFRUIT_STANDARD_20x4        
};
enum lcd_bit_mode_t {
    MODE_4BIT,
    MODE_8BIT
};


// Function prototypes
// NOTE: Recall for 16x2 non-shift operation, DDRAM address 0x00 to 0x0F are for line 1, 0x40 to 0x4F are for line 2.
// NOTE: The return type for some is uint8_t to indicate whether the function was carried out successfully (0x1) or not (0x0)
void static LCD_enable_toggle(void);
void static LCD_wait_for_BF(void);

void LCD_write_data_byte_4bit(uint8_t data);
void LCD_write_data_byte_8bit(uint8_t data);
void LCD_write_instr_byte_4bit(uint8_t instr);
void LCD_write_instr_byte_8bit(uint8_t instr);
void LCD_Init_ECE376(void);
void LCD_Init_amazonLCD(uint8_t mode_4bit);
void LCD_Init(uint8_t entry_mode, uint8_t disp_ctrl, uint8_t func_set, enum lcd_display_t disp_to_be_used);

// Basic LCD operations/instructions
uint8_t LCD_isInit(void);
uint8_t LCD_clear_display(void);
uint8_t LCD_return_home(void);
uint8_t LCD_read_current_address_counter(void);
uint8_t LCD_set_cursor_position(uint8_t line, uint8_t pos_on_line);
uint8_t LCD_write_characters(char * toWrite, uint8_t size);
uint8_t LCD_turn_off_cursor(void);
uint8_t LCD_turn_on_cursor(void);

// Higher level functions
void LCD_write_uint32_number(uint32_t num);



#endif	/* endif for the IFNDEF LCD_DRIVER_H */

