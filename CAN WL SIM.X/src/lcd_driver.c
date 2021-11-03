/*
 * File:   lcd_driver.c
 * Author: Abdullah Almosalami
 *
 * Created on October 23, 2021, 4:43 PM
 */


#include <xc.h>
#include "lcd_driver.h"


static uint8_t init_complete = 0x00;    // Have we initialized?
static uint8_t shift_mode = 0x0u;   // Is display in shift mode?
static uint8_t disp_16x2 = 0x1u;    // Is display 16x2?

static uint8_t current_entry_mode = ENTRY_MODE_SET_DEFAULT;
static uint8_t current_disp_ctrl = DISPLAY_ONOFF_CTRL_DEFAULT;
static uint8_t current_func_set = FUNCTION_SET_DEFAULT;

static uint8_t address_counter = 0x00;  // Not 100% reliable; only updated by calling LCD_read_current_AC())


// The following are the functions for the ECE376 ebay-purchased LCD display
void static LCD_enable_toggle(void){
    DISP_E_LOW;
    __delay_us(2u);
    DISP_E_HIGH;
    __delay_us(2u);     // Need to wait at least 0.5us for enable line
    DISP_E_LOW;
    __delay_us(2u);
}

// NOTE! This function will change the direction of the data pin used and also affect the outputs on the control lines!
void static LCD_wait_for_BF(void){
    // Wait for Busy Flag to clear
    DISP_DATA_DIR_READ;
    DISP_RS_IR;
    DISP_RW_READ;
    LCD_enable_toggle();
    while(DISP_BF);
}

// DB7 to DB4 used for data lines
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

// DB7 to DB4 used for data lines
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

void LCD_Init_ECE376(void){
    // Initialize I/O ports -- PORTC is for control lines, PORTD is for data lines, PORTE for debugging
    // Clear all ports
    PORTC = 0x00; PORTD = 0x00; // PORTE = 0x00;
    // Set pin directions
    DISP_CTRL_LINES_DIR;   // RC0, RC1, RC2 outputs
    DISP_DATA_DIR_READ_4BIT;   // All relevant PORTD pins as inputs for now
//    TRISE = 0xFC;   // RE0, RE1 outputs
    
    // For display to initialize, need to have at least a 9s delay...
    __delay_ms(4000);
    __delay_ms(4000);
    __delay_ms(2000);
    
    // We're gonna assume the Power Conditions for Internal Reset Circuitry
    //have not happened, so I'll be following page 45 of the HDD44780U datasheet    
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
    init_complete = 0x01u;
    current_entry_mode = ENTRY_MODE_SET_INC_NOSHIFT;
    current_disp_ctrl = DISPLAY_ONOFF_CTRL_DON_CON_BOFF;
    current_func_set = FUNCTION_SET_4BIT_2LINE_5x8;
}

void LCD_Init(uint8_t entry_mode, uint8_t disp_ctrl, uint8_t func_set){
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
    DISP_DATA_LINES = func_set;   // 8-bit, 2-line, 5x8 font
    LCD_enable_toggle();
    
    // Set display on/off control
    DISP_DATA_LINES = DISPLAY_ONOFF_CTRL_ALL_OFF; // Display on, cursor on, blinking off
    LCD_enable_toggle();
    
    // Clear display
    DISP_DATA_LINES = CLEAR_DISPLAY;
    LCD_enable_toggle();
    
    // Set entry-mode
    DISP_DATA_LINES = entry_mode; // Increment, No shift /w increment
    LCD_enable_toggle();
    
    // Turn on display and cursor after waiting a bit
    __delay_ms(100);
    DISP_DATA_LINES = disp_ctrl;
    LCD_enable_toggle();
    
    // Set flag for initialization
    init_complete = 0x01u;

    current_entry_mode = entry_mode;
    current_disp_ctrl = disp_ctrl;
    current_func_set = func_set;
}

uint8_t LCD_isInit(void){
    return init_complete;
}

uint8_t LCD_clear_display(void){
    if(TEST_BIT_CLEAR(current_entry_mode, FUNCTION_SET_DL_BIT)){
        LCD_write_instr_byte_4bit(CLEAR_DISPLAY);
    } else{
        LCD_write_instr_byte_8bit(CLEAR_DISPLAY);
    }
    
    __delay_ms(10);
    
    return EXEC_SUCCESS;
}

