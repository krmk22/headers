#ifndef _GPIO_H
#define _GPIO_H

#pragma warning disable 967

/***************************************************/
/********PORT ALLOCATION INTO VARIABLE**************/
/***************************************************/
#define portstate	&PORTA 
#define trisstate	&TRISA
#define pinstate	&LATA

/***************************************************/
/************TYPEDEFINED ENUMERATION****************/
/***************************************************/
typedef enum
{
	LOW = 0,
	HIGH = 1,
	OUTPUT = 0,
	INPUT = 1,
	TOGGLE
}
PIC_PIN_CONDITION; 

/***************************************************/
/*****WE NEED TO MODIFY PIN NAMES TO ACCESS HERE****/
/***************************************************/
typedef enum
{
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
	PE0, PE1, PE2, PE3
}
PIC_PORT_GPIO_PINS;

/***************************************************/
/*****SOME PREDEFINED FUNCTIONS TO FAST ACCESS******/
/***************************************************/
#define gpio_set_output(pin) 			(*(trisstate + (pin >> 3)) &=~(1 << (pin & 0x07)))
#define gpio_set_input(pin)			(*(trisstate + (pin >> 3)) |= (1 << (pin & 0x07)))
#define gpio_set_toggle(pin)			(*(trisstate + (pin >> 3)) ^= (1 << (pin & 0x07)))
#define gpio_set_output_all(pin)		(*(trisstate + (pin >> 3)) &=~0xFF)
#define gpio_set_input_all(pin)			(*(trisstate + (pin >> 3)) |= 0xFF)
#define gpio_set_toggle_all(pin)		(*(trisstate + (pin >> 3)) ^= 0xFF)

#define gpio_put_high(pin)			(*(pinstate + (pin >> 3)) |= (1 << (pin & 0x07)))
#define gpio_put_low(pin)			(*(pinstate + (pin >> 3)) &=~(1 << (pin & 0x07)))
#define gpio_put_toggle(pin)			(*(pinstate + (pin >> 3)) ^= (1 << (pin & 0x07)))
#define gpio_put_high_all(pin)			(*(pinstate + (pin >> 3)) |= 0xFF)
#define gpio_put_low_all(pin)			(*(pinstate + (pin >> 3)) &=~0xFF)
#define gpio_put_toggle_all(pin)		(*(pinstate + (pin >> 3)) ^= 0xFF)

#define gpio_get(pin)				(*(portstate + (pin >> 3)) & (1 << (pin & 0x07)))
#define gpio_get_all(pin)			(*(portstate + (pin >> 3)))

/**********************************************************************
@brief	set gpio direction of pic i/o port pin
@param	pin that contains number of pin 
@param	state requires input, output, toggle
@return	function returns nothing
**********************************************************************/
static void gpio_set(unsigned char pin, unsigned char state)
{
	switch(state)
	{
		case INPUT: gpio_set_input(pin); break;
		case OUTPUT: gpio_set_output(pin); break;
		case TOGGLE: gpio_set_toggle(pin); break;
		default: break;
	}
}

/*********************************************************************
@brief	set gpio direction of pic particular port
@param  pin that contains number of pin for identify port
@param	state requires input, output, toggle
@return function returns nothing
**********************************************************************/
static void gpio_set_all(unsigned char pin, unsigned char state)
{
	switch(state)
	{
		case INPUT: gpio_set_input_all(pin); break;
		case OUTPUT: gpio_set_output_all(pin); break;
		case TOGGLE: gpio_set_toggle_all(pin); break;
		default: break;
	}
}

/********************************************************************
@brief	put gpio high/low of pic particular port pin
@param	pin that contains number of pin
@param	state requires high, low, toggle
@return	function returns nothing
*********************************************************************/
static void gpio_put(unsigned char pin, unsigned char state)
{
	switch(state)  
	{
		case HIGH: gpio_put_high(pin); break;
		case LOW: gpio_put_low(pin); break;
		case TOGGLE: gpio_put_toggle(pin); break;
		default: break;
	}
}

/*******************************************************************
@brief	put gpio high/low of pic particular port
@param	pin that contains number of pin to identify port
@param	state requires high, low, toggle
@return	function returns nothing
********************************************************************/
static void gpio_put_all(unsigned char pin, unsigned char state)
{
	switch(state)
	{
		case HIGH: gpio_put_high_all(pin); break;
		case LOW: gpio_put_low_all(pin); break;
		case TOGGLE: gpio_put_toggle_all(pin); break;
		default: break;
	}
}
#endif						
