#include <htc.h>
#include <stdarg.h>
#include <string.h>

#ifndef _GPIO_H
#include "gpio.h"
#endif

#ifndef _UTILS_H
#include "utils.h"
#endif

#ifndef _PRINT_H
#include "print.h"
#endif

#include "lcd.h"

LCD_PIN_CONFIG pinconfig;

/**************************************************************
@brief	initialize the lcd display module
@param	rs register select gpio pin
@param	en enable gpio pin
@param	d4 databit 4 gpio pin
@param	d5 databit 5 gpio pin
@param	d6 databit 6 gpio pin
@param	d7 databit 7 gpio pin
@return	function returns nothing
***************************************************************/
void lcd_initialize(char rs, char en, char d4, char d5, char d6,
char d7)
{
	pinconfig.RS = rs;
	pinconfig.EN = en;
	pinconfig.D4 = d4;
	pinconfig.D5 = d5;
	pinconfig.D6 = d6;
	pinconfig.D7 = d7;
	
	gpio_set_output(pinconfig.RS);
	gpio_set_output(pinconfig.EN);
	gpio_set_output(pinconfig.D4);
	gpio_set_output(pinconfig.D5);
	gpio_set_output(pinconfig.D6);
	gpio_set_output(pinconfig.D7);
	DelayMS(250);
	
	lcd_command(0x03); DelayMS(5);
	lcd_command(0x03); DelayMS(5);
	lcd_command(0x03); DelayUS(150);
	
	lcd_command(RETURN_HOME); DelayUS(150);
	lcd_command(FOUR_BIT | TWO_LINE | CHAR_SET1); DelayUS(150);
	lcd_command(DISPLAY_ON | CURSOR_OFF | BLINK_OFF); DelayUS(150);
	lcd_command(SHIFT_DISABLE | CUR_INCREMENT); DelayUS(150);
	lcd_function(CLEAR_DISPLAY);
}

/******************************************************************
@brief	lcd display command mode
@param	command require command data need to send
@return	function returns nothing
******************************************************************/	
void lcd_command(unsigned char command)
{
	gpio_put_low(pinconfig.RS);
	lcd_allort(command & 0xF0); lcd_enable();
	lcd_allort((command << 4) & 0xF0); lcd_enable();
}

/*****************************************************************
@brief	same as command mode but some commands req delay
@param	command require command data need to send
@return function returns nothing
******************************************************************/
void lcd_function(unsigned char command)
{
	lcd_command(command);
	DelayMS(20);
}

/*****************************************************************
@brief	lcd display write mode
@param	command lcd display cursor address
@param	data write data charactor
@return	function returns nothing
******************************************************************/
void lcd_write(unsigned char command, unsigned char data)
{
	lcd_command(command);
	gpio_put_high(pinconfig.RS);
	lcd_allort(data & 0xF0); lcd_enable();
	lcd_allort((data << 4) & 0xF0); lcd_enable();
}

/****************************************************************
@brief	same as lcd display write mode but number of char
@param	command lcd display cursor address
@param	data printable array
@param	length require length of data
@return function returns nothing
****************************************************************/
void lcd_writeln(unsigned char command, const char *data,
unsigned char length)
{
	while(length --> 0)
	lcd_write(command++, *data++);
}

/***************************************************************
@brief	same as lcd display write mode but bunch of char
@param	command lcd display cursor address
@param	data printable data that holds on
@return	function returns nothing
****************************************************************/
void lcd_disp(unsigned char command, const char *data)
{
	while(*data)
	lcd_write(command++, *data++);
}

/**************************************************************
@brief	display a decimal charactor on lcd display
@param	command lcd display cursor address
@param	data decimal data
@param 	length require length of data
@param	base number conversion base
@return	function returns nothing
***************************************************************/
void lcd_decimal(unsigned char command, unsigned long data, 
int length, int base)
{
	unsigned char localarray[10];
	dtoa(data, (char*)localarray, length, base);
	lcd_disp(command, (char*)localarray);
}

/**************************************************************
@brief	display a float charactor on lcd display
@param	command lcd display cursor address
@param	data float point data
@param	postfix integral data length
@param	prefix decimal data length
@return	function returns nothing
**************************************************************/
void lcd_float(unsigned char command, float data, int postfix,
int prefix)
{
	unsigned char localarray[15];
	ftostra(data, postfix, prefix, (char*)localarray);
	lcd_disp(command, (char*)localarray);
}

/*************************************************************
@brief	display a data on lcd using printf style syntax
@param	format that holds va_arg 
@param	... arguments (mostly compatible with printf)
@return	function returns nothing
**************************************************************/
void lcd_print(unsigned char command, const char *format, ...)
{
	va_list(arg);
	va_start(arg, format);
	print(format, arg);
	va_end(arg);
	lcd_disp(command, printablearray);
	
}	

/*************************************************************
@brief	create custom char on cgram location
@param	location cgram location (0 - 7)
@param	data custom char data
@return	function returns nothing
**************************************************************/
void lcd_custom(unsigned char location, const char *data)
{
	location *= 8;
	for(char k = 0; k < 8; k++)
	lcd_write((CGRAM_ADDRESS + location + k), data[k]);
}

/************************************************************
@brief	same as customchar but this will display instantly
@param	command	lcd display cursor address
@param	location cgram location (0 - 7)
@param	data custom char data
@return	function returns nothing
************************************************************/
void lcd_char(unsigned char command, unsigned char location, 
const char *data)
{
	lcd_custom(location, data);
	lcd_write(command, location);
}

/**********************************************************
@brief	allort a data into lcd for 4bit mode
@param	data that holds write data
@return	function returns nothing
***********************************************************/
void lcd_allort(unsigned char data)
{
	gpio_put(pinconfig.D4, (data & 0x10 ?HIGH :LOW));
	gpio_put(pinconfig.D5, (data & 0x20 ?HIGH :LOW));
	gpio_put(pinconfig.D6, (data & 0x40 ?HIGH :LOW));
	gpio_put(pinconfig.D7, (data & 0x80 ?HIGH :LOW));
}

/**********************************************************
@brief	enable lcd display to intimate data ready to read
@param	- no parameters performed here
@return	function returns nothing
***********************************************************/
void lcd_enable(void)
{
	gpio_put_low(pinconfig.EN); DelayUS(5);
	gpio_put_high(pinconfig.EN); DelayUS(5);
	gpio_put_low(pinconfig.EN); DelayUS(100);
}	
