#include <htc.h>
#include <string.h>

#ifndef _GPIO_H
#include "gpio.h"
#endif

#ifndef _UTILS_H
#include "utils.h"
#endif

#include "i2c.h"

/******************************************************************
@brief	polling a start condition on i2c line
@param	i2c type of i2c either i2c0 or i2c1
@return	function returns nothing
*******************************************************************/
void i2c_start(PIC_I2C_CONFIG *i2c)
{
	if(i2c -> type)
	{
		gpio_put_high(i2c -> scl);
		gpio_put_high(i2c -> sda);
		DelayUS(BusFreeTime);
		gpio_put_low(i2c -> sda);
		DelayUS(StartHoldTime);
		gpio_put_low(i2c -> scl);
	}
	else
	{
		SEN = 1;
		while(SEN);
	}
}

/*******************************************************************
@brief	polling a stop condition on i2c line
@param	i2c type of i2c either i2c0 or i2c1
@return	function returns nothing
********************************************************************/
void i2c_stop(PIC_I2C_CONFIG *i2c)
{
	if(i2c -> type)
	{	
		gpio_put_low(i2c -> sda);
		DelayUS(BusFreeTime);
		gpio_put_high(i2c -> scl);
		DelayUS(BusFreeTime);
		gpio_put_low(i2c -> sda);
		DelayUS(StopHoldTime);
	}
	else
	{
		PEN = 1;
		while(PEN);
	}
}

/********************************************************************
@brief	polling a restart condition on i2c line
@param	i2c type of i2c either i2c0 or i2c1
@return	function returns nothing
*********************************************************************/
void i2c_restart(PIC_I2C_CONFIG *i2c)
{
	if(i2c -> type)
	{
		gpio_put_high(i2c -> sda);
		gpio_put_high(i2c -> scl);
		DelayUS(BusFreeTime);
		gpio_put_low(i2c -> sda);
		DelayUS(RestartHoldTime);
		gpio_put_low(i2c -> scl);
	}
	else
	{
		RSEN = 1;
		while(RSEN);
	}
}

/**********************************************************************
@brief	polling clock cycle for i2c line
@param	i2c type of i2c either i2c0 or i2c1
@return	function returns nothing
@note	this function used when bitbanging i2c processed
***********************************************************************/
void i2c_clock(PIC_I2C_CONFIG *i2c)
{
	DelayUS(DataHoldTime);
	gpio_put_high(i2c -> scl);
	DelayUS(ClockHighTime);
	gpio_put_low(i2c -> scl);
	DelayUS(ClockLowTime);
	DelayUS(DataSetupTime);
}

/***********************************************************************
@brief	set i2c address for multiple device communication
@param	i2c type of i2c either i2c0 or i2c1
@param	address modified i2c address
@return	function returns nothing
************************************************************************/
void i2c_setaddress(PIC_I2C_CONFIG *i2c, unsigned char address)
{
	i2c -> address = address;
}

/***********************************************************************
@brief	set i2c gpio lines
@param	i2c type of i2c either i2c0 or i2c1
@param	scl gpio pin of serial clock
@param	sda gpio pin of serial data
@return	function returns nothing
@note	while using i2c0 use default pin numbers
************************************************************************/
void i2c_setgpio(PIC_I2C_CONFIG *i2c, unsigned char scl, unsigned char sda)
{
	if(i2c -> type)
	{
		i2c -> scl = scl;
		i2c -> sda = sda;
	}
	else
	{
		i2c -> scl = DEFAULT_I2C_SCL;
		i2c -> sda = DEFAULT_I2C_SDA;
	}
}

/***********************************************************************
@brief	set i2c speed in khz
@param	i2c type of i2c either i2c0 or i2c1
@param	speed require bus speed for i2c lines in khz
@return	function returns nothing
@note	i2c1 has default 100khz speed 
************************************************************************/
void i2c_setspeed(PIC_I2C_CONFIG *i2c, unsigned int speed)
{
	if(i2c -> type) return ; //i2c1 has default 100khz speed
	i2c -> speed = speed;
	unsigned int regvalue = (_XTAL_FREQ / ((speed * 1000) * 4UL)) - 1;
	SMP = (speed > 100 ?0 : 1);
	SSPADD = regvalue;
}

/***********************************************************************
@brief	initialize i2c module for communication
@param	i2c type of i2c either i2c0 or i2c1
@return	function returns nothing
************************************************************************/
void i2c_initialize(PIC_I2C_CONFIG *i2c)
{
	if(i2c -> type)
	{
		gpio_set_output(i2c -> scl);
		gpio_set_output(i2c -> sda);
	}
	else
	{
		gpio_set_input(i2c -> scl);
		gpio_set_input(i2c -> sda);
		SSPSTAT = (1 << 7); //slewrate
		SSPEN = 1; //enable ssp module
		SSPM3 = 1; SSPM2 = SSPM1 = SSPM0 = 0; //i2c 7bit master
		SSPCON2 = 0; //clear polling registers
		i2c_setspeed(i2c, i2c -> speed);
		SSPIF = 0; //clear interrupt flag
	}
}