uint8_t LCD_return_home(void){
    if(!TEST_BIT_SET(current_entry_mode, FUNCTION_SET_DL_BIT)){
        LCD_write_instr_byte_4bit(RETURN_HOME);
    } else{
        LCD_write_instr_byte_8bit(RETURN_HOME);
    }
    
    return EXEC_SUCCESS;
    
    
    address_counter = 0x00;
    
    return EXEC_SUCCESS;
}

uint8_t LCD_read_current_address_counter(void){
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
        
    DISP_RS_IR;
    DISP_RW_READ;
    LCD_enable_toggle();
    address_counter = DISP_DATA_LINES & 0x7F;
    
    return address_counter;
}

// line and pos_on_line are NOT 0-indexed --> i.e., first position is 1, not 0
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
    
    if(TEST_BIT_CLEAR(current_entry_mode, FUNCTION_SET_DL_BIT)){
        LCD_write_instr_byte_4bit(SET_DDRAM_ADDR_INSTR | ddram_addr);
    } else{
        LCD_write_instr_byte_8bit(SET_DDRAM_ADDR_INSTR | ddram_addr);
    }
    
    address_counter = ddram_addr;
    
    return EXEC_SUCCESS;
}

// Before calling this function, make sure the cursor is set correctly!
uint8_t LCD_write_characters(char * toWrite, uint8_t size){
    // Make sure size of number of bytes is at least smaller than DDRAM size
    if(size > DDRAM_SIZE) { return EXEC_FAIL; }
    
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
    
    // Output to data lines
    for(uint8_t i=0u; i<size; i++){
        LCD_wait_for_BF();      // Make sure BF is not set
        // Prepare lines to write
        DISP_DATA_DIR_WRITE_4BIT;
        DISP_RS_DR;
        DISP_RW_WRITE;
        DISP_DATA_LINES = toWrite[i];
        LCD_enable_toggle();
        
        address_counter++;
    }
    
    return EXEC_SUCCESS;
}

uint8_t LCD_turn_off_cursor(void){
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
    
    // Make sure BF is not set
    LCD_wait_for_BF();
    
    // Prepare lines to write to IR
    DISP_DATA_DIR_WRITE_4BIT;
    DISP_RS_IR;
    DISP_RW_WRITE;
        
    // Write to data lines
    DISP_DATA_LINES = current_disp_ctrl & ~(DISPLAY_ONOFF_CTRL_CURS);
    LCD_enable_toggle();
    
    current_disp_ctrl &= ~(DISPLAY_ONOFF_CTRL_CURS);
    
    return EXEC_SUCCESS;
}

uint8_t LCD_turn_on_cursor(void){
    // Make sure display has been initialized
    if(!init_complete) { return EXEC_FAIL; }
    
    // Make sure BF is not set
    LCD_wait_for_BF();
    
    // Prepare lines to write to IR
    DISP_DATA_DIR_WRITE_4BIT;
    DISP_RS_IR;
    DISP_RW_WRITE;
        
    // Write to data lines
    DISP_DATA_LINES = current_disp_ctrl | DISPLAY_ONOFF_CTRL_CURS;
    LCD_enable_toggle();
    
    current_disp_ctrl |= DISPLAY_ONOFF_CTRL_CURS;
    
    return EXEC_SUCCESS;
}


// Now for the other LCD display I purchased from Amazon
void static LCD_enable_toggle_amazonLCD(void){
    DISP_E_LOW;
    __delay_us(1u);
    DISP_E_HIGH;
    __delay_us(1u);     // Need to wait at least 0.5us for enable line
    DISP_E_LOW;
    __delay_us(100u);
}

void LCD_write_data_byte_4bit_amazonLCD(uint8_t data){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_4BIT;    // Port D as output
    DISP_RS_DR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte via two 4-bit writes
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | (data & 0xF0);
    LCD_enable_toggle_amazonLCD();
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | ((data<<4) & 0xF0);
    LCD_enable_toggle_amazonLCD();
}

void LCD_write_data_byte_8bit_amazonLCD(uint8_t data){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_8BIT;    // Port D as output
    DISP_RS_DR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte
    DISP_DATA_LINES = data;
    LCD_enable_toggle_amazonLCD();
}

void LCD_write_instr_byte_4bit_amazonLCD(uint8_t instr){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_4BIT;    // Port D as output
    DISP_RS_IR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte via two 4-bit writes, upper-nibble first
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | (instr & 0xF0);
    LCD_enable_toggle_amazonLCD();
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | ((instr<<4) & 0xF0);
    LCD_enable_toggle_amazonLCD();
}

