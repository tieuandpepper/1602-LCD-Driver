#ifndef LCD_H
#define LCD_H

// mode pin
#define LCD_MODE        GPIO_PORTB_BASE
#define RS              GPIO_PIN_0
#define EN              GPIO_PIN_1
#define OV              GPIO_PIN_2

//MACRO for mode
#define RS_instruction     0x00
#define RS_display         0x01
#define display_overflow   0x04
#define Enable             0x02
#define Disable            0x00

// data pin
#define LCD_DATA        GPIO_PORTD_BASE
#define D4              GPIO_PIN_0
#define D5              GPIO_PIN_1
#define D6              GPIO_PIN_2
#define D7              GPIO_PIN_3
#define DATA_PINS       GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3

// MACRO for data pins
#define clear_display               0x01    // clear entire display and set DDRAM address 0 in address counter
#define return_home                 0x02    // set DDRAM address 0 in address counter. Return display to original position. DDRAM contents remain unchanged
#define entry_mode                  0x06    // set mode to increment the address by one and shift the cursor to the right when writing to DDRAM/CGRAM
#define display_cursor_blink        0x0F  // set display on, cursor on and blink
#define display_cursor              0x0E    // set display on, cursor on, no blink
#define display_on                  0x0C    // set display on, no cursor
#define display_shift_right         0x1C    // display shift right
#define display_shift_left          0x18    // display shift left
#define cursor_shift_right          0x14    // cursor shift right
#define cursor_shift_left           0x10    // cursor shift left
#define set_8bit_2Line_5x8font      0x38    // Set interface data length of 8 bit (send 8-bit data each cycle), 2 lines display, font 5x8 dots
#define set_8bit_1Line_5x8font      0x30    // Set interface data length of 8 bit (send 8-bit data each cycle), 1 line display, font 5x8 dots
#define set_4bit_2Line_5x8font      0x28    // Set interface data length of 4 bit (send 4-bit data each cycle), 2 lines display, font 5x8 dots
#define set_4bit_1Line_5x8font      0x20    // Set interface data length of 4 bit (send 8-bit data each cycle), 1 line display, font 5x8 dots
#define cursor_line_2_front         0xC0    // Set the cursor to the beginning position on line 2
#define cursor_line_1_front         0x80    // Set the cursor to the beginning position on line 1
#define cursor_line_1_back          0xCF    // Set the cursor to the end position on line 2
#define cursor_line_2_back          0x0F    // Set the cursor to the end position on line 1
#define cursor_current              0x00    // no need to shift cursor

// System initialize
void Port_Init(void);                       // initialize ports
void delay_us(unsigned long time);          // delay in microseconds
void delay_ms(unsigned long time);          // delay in milliseconds

// LCD control
void LCD_Init(void);                                            //initialize LCD
void Enable_Pulse(unsigned long duty);                          // generate clock pulse for enable pin
void LCD_Write_4bit(unsigned char data);                        // write instructions/data in 4 bit mode
void LCD_Instruction(unsigned char command);                    // Write instruction to LCD
void LCD_Cursor(unsigned char location);                        // set the cursor position
void LCD_clear_line(uint8_t line);                              // clear line

// Print function
void LCD_Print_Int(uint64_t num);                         // Write a intgeger to screen
void LCD_PutChar(unsigned char data);                         // Write a character to screen
void LCD_Print_Str(char* str);                                // Write string to screen

#endif
