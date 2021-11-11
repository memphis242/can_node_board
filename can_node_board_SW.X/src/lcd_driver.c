/*
 * File:   lcd_driver.c
 * Author: Abdullah Almosalami
 *
 * Created on October 23, 2021, 4:43 PM
 *
 */


#include <xc.h>
#include "lcd_driver.h"


// <editor-fold defaultstate="collapsed" desc="GLOBAL AND STATIC VARIABLES">
// Global and static (internally linked) variables relevant to the LCD driver
static enum lcd_display_t CURRENT_DISP = QAPASS_EBAY;
static enum lcd_bit_mode_t DISP_BIT_MODE = MODE_4BIT;
static uint8_t init_complete = 0x00;    // Have we initialized?
static uint8_t shift_mode = 0x0u;   // Is display in shift mode?
static uint8_t disp_16x2 = 0x1u;    // Is display 16x2?

static uint8_t current_entry_mode = ENTRY_MODE_SET_DEFAULT;
static uint8_t current_disp_ctrl = DISPLAY_ONOFF_CTRL_DEFAULT;
static uint8_t current_func_set = FUNCTION_SET_DEFAULT;

static uint8_t address_counter = 0x00;  // Not 100% reliable; only updated by
                                        // calling LCD_read_current_AC())
// </editor-fold>


// Functions

/* Function: LCD_enable_toggle
 * ---------------------------------
 * Toggles the enable pin connected to the LCD enable, which allows the display
 * to read in/write to the data lines.
 * 
 * NOTE! Depending on the display used, you will want to change the delay time
 *       used here! Hence the switch-case statement.
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void static LCD_enable_toggle(void){
    switch(CURRENT_DISP){
        
        case QAPASS_EBAY:
            DISP_E_LOW;
            __delay_us(2u);
            DISP_E_HIGH;
            __delay_us(2u);     // Need to wait at least 0.5us for enable line
            DISP_E_LOW;
            __delay_us(2u);
            break;
            
        case QAPASS_AMAZON:
            DISP_E_LOW;
            __delay_us(1u);
            DISP_E_HIGH;
            __delay_us(1u);     // Need to wait at least 0.5us for enable line
            DISP_E_LOW;
            __delay_us(100u);
            break;
            
        case ADAFRUIT_STANDARD_16x2:
            DISP_E_LOW;
            __delay_us(2u);
            DISP_E_HIGH;
            __delay_us(2u);     // Need to wait at least 0.5us for enable line
            DISP_E_LOW;
            __delay_us(2u);
            break;
            
        case ADAFRUIT_STANDARD_20x4:
            DISP_E_LOW;
            __delay_us(2u);
            DISP_E_HIGH;
            __delay_us(2u);     // Need to wait at least 0.5us for enable line
            DISP_E_LOW;
            __delay_us(2u);
            break;
        
    }
    
}

/* Function: LCD_wait_for_BF
 * ---------------------------------
 * Ideally, this function would be used to confirm that an instruction has been
 * executed before trying to ask the display to execute another instruction. In
 * practice, it seems (at least as of 11/05/2021) that this causes unnecessary
 * added complexity. I'll note that the way these LCD displays indicate they
 * are busy is by setting the Busy Flag pin, which is DB7 when RS = 0
 * (instruction register) and R/~W = 1 (read from the LCD). When the LCD is not
 * busy, this line is pulled low.
 * 
 * NOTE! As a result of having to set the RS and R/~W control lines, as well as
 *       changing the data lines to inputs, programmer should take care to
 *       set the state of these lines to the same state they were in prior to
 *       calling this function!
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void static LCD_wait_for_BF(void){
    // Wait for Busy Flag to clear
    DISP_DATA_DIR_READ;
    DISP_RS_IR;
    DISP_RW_READ;
    LCD_enable_toggle();
    while(DISP_BF);
}

/* Function: LCD_write_data_byte_4bit
 * ---------------------------------
 * Sends a byte to the LCD data register in two 4-bit writes. This is part of
 * the HD44780's 4-bit mode of operation, allowing the use of four less data
 * lines from 8-bit mode. The 4 lines used are DB7 to DB4.
 * 
 * Parameters: uint8_t data --> the byte to be sent
 * 
 * Returns: none
 */
