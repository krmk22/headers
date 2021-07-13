#include <htc.h>
#include <string.h>

#ifndef _GPIO_H
#include "gpio.h"
#endif

#ifndef _UTILS_H
#include "utils.h"
#endif

#ifndef _UART_H
#include "uart.h"
#endif

#include "esp8266.h"

PIC_UART_CONFIG *esp;

/*****************************************************************
@brief	initialize esp8266 module (serial registers)
@param	uart type of uart either uart0 or uart
@param	speed communication baudrate speed
@return	function returns nothing
******************************************************************/
void esp_initialize(PIC_UART_CONFIG *uart, unsigned long speed)
{
	esp = uart;
	serial_initialize(esp, speed);
	serial_flush(esp);
	serial_enable(esp, true);
}

/*****************************************************************
@brief	check esp module found or not
@param	- no parameters performed here
@return	true if module found
*****************************************************************/
char esp_found(void)
{
	serial_write(esp, "AT");
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}	
		
/****************************************************************
@brief	change echo mode 
@param	state true/false
@return	true if command executed
*****************************************************************/
char esp_echo(unsigned char state)
{
	serial_write(esp, "ATE");
	serial_send(esp, state ?'1' :'0');
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}

/****************************************************************
@brief	resetting the esp8266 module
@param	- no parameters performed here
@return	true if command executed
****************************************************************/
char esp_reset(void)
{
	serial_write(esp, "AT+RST");
	serial_flush(esp);
	char returnvariable = serial_talkback(esp, true, "ready", 5, 2500);
	
	DelayMS(1500);
	serial_talkback(esp, false, "WIFI CONNECTED", 14, 5000);
	serial_talkback(esp, false, "WIFI GOT IP", 11, 5000);
	
	return returnvariable;
}

/**************************************************************
@brief	enter sleep mode
@param	state indicated the type of sleep mode
@return	true if command executed
***************************************************************/
char esp_sleep(unsigned char state)
{
	switch(state)
	{
		case WAKEUP: serial_write(esp, "AT+SLEEP=0"); break;
		case LIGHT: serial_write(esp, "AT+SLEEP=1"); break;
		case MODEM: serial_write(esp, "AT+SLEEP=2"); break;
		case DEEP: serial_write(esp, "AT+GSLP=100"); break;
		default: break;
	}
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}

/*************************************************************
@brief	configuring wifi connection mode
@param	state indicates the wifi mode
@return	true if command executed
**************************************************************/
char esp_mode(unsigned char state)
{
	serial_write(esp, "AT+CWMODE=");
	serial_send(esp, state + '0');
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}	

/************************************************************
@brief	connect wifi using given parameters
@param	ssid name of the wifi network
@param	pass presharedkey security key 
@return true if wifi connected
************************************************************/
char esp_connect(const char *ssid, const char *pass)
{
	serial_write(esp, "AT+CWJAP?");
	serial_flush(esp);
	if(serial_talkback(esp, true, ssid, strlen(ssid), 5000))
	return true; //wifi connection was already established
	
	serial_print(esp, "AT+CWLAP=\"%s\"", ssid);
	serial_flush(esp);
	if(!serial_talkback(esp, true, ssid, strlen(ssid), 5000)) 
	return false; //no wifi signal found from given ssid
	
	serial_print(esp, "AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 25000);
}	

/********************************************************************
@brief	disconnect wifi (if connected)
@param	- no parameters performed here
@return	true if wifi disconnected
********************************************************************/
char esp_disconnect(void)
{
	serial_write(esp, "AT+CWQAP");
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 5000);
}

/*******************************************************************
@brief	autoconnect wifi when power on or reset
@param	state true/flase for autoconnect
@return	true if command executed
********************************************************************/
char esp_autoconnect(unsigned char state)
{
	serial_write(esp, "AT+CWAUTOCONN=");
	serial_send(esp, state + '0');
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}	

/*******************************************************************
@brief	enable/disable multiplex server connection
@param	state true/false for multiplex connection
@return	true if command executed
*******************************************************************/
char esp_multiplex(unsigned char state)
{
	serial_write(esp, "AT+CIPMUX=");
	serial_send(esp, state + '0');
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}

/******************************************************************
@brief	start a new server connection
@param	id connection number (0 - 4)
@param	level communication protocol
@param	server server name of fixed ip
@param	port server communication port
@return true if server connection started
******************************************************************/
char esp_start(unsigned char id, unsigned char level, const char *server, const char *port)
{
	serial_write(esp, "AT+CIPSTART=");
	serial_send(esp, id + '0');
	serial_send(esp, ',');
	
	switch(level)
	{
		case MQTT: case HTTP: case TCP:
		serial_write(esp, "\"TCP\",\""); break;
		case UDP:
		serial_write(esp, "\"UDP\",\""); break;
		case SSL:
		serial_write(esp, "\"SSL\",\""); break;
		default: break;
	}
	
	serial_writeln(esp, server, strlen(server));
	serial_write(esp, "\",");
	serial_writeln(esp, port, strlen(port));
	serial_flush(esp);
	return serial_talkback(esp, true, "CONNECT", 7, 10000);
}

/*********************************************************************
@brief	modify SSL buffer size 
@param	size require buffer size for ssl (2048 - 4096)
@return	true if command executed
@note	current esp8266 module supports only one ssl at a time
**********************************************************************/
char esp_size(unsigned long size)
{
	serial_write(esp, "AT+CIPSSLSIZE=");
	serial_decimal(esp, size, 4, DEC);
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}	

/********************************************************************
@brief	initiate to send chars on esp module
@param	id connection id number
@param	length require data length to send
@return	true if esp is ready to send data
*********************************************************************/
char esp_send(unsigned char id, unsigned char length)
{
	serial_write(esp, "AT+CIPSEND=");
	serial_send(esp, id + '0');
	serial_send(esp, ',');
	serial_decimal(esp, length, 3, DEC);
	serial_flush(esp);
	return serial_talkback(esp, true, ">", 1, 2500);
}

/********************************************************************
@brief	closer particular id server connection
@param	id that need to close 
@return true if connection closed
@note	if id is 5 that will close all connections
*********************************************************************/
char esp_close(unsigned char id)
{
	serial_write(esp, "AT+CIPCLOSE=");
	serial_send(esp, id + '0');
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}

/********************************************************************
@brief	set timeout period for tcp server
@param	time require timeout period in second (0 - 7200)
@retrun	true if command executed
********************************************************************/
char esp_timeout(unsigned long time)
{
	serial_write(esp, "AT+CIPSTO=");
	serial_decimal(esp, time, 4, DEC);
	serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}

/******************************************************************
@brief	check out server connection alive status by pinging
@param	server require server name or fixed ip
@return	true if connection was alive
*******************************************************************/
char esp_ping(const char *server)
{
	serial_write(esp, "AT+PING=\"");
	serial_write(esp, server);
	serial_send(esp, '"'); serial_flush(esp);
	return serial_talkback(esp, true, "OK", 2, 2500);
}		
