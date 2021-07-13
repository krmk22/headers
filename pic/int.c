#include <htc.h>

#include "int.h"

/***************************************************************
@brief	interrupt service routine for pic microcontrollers
@param	- No parameters performed here
@return	function return nothing
****************************************************************/
static void interrupt ISR(void)
{
	if(TMR0IE && TMR0IF)
	{
		TIMER0_ISR();
		TMR0IF = 0;
		return ;
	}	
	if(TMR1IE && TMR1IF) 
	{
		TIMER1_ISR();
		TMR1IF = 0;
		return ;
	}	
	if(TMR2IE && TMR2IF)
	{
		TIMER2_ISR();
		TMR2IF = 0;
		return ;
	}	
	if(TMR3IE && TMR3IF)
	{
		TIMER3_ISR();
		TMR3IF = 0;
		return ;
	}	
	
	if(INT0IE && INT0IF)
	{
		EXTIN0_ISR();
		INT0IF = 0;
		return ;
	}	
	if(INT1IE && INT1IF)
	{
		EXTIN1_ISR();
		INT1IF = 0;
		return ;
	}	
	if(INT2IE && INT2IF)
	{
		EXTIN2_ISR();
		INT2IF = 0;
		return ;
	}	
	
	if(RCIE && RCIF)
	{
		RXRINT_ISR();
		RCIF = 0;
		return ;
	}	
	if(RBIF && RBIE)
	{
		PORTCH_ISR();
		RBIE = 0;
		return ;
	}	
	
	if(CCP1IE && CCP1IF)
	{
		CP1INT_ISR();
		CCP1IF = 0;
		return ;
	}	
	if(CCP2IE && CCP2IF)
	{
		CP2INT_ISR();
		CCP2IF = 0;
		return ;
	}	
}	

/**************************************************************
@brief	enableinterrupt with type and callback function
@param	type interrupt type
@param	function call back function
@return	function return nothing
***************************************************************/
void enableinterrupt(int type, void *function)
{
	switch(type)
	{
		case TIMER0: TIMER0_ISR = function; break;
		case TIMER1: TIMER1_ISR = function; break;
		case TIMER2: TIMER2_ISR = function; break;
		case TIMER3: TIMER3_ISR = function; break;
		
		case EXTIN0: EXTIN0_ISR = function; break;
		case EXTIN1: EXTIN1_ISR = function; break;
		case EXTIN2: EXTIN2_ISR = function; break;
		
		case RXRINT: RXRINT_ISR = function; break;
		case PORTCH: PORTCH_ISR = function; break;
		
		case CP1INT: CP1INT_ISR = function; break;
		case CP2INT: CP2INT_ISR = function; break;
		
		default: break;
	}
}	

/**************************************************************
@brief	disable interrupt (if enabled)
@param	type interrupt type
@return	function returns nothing
***************************************************************/
void disableinterrupt(int type)
{
	switch(type)
	{
		case TIMER0: TIMER0_ISR = '\0'; break;
		case TIMER1: TIMER1_ISR = '\0'; break;
		case TIMER2: TIMER2_ISR = '\0'; break;
		case TIMER3: TIMER3_ISR = '\0'; break;
		
		case EXTIN0: EXTIN0_ISR = '\0'; break;
		case EXTIN1: EXTIN1_ISR = '\0'; break;
		case EXTIN2: EXTIN2_ISR = '\0'; break;
		
		case RXRINT: RXRINT_ISR = '\0'; break;
		case PORTCH: PORTCH_ISR = '\0'; break;
		
		case CP1INT: CP1INT_ISR = '\0'; break;
		case CP2INT: CP2INT_ISR = '\0'; break;
		
		default: break;
	}
}	