void LCD_write_data_byte_4bit(uint8_t data){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_4BIT;    // Port D as output
    DISP_RS_DR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte via two 4-bit writes
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | (data & 0xF0);
    LCD_enable_toggle();
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | ((data<<4) & 0xF0);
    LCD_enable_toggle();
    
    __delay_ms(10);
}

/* Function: LCD_write_data_byte_8bit
 * ---------------------------------
 * Sends a byte to the LCD data register in a single 8-bit write. This is part of
 * the HD44780's 8-bit mode of operation, which works twice as fast as the
 * 4-bit mode of operation but requires twice the number of data lines.
 * 
 * Parameters: uint8_t data --> the byte to be sent
 * 
 * Returns: none
 */
void LCD_write_data_byte_8bit(uint8_t data){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_8BIT;    // Port D as output
    DISP_RS_DR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte
    DISP_DATA_LINES = data;
    LCD_enable_toggle();
    
    __delay_ms(10);
}

/* Function: LCD_write_instr_byte_4bit
 * ---------------------------------
 * Just like the data write version of the function except this will be for
 * sending a byte to the instruction register - i.e., sending an instruction.
 * 
 * Parameters: uint8_t instr --> the byte to be sent
 * 
 * Returns: none
 */
void LCD_write_instr_byte_4bit(uint8_t instr){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_4BIT;    // Port D as output
    DISP_RS_IR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte via two 4-bit writes, upper-nibble first
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | (instr & 0xF0);
    LCD_enable_toggle();
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | ((instr<<4) & 0xF0);
    LCD_enable_toggle();
    
    __delay_ms(10);
}

/* Function: LCD_write_instr_byte_8bit
 * ---------------------------------
 * Just like the data write version of the function except this will be for
 * sending a byte to the instruction register - i.e., sending an instruction.
 * 
 * Parameters: uint8_t instr --> the byte to be sent
 * 
 * Returns: none
 */
void LCD_write_instr_byte_8bit(uint8_t instr){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_8BIT;    // Port D as output
    DISP_RS_IR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte
    DISP_DATA_LINES = instr;
    LCD_enable_toggle();
    
    __delay_ms(10);
}

/* Function: LCD_Init_ECE376
 * ---------------------------------
 * Initializes the display assuming it is a QAPASS_EBAY. I called this ECE376
 * because this is the display we had for that class. I will add that I am
 * using 4-bit mode by default for this display.
 * 
 * Parameters: none
 * 
 * Returns: none
 */
void LCD_Init_ECE376(void){
    // Initialize I/O ports -- PORTC is for control lines, PORTD is for data lines, PORTE for debugging
    // Clear all ports
    PORTC = 0x00; PORTD = 0x00; // PORTE = 0x00;
//    // Set pin directions
    DISP_CTRL_LINES_DIR;   // RC0, RC1, RC2 outputs
    DISP_DATA_DIR_READ_4BIT;   // All relevant PORTD pins as inputs for now

    
    // For display to initialize, need to have at least a 9s delay...
    __delay_ms(4000);
    __delay_ms(4000);
    __delay_ms(2000);
    
    // We're gonna assume the Power Conditions for Internal Reset Circuitry
    // have not happened, so I'll be following page 45 of the HDD44780U datasheet    
    // Perform initialization by instructions:
    __delay_ms(100);     // Initial wait after Power On
    LCD_write_instr_byte_4bit(FUNCTION_SET_DEFAULT); // 0x30
    __delay_ms(5);
    LCD_write_instr_byte_4bit(FUNCTION_SET_DEFAULT);
    __delay_us(150);
    LCD_write_instr_byte_4bit(FUNCTION_SET_DEFAULT);
    
    LCD_write_instr_byte_4bit(FUNCTION_SET_4BIT_2LINE_5x8); // 0x28
    LCD_write_instr_byte_4bit(DISPLAY_ONOFF_CTRL_DON_CON_BOFF); // 0x0E
    LCD_write_instr_byte_4bit(CLEAR_DISPLAY);   // 0x01
    LCD_write_instr_byte_4bit(ENTRY_MODE_SET_INC_NOSHIFT);  // 0x06
    
    __delay_ms(100);
    
    // Set flags for initialization
    CURRENT_DISP = QAPASS_EBAY;
    DISP_BIT_MODE = MODE_4BIT;
    init_complete = 0x01u;
    current_entry_mode = ENTRY_MODE_SET_INC_NOSHIFT;
    current_disp_ctrl = DISPLAY_ONOFF_CTRL_DON_CON_BOFF;
    current_func_set = FUNCTION_SET_4BIT_2LINE_5x8;
}

