#include <htc.h>

#ifndef _UTILS_H
#include "utils.h"
#endif

#ifndef _INT_H
#include "int.h"
#endif

#include "timer.h"

volatile unsigned long systemtick;
volatile unsigned int timervalue;

/**************************************************************
@brief	timer0 isr routine
@param	systemtick will affected here
@return	function return nothing
***************************************************************/
void ontick()
{	
	TMR1ON = false; 
	WRITETIMER1(timervalue);
	systemtick++;
	TMR1ON = true;
}	

/**************************************************************
@brief	this function will initialze timer0 for accurate delay
@param	- no parameters performed here
@return	function returns nothing
***************************************************************/
void timer_init_all()
{
	timervalue = 65535 - ((unsigned)(_XTAL_FREQ * 0.05F) / (8 * 4)); //50ms isr callback 
	
	T1RD16 = 1; //enable 16bit timer mode
	T1CKPS0 = 1; T1CKPS1 = 1; //enable 1:8 prescale
	T1OSCEN = 0; //enable timer1 oscillator 32khz
	T1SYNC = TMR1CS = 0; //internal clock (fosc / 4)
	
	WRITETIMER1(timervalue);
	enableinterrupt(TIMER1, ontick);
	activateinterrupt();
	TMR1IE = TMR1ON = true;
}	
	
/**************************************************************
@brief	return absolute milli seconds from timer initialized
@param	- no parmeters performed here
@return	function returns milliseconds
***************************************************************/
unsigned long millis()
{	
	TMR1ON = false; unsigned int timer1 = READTIMER1(); TMR1ON = true;
	return ((unsigned)((timer1 - timervalue) / (1.25F * 1000)) + (systemtick * 50));
}

/**************************************************************
@brief	return absolute micro seconds from timer initialized
@param	- no parameters performed here
@return	function returns microseconds
***************************************************************/
unsigned long micros()
{
	TMR1ON = false; unsigned int timer1 = READTIMER1(); TMR1ON = true;
	return ((unsigned)((timer1 - timervalue) / 1.25F) + (systemtick * 50000)) + 50;
}	
