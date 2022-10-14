#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "string.h"

void Port_Init(void) {
	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    
    // ADD Tiva-C GPIO setup - enables port B, sets pins 0-2 pins for output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(LCD_MODE, RS | EN | OV);
    
    // ADD Tiva-C GPIO setup - enables port D, sets pins 0-3 pins for output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(LCD_DATA, DATA_PINS);
}

void delay_us(unsigned long time) {
	uint32_t i;
	for (i = 0; i < time; ++i) {
		SysCtlDelay(14);				// 1 us delay for 40MHz CPU clock
	}
}

void delay_ms(unsigned long time) {
	uint32_t i;
	for (i = 0; i < time; ++i) {
		SysCtlDelay(13330);				// 1 ms delay for 40MHz CPU clock
	}
}

void LCD_Init(void) {
	LCD_Instruction(set_4bit_2Line_5x8font); 	// Set 4 bit mode, 2 lines display, font 5x8 dots
	LCD_Instruction(display_on);				// turn display on, no cursor
	LCD_Instruction(entry_mode);				// set mode to increment and shift to the right each time of writing
	LCD_Instruction(clear_display);				//clear DDRAM, nothing on the display
	LCD_Instruction(return_home);				//set cursor to the beginning of line 1
	delay_ms(2); 								// wait for the last instruction to complete (take 1.52ms)
}

void Enable_Pulse(unsigned long duty){
	GPIOPinWrite(LCD_MODE, EN , Enable);
	delay_us(duty);
	GPIOPinWrite(LCD_MODE, EN, 0); 
}


void LCD_Write_4bit(unsigned char data){
	GPIOPinWrite(LCD_DATA, DATA_PINS, data);
	Enable_Pulse(1);
}

void LCD_Instruction(unsigned char command){
	GPIOPinWrite(LCD_MODE,RS,RS_instruction);
	delay_us(40);
	unsigned char upper_nibble = command >> 4;
	LCD_Write_4bit(upper_nibble);
	LCD_Write_4bit(command);
}

void LCD_PutChar(unsigned char data){
	GPIOPinWrite(LCD_MODE,RS,RS_display);
	delay_us(40);
	unsigned char upper_nibble = data >> 4;			// separate the upper 4 bits
	LCD_Write_4bit(upper_nibble);					// write the upper nibble
	LCD_Write_4bit(data);							// write the lower nibble
	GPIOPinWrite(LCD_MODE,RS,RS_instruction);		// reset the RS pin to previous state
}

void LCD_Print_Str(char* str, unsigned char cursor_position){
	unsigned int i;
	if (cursor_position != cursor_current)
		LCD_Instruction(cursor_position);					// Set the location of the cursor (if cursor > 0)
	
	for (i =0; str[i] != 0; i++) { 							// ASCII code 0 is NULL
		LCD_PutChar(str[i]);								// Print a character to display 
		if (i > 15) {										// Check for Overflow
			GPIOPinWrite(LCD_MODE,OV,display_overflow);		// Set overflow bit (PB2)
		}
	}
}

void LCD_Overflow(void){
	if (GPIOPinRead(LCD_MODE, OV)) {					// PB2 = 1 when overflow
		while (1) {										// infinite loop
			LCD_Instruction(display_shift_left);		// shift display to the left
			delay_ms(300);								// wait time for each shift
		}
	}
}