/* Function: LCD_Init_amazonLCD
 * ---------------------------------
 * Initializes the display assuming it is a QAPASS_AMAZON. I called this amazonLCD
 * because this is the display I purchased from amazon a while ago. See the url
 * link in the lcd_driver.h file.
 * 
 * Parameters: uint8_t mode_4bit --> Used to indicate whether or not 4-bit mode
 *                                   will be used.
 * 
 * Returns: none
 */
void LCD_Init_amazonLCD(uint8_t mode_4bit){
    __delay_ms(50);
    
    if(mode_4bit){
        CLEAR_BIT(current_func_set,FUNCTION_SET_DL_BIT);
        
        LCD_write_instr_byte_4bit(0x03);
        __delay_us(4500);
        LCD_write_instr_byte_4bit(0x03);
        __delay_us(450);
        LCD_write_instr_byte_4bit(0x03);
        __delay_us(150);
        LCD_write_instr_byte_4bit(0x02);    // This is to set 4-bit interface
        
        LCD_write_instr_byte_4bit(FUNCTION_SET_4BIT_2LINE_5x8);
        LCD_write_instr_byte_4bit(DISPLAY_ONOFF_CTRL_DON_COFF_BOFF);
        LCD_clear_display();
        LCD_write_instr_byte_4bit(ENTRY_MODE_SET_INC_NOSHIFT);
        
        current_func_set = FUNCTION_SET_4BIT_2LINE_5x8;
        current_disp_ctrl = DISPLAY_ONOFF_CTRL_DON_COFF_BOFF;
        current_entry_mode = ENTRY_MODE_SET_INC_NOSHIFT;
        DISP_BIT_MODE = MODE_4BIT;
        
    } else{
        SET_BIT(current_func_set,FUNCTION_SET_DL_BIT);
        
        LCD_write_instr_byte_8bit(FUNCTION_SET_8BIT_2LINE_5x8);
        __delay_us(4500);
        LCD_write_instr_byte_8bit(FUNCTION_SET_8BIT_2LINE_5x8);
        __delay_us(150);
        LCD_write_instr_byte_8bit(FUNCTION_SET_8BIT_2LINE_5x8);
        
        LCD_write_instr_byte_8bit(FUNCTION_SET_8BIT_2LINE_5x8);
        LCD_write_instr_byte_8bit(DISPLAY_ONOFF_CTRL_DON_COFF_BOFF);
        LCD_clear_display();
        LCD_write_instr_byte_8bit(ENTRY_MODE_SET_INC_NOSHIFT);
        
        // Set some flags
        CURRENT_DISP = QAPASS_AMAZON;
        init_complete = 0x01u;
        current_func_set = FUNCTION_SET_8BIT_2LINE_5x8;
        current_disp_ctrl = DISPLAY_ONOFF_CTRL_DON_COFF_BOFF;
        current_entry_mode = ENTRY_MODE_SET_INC_NOSHIFT;
        DISP_BIT_MODE = MODE_8BIT;
    }
}

/* Function: LCD_Init
 * ---------------------------------
 * Initializes the display with no underlying assumptions made. User needs to
 * send as arguments the desired configuration of the display.
 * 
 * Parameters: uint8_t entry_mode --> increment or decrement AC, shit or no shift
 *             uint8_t disp_ctrl --> display, cursor, blinking on/off
 *             uint8_t func_set --> 8 or 4-bit, num of lines, character font
 *             enum lcd_display_t disp_to_be_used --> Which display is being used
 * 
 * Returns: none
 */