void LCD_write_instr_byte_8bit_amazonLCD(uint8_t instr){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_8BIT;    // Port D as output
    DISP_RS_IR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send byte
    DISP_DATA_LINES = instr;
    LCD_enable_toggle_amazonLCD();
}

void LCD_write_instr_nibble_4bit_amazonLCD(uint8_t instr){
    // Prepare control & data lines
    DISP_CTRL_LINES_DIR;    // RC0 (RS), RC1 (RW), RC2 (E) outputs
    DISP_DATA_DIR_WRITE_4BIT;    // Port D as output
    DISP_RS_IR;     // Set instruction register to be written to
    DISP_RW_WRITE;  // Set write mode
    
    // Send just lower nibble of instr byte onto DB7 to DB4
    DISP_DATA_LINES = (DISP_DATA_LINES & 0x0F) | ((instr<<4) & 0xF0);
    LCD_enable_toggle_amazonLCD();
}

uint8_t LCD_clear_display_amazonLCD(void){
    if(TEST_BIT_CLEAR(current_entry_mode, FUNCTION_SET_DL_BIT)){
        LCD_write_instr_byte_4bit_amazonLCD(CLEAR_DISPLAY);
    } else{
        LCD_write_instr_byte_8bit_amazonLCD(CLEAR_DISPLAY);
    }
    __delay_ms(2);
    
    return EXEC_SUCCESS;
}

void LCD_Init_amazonLCD(uint8_t mode_4bit){
    __delay_ms(50);
    
    if(mode_4bit){
        CLEAR_BIT(current_func_set,FUNCTION_SET_DL_BIT);
        
        LCD_write_instr_nibble_4bit_amazonLCD(0x03);
        __delay_us(4500);
        LCD_write_instr_nibble_4bit_amazonLCD(0x03);
        __delay_us(450);
        LCD_write_instr_nibble_4bit_amazonLCD(0x03);
        __delay_us(150);
        LCD_write_instr_nibble_4bit_amazonLCD(0x02);    // This is to set 4-bit interface
        
        LCD_write_instr_byte_4bit_amazonLCD(FUNCTION_SET_4BIT_2LINE_5x8);
        LCD_write_instr_byte_4bit_amazonLCD(DISPLAY_ONOFF_CTRL_DON_COFF_BOFF);
        LCD_clear_display_amazonLCD();
        LCD_write_instr_byte_4bit_amazonLCD(ENTRY_MODE_SET_INC_NOSHIFT);
        
        current_func_set = FUNCTION_SET_4BIT_2LINE_5x8;
        current_disp_ctrl = DISPLAY_ONOFF_CTRL_DON_COFF_BOFF;
        current_entry_mode = ENTRY_MODE_SET_INC_NOSHIFT;
        
    } else{
        SET_BIT(current_func_set,FUNCTION_SET_DL_BIT);
        
        LCD_write_instr_byte_8bit_amazonLCD(FUNCTION_SET_8BIT_2LINE_5x8);
        __delay_us(4500);
        LCD_write_instr_byte_8bit_amazonLCD(FUNCTION_SET_8BIT_2LINE_5x8);
        __delay_us(150);
        LCD_write_instr_byte_8bit_amazonLCD(FUNCTION_SET_8BIT_2LINE_5x8);
        
        LCD_write_instr_byte_8bit_amazonLCD(FUNCTION_SET_8BIT_2LINE_5x8);
        LCD_write_instr_byte_8bit_amazonLCD(DISPLAY_ONOFF_CTRL_DON_COFF_BOFF);
        LCD_clear_display_amazonLCD();
        LCD_write_instr_byte_8bit_amazonLCD(ENTRY_MODE_SET_INC_NOSHIFT);
        
        // Set some flags        
        init_complete = 0x01u;
        current_func_set = FUNCTION_SET_8BIT_2LINE_5x8;
        current_disp_ctrl = DISPLAY_ONOFF_CTRL_DON_COFF_BOFF;
        current_entry_mode = ENTRY_MODE_SET_INC_NOSHIFT;
    }
}

uint8_t LCD_set_cursor_position_amazonLCD(uint8_t line, uint8_t pos_on_line){
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
    
    if(TEST_BIT_CLEAR(current_entry_mode, FUNCTION_SET_DL_BIT)){
        LCD_write_instr_byte_4bit_amazonLCD(SET_DDRAM_ADDR_INSTR | ddram_addr);
    } else{
        LCD_write_instr_byte_8bit_amazonLCD(SET_DDRAM_ADDR_INSTR | ddram_addr);
    }
    
    address_counter = ddram_addr;
    
    return EXEC_SUCCESS;    
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