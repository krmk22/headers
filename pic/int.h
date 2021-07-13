#ifndef _INT_H
#define _INT_H

/**************************************************************/
/*********ENABLE / DISABLE INTERRUPT PREDEFINE FUNCTION********/
/**************************************************************/
#define activateinterrupt()  	GIE = PEIE = 1
#define deactivateinterrupt()	GIE = PEIE = 0

/**************************************************************/
/****************ENUMURATION INTERRUPT NAMES*******************/
/**************************************************************/
typedef enum
{
	TIMER0, 
	TIMER1, 
	TIMER2,
	TIMER3,
	EXTIN0, 
	EXTIN1,
	EXTIN2,
	EXTIN3,
	RXRINT,
	PORTCH,
	CP1INT, 
	CP2INT
}
PIC_ISR_LIST;

/**************************************************************/
/**************FUNCTION POINTER DEFINITION FOR ISR*************/
/**************************************************************/
void (*TIMER0_ISR)(void);
void (*TIMER1_ISR)(void);
void (*TIMER2_ISR)(void);
void (*TIMER3_ISR)(void);

void (*EXTIN0_ISR)(void);
void (*EXTIN1_ISR)(void);
void (*EXTIN2_ISR)(void);

void (*RXRINT_ISR)(void);
void (*PORTCH_ISR)(void);

void (*CP1INT_ISR)(void);
void (*CP2INT_ISR)(void);

/*************************************************************/
/****************EXTERN ISR RELATED FUNCTIONS*****************/
/*************************************************************/
extern void enableinterrupt(int type, void *function);
extern void disableinterrupt(int type);
#endif