void LCD_Init(uint8_t entry_mode, uint8_t disp_ctrl, uint8_t func_set, enum lcd_display_t disp_to_be_used){
    // We're gonna assume the Power Conditions for Internal Reset Circuitry have not happened, so I'll be following page 45 of the HDD44780U datasheet    
    // Perform initialization by instructions:
    __delay_ms(60);     // Initial wait after Power On
    
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_4BIT;    // Port D as output
    DISP_RS_IR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // First function set block
    DISP_DATA_LINES = 0x30; // 8-bit operation
    DISP_E_HIGH;
    __delay_ms(10);
    DISP_E_LOW;
    __delay_ms(10);
    
    // Second function set block
    DISP_DATA_LINES = 0x30; // 8-bit operation
    DISP_E_HIGH;
    __delay_ms(1);
    DISP_E_LOW;
    __delay_ms(1);
    
    // Third function set block
    DISP_DATA_LINES = 0x30; // 8-bit operation
    DISP_E_HIGH;
    __delay_ms(1);
    DISP_E_LOW;
    __delay_ms(1);
    
    
    // Now begin actual configuration
    // Set functional mode
    DISP_DATA_LINES = func_set;
    LCD_enable_toggle();
    
    // Set display on/off control
    DISP_DATA_LINES = DISPLAY_ONOFF_CTRL_ALL_OFF;
    LCD_enable_toggle();
    
    // Clear display
    DISP_DATA_LINES = CLEAR_DISPLAY;
    LCD_enable_toggle();
    
    // Set entry-mode
    DISP_DATA_LINES = entry_mode;
    LCD_enable_toggle();
    
    // Turn on display and cursor after waiting a bit
    __delay_ms(100);
    DISP_DATA_LINES = disp_ctrl;
    LCD_enable_toggle();
    
    if(TEST_BIT_CLEAR(entry_mode, FUNCTION_SET_DL_BIT)){
        DISP_BIT_MODE = MODE_4BIT;
    } else{
        DISP_BIT_MODE = MODE_8BIT;
    }
    
    // Set flag for initialization
    CURRENT_DISP = disp_to_be_used;
    init_complete = 0x01u;
    current_entry_mode = entry_mode;
    current_disp_ctrl = disp_ctrl;
    current_func_set = func_set;
}


// Basic LCD operations

/* Function: LCD_isInit
 * ---------------------------------
 * Is the LCD initialized?
 * 
 * Parameters: none
 * 
 * Returns: uint8_t --> Yes = 1; No = 0;
 */
uint8_t LCD_isInit(void){
    return init_complete;
}

/* Function: LCD_clear_display
 * ---------------------------------
 * Clear the display.
 * 
 * Parameters: none
 * 
 * Returns: uint8_t --> Yes = 1; No = 0;
 */
uint8_t LCD_clear_display(void){
    if(DISP_BIT_MODE == MODE_4BIT){
        LCD_write_instr_byte_4bit(CLEAR_DISPLAY);
    } else{
        LCD_write_instr_byte_8bit(CLEAR_DISPLAY);
    }
    
    __delay_ms(10);
    
    return EXEC_SUCCESS;
}

/* Function: LCD_return_home
 * ---------------------------------
 * Set the cursor back to the first row first column.
 * 
 * Parameters: none
 * 
 * Returns: uint8_t --> Successful = 1; Fail = 0;
 * 
 * TODO:
 *      1- Need to utilize succ_or_fail variable after I change the LCD routines
 *         to actually return their execution success value.
 */
uint8_t LCD_return_home(void){
    uint8_t succ_or_fail = 0x00;
    if(DISP_BIT_MODE == MODE_4BIT){
        LCD_write_instr_byte_4bit(RETURN_HOME);
    } else{
        LCD_write_instr_byte_8bit(RETURN_HOME);
    }
    address_counter = 0x00;
    
    return EXEC_SUCCESS;
}

