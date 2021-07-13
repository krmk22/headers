#include <htc.h>
#include <string.h>
#include <stdarg.h>

#ifndef _GPIO_H
#include "gpio.h"
#endif

#ifndef _UTILS_H
#include "utils.h"
#endif

#ifndef _INT_H
#include "int.h"
#endif

#ifndef _PRINT_H
#include "print.h"
#endif

#include "uart.h"

#define SerialUS(Delay)\
{\
	unsigned int ReqDelay = Delay;\
	while(--ReqDelay) asm("nop");\
}	

static unsigned int FCallDelay;
static unsigned int FIntDelay;
static unsigned int BasicDelay;
static unsigned int FStopDelay;
static unsigned int FLoopDelay;
static unsigned int BitPerUS;

/************************************************************
@brief	default and main isr routine for uart0 interrupt
@param	- no parameters performed here
@retrun	isr function returns nothing
*************************************************************/
void serialuart0routine()
{
	uart0.array[uart0.count] = serial_receive(&uart0);
	uart0.count = (uart0.count < 100 ?uart0.count + 1 :0);
}

/***********************************************************
@brief	default and main isr routine for uart1 interrupt
@param	- no parmeters performed here
@return	isr function returns nothing
************************************************************/
void serialuart1routine()
{
	uart1.array[uart1.count] = serial_receive(&uart1);
	uart1.count = (uart1.count < 100 ?uart1.count + 1 :0);
}

/***********************************************************
@brief	initializing serial registers along with functions
@param	uart either uart0 or uart1
@param	speed uart communication baudrate
@return	function returns nothing
@note 	while using uart1 ccp0 will disable permanently
************************************************************/
void serial_initialize(PIC_UART_CONFIG *uart, unsigned long speed)
{
	gpio_set_input(uart -> receiver);
	gpio_set_output(uart -> transmitter);
	gpio_put_high(uart -> transmitter);
	
	serial_baudrate(uart, speed);
	
	if(uart -> id)
	{
		CCP2CON = (1 << 2); CCP2IF = 0;
		enableinterrupt(CP2INT, serialuart1routine);
	}
	else
	{
		TX9 = SYNC = TX9D = 0;
		BRGH = SPEN = 1;
		RX9 = ADDEN = 0;
		BRG16 = TXEN = 1;
		ADDEN = RCIDL = WUE = 0;
		enableinterrupt(RXRINT, serialuart0routine);
	}
}		

/***************************************************************************
@brief	make a new call back function by disabling default one for ISR
@param	uart type of uart either uart0 or uart
@param	function that where isr will be executed
@return	function returns nothing
@note	user must care about interrupt functions to disable default ISR
***************************************************************************/
void serial_callback(PIC_UART_CONFIG *uart, void *function)
{
	deactivateinterrupt();
	if(uart -> id) 
	{
		disableinterrupt(CP2INT);
		enableinterrupt(CP2INT, function);
	}
	else
	{
		disableinterrupt(RXRINT);
		enableinterrupt(RXRINT, function);
	}
	activateinterrupt();
}

/*************************************************************************
@param	function will calculate register and timing delay for require baud
@param	uart type of uart either uart0 or uart1
@param	speed require baudrate speed valid from (120..115200)
@return	function returns nothing
**************************************************************************/
void serial_baudrate(PIC_UART_CONFIG *uart, unsigned long speed)
{
	uart -> speed = speed;
	if(uart -> id)
	{
		BitPerUS = 1000000UL / speed;
		FCallDelay = (BitPerUS - (unsigned)(10 * OneMachineCycle));
		FIntDelay = (BitPerUS - (unsigned)(10 * OneMachineCycle));
		BasicDelay = (unsigned)(2 * OneMachineCycle);
		FLoopDelay = FStopDelay = BitPerUS;
	}
	else
	{
		unsigned long finalbaud = (((_XTAL_FREQ / (speed * 4))) - 1);
		SPBRGH = (unsigned int)((finalbaud >> 8) & 0xFF);
		SPBRG = (unsigned int)(finalbaud & 0xFF);
	}
}	

/***********************************************************************
@brief	send a single charactor on serial
@param	uart type of uart either uart0 or uart1
@param	data require char that need to send on serial
@return	function returns nothing
************************************************************************/
void serial_send(PIC_UART_CONFIG *uart, unsigned char data)
{
	if(uart -> id)
	{
		deactivateinterrupt();
		gpio_put_low(uart -> transmitter);
		SerialUS((unsigned)(FCallDelay - BasicDelay));
		
		for(unsigned char shifting = 0x01; shifting; shifting <<= 1)
		{
			if(data & shifting) 
			gpio_put_high(uart -> transmitter);
			else gpio_put_low(uart -> transmitter);
			SerialUS((unsigned)(FLoopDelay));
		}
		
		gpio_put_high(uart -> transmitter);
		activateinterrupt();
		SerialUS((unsigned)(FStopDelay - BasicDelay));
	}
	else
	{
		while(!TRMT) continue;
		TXREG = data;
	}
}		
				