/********************************************************************
@brief	write a data on i2c line
@param 	i2c type of i2c either i2c0 or i2c1
@return	function returns acknowledge status
*********************************************************************/
unsigned char i2c_write(PIC_I2C_CONFIG *i2c, unsigned char data)
{
	unsigned char ackstate;
	
	if(i2c -> type)
	{
		for(unsigned char shifting = 0x80; shifting; shifting >>= 1)
		{
			if(shifting & data) 
			gpio_put_high(i2c -> sda);
			else gpio_put_low(i2c -> sda);
			i2c_clock(i2c);
		}
		gpio_set_input(i2c -> sda);
		i2c_clock(i2c);
		ackstate = gpio_get(i2c -> sda);
		gpio_set_output(i2c -> sda);
	}
	else
	{
		SSPBUF = data;
		while(!SSPIF); SSPIF = 0;
		ackstate = ACKSTAT; ACKEN = 1; while(ACKEN);
		while(SSPSTAT & (1 << 2));
	}
	
	return ackstate;
}	

/*******************************************************************************
@brief	read data from i2c line
@param	i2c type of i2c either i2c0 or i2c1
@param	ack varaible that has ack state put one for sequenctial write
@return	retrive value from i2c line
********************************************************************************/
unsigned char i2c_read(PIC_I2C_CONFIG *i2c, unsigned char ack)
{
	unsigned char returnvariable;
	
	if(i2c -> type)
	{
		gpio_set_input(i2c -> sda);
		for(unsigned char shifting = 0x80; shifting; shifting >>= 1)
		{
			if(gpio_get(i2c -> sda)) 
			returnvariable |= shifting;
			else returnvariable &=~ shifting;
			i2c_clock(i2c);
		}
		gpio_set_output(i2c -> sda);
		gpio_put(i2c -> sda, (ack ?HIGH :LOW));
		i2c_clock(i2c);
	}
	else
	{
		RCEN = 1; //enable i2c receive
		while(!BF) continue; //check buffer has data
		returnvariable = SSPBUF; //store buffer into varaible
		ACKDT = (ack ?1 :0); ACKEN = 1; while(ACKEN); //wait till ack
		while(!SSPIF); SSPIF = 0; RCEN = 0;//clear interrupt flag
	}
	
	return returnvariable;
}						

/********************************************************************************
@brief	write data on i2c line with slave register and value
@param	i2c type of i2c either i2c0 or i2c1
@param	address slave register address
@param	data require data to be modified on slave register
@return	function return true if data written successfully
*********************************************************************************/
unsigned char i2c_register_write(PIC_I2C_CONFIG *i2c, unsigned char address, unsigned char data)
{
	unsigned char ackstate;
	i2c_start(i2c);
	ackstate = i2c_write(i2c, i2c -> address);
	ackstate = i2c_write(i2c, address);
	ackstate = i2c_write(i2c, data);
	i2c_stop(i2c);
	return ackstate;
}

/********************************************************************************
@brief	read data from slave register on i2c line
@param	i2c type of i2c either i2c0 or i2c1
@param	address slave register address
@return	function return retrieved register value
********************************************************************************/
unsigned char i2c_register_read(PIC_I2C_CONFIG *i2c, unsigned char address)
{
	unsigned char returnvariable;
	i2c_start(i2c);
	i2c_write(i2c, i2c -> address);
	i2c_write(i2c, address);
	i2c_restart(i2c);
	i2c_write(i2c, i2c -> address | 1);
	returnvariable = i2c_read(i2c, true);	
	i2c_stop(i2c);
	return returnvariable;
}

/********************************************************************************
@brief	write sequence of data on i2c line
@param	i2c type of i2c either i2c0 or i2c1
@param	address slave register write start address
@param	array that holds the write data
@param	length maximum require data
@return	function returns nothing
**********************************************************************************/
void i2c_sequence_write(PIC_I2C_CONFIG *i2c, unsigned char address, unsigned char *array, unsigned char length)
{
	i2c_start(i2c);
	i2c_write(i2c, i2c -> address);
	i2c_write(i2c, address);
	while(length --> 0) i2c_write(i2c, *array++);
	i2c_stop(i2c);
}

/*********************************************************************************
@brief	read sequence of data from i2c line
@param	i2c type of i2c either i2c0 or i2c1
@param	address slave register read start address
@param	array that holds the retrive data
@param	length require data length
@return	function returns nothing
**********************************************************************************/
void i2c_sequence_read(PIC_I2C_CONFIG *i2c, unsigned char address, unsigned char *array, unsigned char length)
{
	unsigned char localvariable;
	memset(array, '\0', sizeof(array)); //we don't know that the array have values
	i2c_start(i2c);
	i2c_write(i2c, i2c -> address);
	i2c_write(i2c, address);
	i2c_restart(i2c);
	i2c_write(i2c, i2c -> address | 1);
	while(length --> 0) *array++ = i2c_read(i2c, (length ?false :true));
	i2c_stop(i2c);
}	