/* Function: LCD_read_current_address_counter
 * ---------------------------------
 * Read from the LCD what its address counter is currently set at. Also updates
 * the address_counter static variable.
 * 
 * Parameters: none
 * 
 * Returns: uint8_t --> The address counter value.
 * 
 * TODO:
 *      1- Currently, this reads in 8-bit mode. Need to also do 4-bit mode.
 */
uint8_t LCD_read_current_address_counter(void){
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
        
    DISP_RS_IR;
    DISP_RW_READ;
    LCD_enable_toggle();
    address_counter = DISP_DATA_LINES & 0x7F;
    
    return address_counter;
}

/* Function: LCD_set_cursor_position
 * ---------------------------------
 * Sets the cursor position on the display.
 * 
 * NOTE: line and pos_on_line are NOT 0-indexed --> i.e., first position is 1, not 0
 * 
 * Parameters: none
 * 
 * Returns: uint8_t --> Successfully executed = 1, failed = 0
 * 
 * TODO:
 *      1- Account for different display sizes (e.g., 20x4, 16x4, 20x2, ...)
 */
uint8_t LCD_set_cursor_position(uint8_t line, uint8_t pos_on_line){    
    // Check that the arguments are reasonable
    if(line > NUM_OF_LINES || pos_on_line > NUM_OF_LINES * NUM_OF_CHAR_PER_LINE){
        return EXEC_FAIL;
    }
    
    uint8_t ddram_addr;     // This should be a 7-bit address
    
    // Set appropriate DDRAM Address
    if(!shift_mode && disp_16x2){
        // Recall line 1 is for DDRAM 0x00 to 0x0F and line 2 is for DDRAM 0x40 to 0x4F
        if(line == 1u && pos_on_line < NUM_OF_CHAR_PER_LINE){
            ddram_addr = DDRAM_16x2_NO_SHIFT_LINE1_START_ADDR + pos_on_line - 1;            
            
        } else if(line == 2u && pos_on_line < NUM_OF_CHAR_PER_LINE){
            ddram_addr = DDRAM_16x2_NO_SHIFT_LINE2_START_ADDR + pos_on_line - 1;
            
        } else if(line == 1u && pos_on_line > NUM_OF_CHAR_PER_LINE){
            ddram_addr = DDRAM_16x2_NO_SHIFT_LINE2_START_ADDR + pos_on_line - 16 - 1;
            
        } else{
            return EXEC_FAIL;
        }
    }
    
    if(DISP_BIT_MODE == MODE_4BIT){
        LCD_write_instr_byte_4bit(SET_DDRAM_ADDR_INSTR | ddram_addr);
    } else{
        LCD_write_instr_byte_8bit(SET_DDRAM_ADDR_INSTR | ddram_addr);
    }
    
    address_counter = ddram_addr;
    
    return EXEC_SUCCESS;
}

/* Function: LCD_set_cursor_position
 * ---------------------------------
 * Writes several characters (bytes) in a row to the data register.
 * 
 * NOTE: Before calling this function, make sure the cursor is set correctly!
 * 
 * Parameters: char * toWrite --> Characters to write
 *             uint8_t size --> Size of bytes to write. Not using size_t because
 *                              size of buffer should NOT be large...
 * 
 * Returns: uint8_t --> Successfully executed = 1, failed = 0
 */
uint8_t LCD_write_characters(char * toWrite, uint8_t size){
    // Make sure size of number of bytes is at least smaller than DDRAM size
    if(size > DDRAM_SIZE) { return EXEC_FAIL; }
    
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
    
    
    if(DISP_BIT_MODE == MODE_4BIT){
        LCD_write_instr_byte_4bit(CLEAR_DISPLAY);
    } else{
        LCD_write_instr_byte_8bit(CLEAR_DISPLAY);
    }
    
    // Output to data lines
    for(uint8_t i=0u; i<size; i++){
        
        if(DISP_BIT_MODE == MODE_4BIT){
            LCD_write_data_byte_4bit(toWrite[i]);
        } else {
            LCD_write_data_byte_8bit(toWrite[i]);
        }
        
        address_counter++;
    }
    
    return EXEC_SUCCESS;
}