/***********************************************************************
@brief	write string charactor on serial line
@param	uart type of uart either uart0 or uart1
@param	data that need to be send on serial
@return	function returns nothing
************************************************************************/
void serial_write(PIC_UART_CONFIG *uart, const char *data)
{
	while(*data) 
	serial_send(uart, *data++);
}

/***********************************************************************
@brief	same as write but with a particular length
@param	uart type of uart either uart0 or uart1
@param	data that need to be send on serial
@param	length data length
@return	function returns nothing
***********************************************************************/
void serial_writeln(PIC_UART_CONFIG *uart, const char *data, unsigned int length)
{
	while(length --> 0) 	
	serial_send(uart, *data++);
}

/*********************************************************************
@brief	print a decimal number on serial line (ascii conversion)
@param	uart type of uart either uart0 or uart1
@param	data decimal data 
@param 	length require length of charactor
@param	base number conversion base (DEC, OCT, BIN, HEX)
@return	function returns nothing
*********************************************************************/
void serial_decimal(PIC_UART_CONFIG *uart, unsigned long data, int length, int base)
{
	unsigned char localarray[12];
	dtoa(data, (char*)localarray, length, base);
	serial_write(uart, localarray);
}

/********************************************************************
@brief	print a float number on serial line (ascii conversion)
@param	uart type of uart either uart0 or uart1
@param	data float point data
@param	postfix integer data length
@param	prefix decimal data length
@return	function returns nothing
*********************************************************************/
void serial_float(PIC_UART_CONFIG *uart, float data, int postfix, int prefix)
{
	unsigned char localarray[12];
	ftostra(data, postfix, prefix, (char*)localarray);
	serial_write(uart, localarray);
}

/*******************************************************************
@brief	write a data on serial line (printf style function)
@param	uart type of uart either uart0 or uart1
@param	function that holds string 
@param	... holds argument lists
@return	function returns nothing
********************************************************************/
void serial_print(PIC_UART_CONFIG *uart, const char *function, ...)
{
	va_list(arg);
	va_start(arg, function);
	print(function, arg);
	va_end(arg);
	serial_write(uart, printablearray);
}	

/*******************************************************************
@brief	flush out particular uart array
@param	uart type of uart either uart0 or uart1
@return	function returns nothing
********************************************************************/
void serial_flush(PIC_UART_CONFIG *uart)
{
	memset(uart -> array, '\0', sizeof(uart -> array));
	uart -> count = 0;
}	

/*******************************************************************
@brief	enabling/disabline serial receive interrupt
@param	uart type of uart either uart0 or uart1
@param	state enable/disable uart function
@return	function returns nothing
********************************************************************/
void serial_enable(PIC_UART_CONFIG *uart, unsigned char state)
{
	if(uart -> id)
	{
		CCP2IE = state;
	}
	else
	{
		RCIE = state;
		CREN = state;
	}
	activateinterrupt();
}

/*******************************************************************
@brief	check any new charactor avaialble from serial line
@param	uart type of uart either uart0 or uart1
@return	function returns received char from serial line
********************************************************************/
unsigned char serial_receive(PIC_UART_CONFIG *uart)
{
	unsigned char returnvariable = 0x00;
	
	if(uart -> id)
	{
		if(gpio_get(uart -> receiver)) return returnvariable;
		
		for(unsigned char shifting = 0x01; shifting; shifting <<= 1)
		{
			SerialUS((unsigned)FIntDelay);
			if(gpio_get(uart -> receiver)) 
			returnvariable |= shifting;
			else returnvariable &=~ shifting;
		}
		
		SerialUS((unsigned)FStopDelay);
	}
	else
	{
		while(!RCIF) continue;
		returnvariable = RCREG;
	}
	
	return returnvariable;	
}

/*************************************************************************
@brief	check serial receives given string with in timelimit
@param	uart type of uart either uart0 or uart1
@param	newline if true new line will be printed on serial line
@param	expdata expected string data
@param	length expdata string length
@param	timedelay require timelimit to discard process
***************************************************************************/
unsigned char serial_talkback(PIC_UART_CONFIG *uart, int newline, 
const char *expdata, unsigned char length, unsigned int timedelay)
{
	if(newline) serial_write(uart, "\r\n");
	
	do
	{
		if(smemmem((char*)uart -> array, uart -> count, 
		expdata, length) != '\0') return true;
		if(smemmem((char*)uart -> array, uart -> count,
		"ERROR", 5) != '\0') return false;
		DelayMS(1);
	}
	while(timedelay --> 0);
	return false;
}		
