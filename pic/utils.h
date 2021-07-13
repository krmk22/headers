#ifndef _UTILS_H
#define _UTILS_H

/***************************************************/
/***********PIC 18F4520 CHIP CONFIG BITS************/
/***************************************************/
#if 0
__CONFIG(1, HSPLL);
__CONFIG(2, PWRTEN & BOREN & WDTDIS);
__CONFIG(3, MCLREN & PBDIGITAL & CCP2RC1);
__CONFIG(4, DEBUGDIS & LVPDIS & STVREN & XINSTDIS);
#endif

/***************************************************/
/****DEFINITION OF CLOCK RELATED CALCULATIONS*******/
/***************************************************/
#define _XTAL_FREQ	40000000UL
#define OneMachineCycle	0.1F
#define MachineCycleUS	10

/***************************************************/
/*******DEFINTION AND DECLARATION OF DELAY**********/
/***************************************************/
#define DelayUS(Delay)\
{\
	unsigned int ReqDelay = Delay;\
	while(--ReqDelay) _delay(2);\
}

#define DelayMS(Delay)\
{\
	unsigned char RoutineDelay;\
	unsigned int RequireDelay = Delay;\
	do\
	{\
		RoutineDelay = 4;\
		do\
		{\
			__delay_us(248);\
		}\
		while(--RoutineDelay);\
	}\
	while(--RequireDelay);\
}			

/************************************************/
/********FIXED PREDEFINED CONFIGURATIONS*********/
/************************************************/
typedef enum
{
	BIN = 2,
	OCT = 8,
	DEC = 10,
	HEX = 16
}
PIC_DECIMAL_CONVERSION;

#define true 1
#define false 0

#define enable 1
#define disable 0

/**************************************************************/
/********************EXTERN DECLARED FUNCTIONS*****************/
/**************************************************************/
extern long mapvalue(long value, long startmin, long startmax,
long endmin, long endmax);
extern char *smemchr(register const char *mainstring, int substring,
int length);
extern char *smemrchr(register const char *mainstring, int substring,
int length);
extern void *smemmem(const void *mainstring, int lengthofmainstring,
const void *substring, int lengthofsubstring);
extern double power(double base, int exponent);
extern int dtoa(unsigned long data, unsigned char *array, int length,
int base);
extern int ftostra(float data, int postfix, int prefix, 
unsigned char *array);
extern char splitstring(unsigned char *mainstring, unsigned char *array,
const char *startofstring, unsigned char endofstring);
#endif