/* Function: LCD_turn_off_cursor
 * ---------------------------------
 * Turns cursor off.
 *
 * Parameters: none
 * 
 * Returns: uint8_t --> Successfully executed = 1, failed = 0
 */
uint8_t LCD_turn_off_cursor(void){
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
    
    // Send instruction
    if(DISP_BIT_MODE == MODE_4BIT){
        LCD_write_instr_byte_4bit(current_disp_ctrl & ~(DISPLAY_ONOFF_CTRL_CURS));
    } else{
        LCD_write_instr_byte_8bit(current_disp_ctrl & ~(DISPLAY_ONOFF_CTRL_CURS));
    }
    
    current_disp_ctrl &= ~(DISPLAY_ONOFF_CTRL_CURS);
    
    return EXEC_SUCCESS;
}

/* Function: LCD_turn_on_cursor
 * ---------------------------------
 * Turns cursor on.
 *
 * Parameters: none
 * 
 * Returns: uint8_t --> Successfully executed = 1, failed = 0
 */
uint8_t LCD_turn_on_cursor(void){
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
    
    // Send instruction
    if(DISP_BIT_MODE == MODE_4BIT){
        LCD_write_instr_byte_4bit(current_disp_ctrl | DISPLAY_ONOFF_CTRL_CURS);
    } else{
        LCD_write_instr_byte_8bit(current_disp_ctrl | DISPLAY_ONOFF_CTRL_CURS);
    }
    
    current_disp_ctrl |= DISPLAY_ONOFF_CTRL_CURS;
    
    return EXEC_SUCCESS;
}


// Higher-level functions
void LCD_write_uint32_number(uint32_t num){
    
    // Get numbers of digits --> uint32_t can only go as high as 4 billion, so 10 digits max
    uint8_t num_of_digits = 0u;
    if(num < 10u) num_of_digits = 1u;
    else if(num < 100u) num_of_digits = 2u;
    else if(num < 1000u) num_of_digits = 3u;
    else if(num < 10000u) num_of_digits = 4u;
    else if(num < 100000u) num_of_digits = 5u;
    else if(num < 1000000u) num_of_digits = 6u;
    else if(num < 10000000u) num_of_digits = 7u;
    else if(num < 100000000u) num_of_digits = 8u;
    else if(num < 1000000000u) num_of_digits = 9u;
    else num_of_digits = 10u;
    
    // Now get the individual digits into an array where each entry is a digit, starting from least significant digit
    uint8_t digits[10] = {0u};
    for(uint8_t i=0; i<num_of_digits; i++){
        digits[i] = num % 10;
        num /= 10;
    }
    
    // Now print out each digit, going in reverse order through the array
    for(int8_t i=((int8_t)num_of_digits-1); i>=0; i--){
        LCD_write_data_byte_4bit( (char) digits[i] + '0');
    }
    
}



//// Initialize I/O ports -- PORTC is for control lines, PORTD is for data lines, PORTE for debugging
//    // Clear all ports just to help with debugging
//    PORTC = 0x00;
//    PORTD = 0x00;
//    PORTE = 0x00;
//    // Set pin directions
//    ADCON1bits.PCFG = 0x0F;
//    TRISC = 0xF8;   // RC0, RC1, RC2 outputs
//    TRISD = 0xFF;   // All of PORTD inputs for now
//    TRISE = 0xFC;   // RE0, RE1 outputs
//    
//    // For display to initialize, need to have at least a 9s delay...
//    __delay_ms(4000);
//    __delay_ms(4000);
//    __delay_ms(2000);
//        
//    // LCD test - ECE376 LCD from ebay
//    LCD_Init_ECE376();
//    LCD_set_cursor_position(1,1);   // Row 1, position 1 (NOT 0 indexed)
//    char message[10u] = {'H', 'i', ' ', 'A', 's', 'h', 'l', 'e', 'y', '!'};
//    for(uint8_t i=0; i<10; i++) LCD_write_data_byte_4bit(message[i]);
