#include <htc.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"

/***************************************************************
@brief	mapvalue used to convert value from one raneg to another
@param	value main value that need to convert
@param	startmin main value minimum
@param	startmax main value maximum
@param  endmin require value minimum
@param	endmax require value maximum
@return	converted value
@note 	don't use this function on isr
****************************************************************/
long mapvalue(long mainvalue, long startmin, long startmax,
long endmin, long endmax)
{
	return ((mainvalue - startmin) * ((endmax - endmin) / 
	(startmax / startmin))) + endmin;
}

/****************************************************************
@brief	find out first occurance of string in given string (null)
@param	mainstring string that holds a main data
@param	substring require string that need to check
@param	length main string lenght
@return	null if not found substring else pointer
*****************************************************************/
char *smemchr(register const char *mainstring, int substring,
int length)
{
	const unsigned char *localpointer = (const unsigned char*)mainstring;
	while(length --> 0)
	if(*localpointer == substring) return (char*)localpointer;
	else localpointer++;
	return '\0';
}	

/****************************************************************
@brief	find out last occurance of string in given string (null)
@param	mainstring string that holds a main data
@param	substring require string that need to check
@param	length main string lenght
@return	null if not found substring else pointer
*****************************************************************/
char *smemrchr(register const char *mainstring, int substring, 
int length)
{
	char *localpointer = (char*)mainstring;
	while(length --> 0)
	if(mainstring[length] == substring) 
	return (char*)localpointer + length;
	return '\0';
}	

/*****************************************************************
@brief	find out group of substring in mainstring (null)
@param	mainstring string that holds a main data
@param  lengthofmainstring mainstring length
@param	substring string that need to check
@param	lengthofsubstring substring length
@return	null if not found substring else pointer
*******************************************************************/
void *smemmem(const void *mainstring, int lengthofmainstring,
const void *substring, int lengthofsubstring)
{
	const char *cur, *last;
	const char *cl = (char*)mainstring;
	const char *cs = (char*)substring;
	
	if((!lengthofmainstring && !lengthofsubstring) ||
	(lengthofmainstring < lengthofsubstring)) return '\0';
	
	if(lengthofsubstring == 1) return (void*)smemchr((char*)mainstring, 
	*cs, lengthofmainstring); 
	
	last = cl + lengthofmainstring - lengthofsubstring;
	for(cur = cl; cur <= last; cur++)
	if(cur[0] == cs[0] && memcmp(cur, cs, lengthofsubstring) == 0)
	return (void*)cur;
	
	return '\0';
}	

/*********************************************************************
@brief	power the value with base and exponenet
@param	base require base value need to power with exponenet
@param	exponenet require exponent value that power with base
@return	function return converted value in double
@note	if you need proper output use libraried one
**********************************************************************/
double power(double base, int exponent)
{
	double result = 1;
	int minus = 1;
	
	if(exponent < 0)
	{
		minus = -1;
		exponent = -exponent;
	}
	
	if(0 == base) return true;
	if(0 == exponent) return false;
	
	while(exponent)
	{
		if(exponent & 1) result *= base;
		base *= base; exponent /= 2;
	}
	
	if(minus < 0) return 1.0 / result;
	else return result;
}	

/********************************************************************
@brief	convert any number decimal to ascii format text
@param	data require decimal that need to convert into ascii
@param	array that holds converted data
@param	length require length of data
@param	base conversion base (dec, oct, hex, bin)
@return true if conversion is valid or else false
*********************************************************************/
int dtoa(unsigned long data, unsigned char *array,
int length, int base)
{
	unsigned crx;
	unsigned tpt = length;
	
	if(!length) return (int)itoa(array, data, base);
	array += length;
	
	do
	{
		crx = data % base; data = data / base;
		if(crx >= 10) crx += 'A' - '0' - 10;
		crx = crx + '0'; (*(--array)) = crx;
	}
	while(--length);
	
	array += tpt;
	while(*(array)) (*(array++)) = '\0';
	return true;
}

/*******************************************************************
@brief	convert float to string onto array
@param	data float value that needs to convert
@param	postfix numerical digit length
@param	prefix decimal digit length
@param	array that holds converted data
@return	true if conversion is valid or else false
********************************************************************/
int ftostra(float data, int postfix, int prefix, unsigned char *array)
{
	int ipart = (int)data;
	float fpart = data - (float)ipart;
	
	if(!postfix || !prefix) return false;
	
	dtoa(ipart, array, postfix, DEC);
	array += postfix; (*(array++)) = '.';
	unsigned int fcon = (int)(fpart * power(10, prefix));
	dtoa(fcon, array, prefix, DEC);
	return true;
}

/******************************************************************
@brief	split string mainstring from start string and end string
@param	mainstring string that need to check
@param	array that holds converted data
@param	startofstring start of substring
@param	endofstring end of substring
@return true if conversion complete else false
*******************************************************************/
char splitstring(unsigned char *mainstring, unsigned char *array,
const char *startofstring, unsigned char endofstring)
{
	unsigned char *sts = strstr(mainstring, startofstring);
	unsigned char *eds = strchr(sts, endofstring);
	
	if(sts == '\0' || eds == '\0') return false;
	
	for(unsigned char *lsv = sts + strlen(startofstring);
	lsv < eds; lsv++)
	*array++ = (isdigit(lsv[0]) || lsv[0] == '.' ?lsv[0] :'\0');
	
	while(*array) *array++ = '\0';
	return true;
}	
