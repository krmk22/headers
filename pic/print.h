#ifndef _PRINT_H
#define _PRINT_H

#include <htc.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#ifndef _UTILS_H
#include "utils.h"
#endif

#ifndef printarray
#define printarray
unsigned char printablearray[255];
#endif

#ifndef printcount
#define printcount strlen((char*)printablearray)
#endif

/**********************************************************************
@brief	implementation of printf style function that holds array (vprintf)
@param	format require function in string 
@param	arg listed function argument
@return function returns nothing
**********************************************************************/
static void print(const char *format, va_list arg)
{
	memset(printablearray, '\0', sizeof(printablearray));
	
	unsigned char localarray[12];
	unsigned char postfix = 0;
	unsigned char prefix = 0;
	
	char *storedata;
	char data;
	
	float fvalue;
	unsigned long dvalue;
	
	do
	{
		if(*format == '%')
		{
			format++;
			
			if(isdigit(*format))
			{
				postfix = *format - '0';
				if(*++format == '.')
				{
					prefix = *(++format) - '0';
					format++;
				}	
			}	
			
			switch(*format)
			{
				case 'S': case 's':
				storedata = va_arg(arg, char*);
				strncat(printablearray, storedata, strlen(storedata));
				break;
				
				case 'D': case 'd': 
				dvalue = va_arg(arg, unsigned long);
				dtoa(dvalue, (char*)localarray, 
				(postfix ?postfix :3), DEC);
				strncat(printablearray, localarray, strlen(localarray));
				postfix = 0;
				break;
				
				case 'F': case 'f':
				fvalue = va_arg(arg, double);
				ftostra(fvalue, (postfix ?postfix :2), 
				(prefix ?prefix :2), (char*)localarray);
				strncat(printablearray, localarray, strlen(localarray));
				postfix = 0; prefix = 0;
				break;
				
				case 'O': case 'o':
				dvalue = va_arg(arg, unsigned int);
				dtoa(dvalue, (char*)localarray, 
				(postfix ?postfix :3), OCT);
				strncat(printablearray, localarray, strlen(localarray));
				postfix = 0;
				break;
				
				case 'H': case 'h':
				dvalue = va_arg(arg, unsigned int);
				dtoa(dvalue, (char*)localarray, 
				(postfix ?postfix :3), HEX);
				strncat(printablearray, localarray, strlen(localarray));
				postfix = 0;
				break;
				
				case 'B': case 'b':
				dvalue = va_arg(arg, unsigned int);
				dtoa(dvalue, (char*)localarray, 
				(postfix ?postfix :8), BIN);
				strncat(printablearray, localarray, strlen(localarray));
				postfix = 0;
				break;
				
				case 'C': case 'c':
				dvalue = va_arg(arg, int);
				localarray[0] = dvalue;
				localarray[1] = '\0';
				strncat(printablearray, localarray, strlen(localarray));
				break;
				
				case 'N': case 'n':
				localarray[0] = '\r';
				localarray[1] = '\n';
				localarray[2] = '\0';
				strncat(printablearray, localarray, strlen(localarray));
				break;
				
				default:
				localarray[0] = '%';
				localarray[1] = '\0';
				strncat(printablearray, localarray, strlen(localarray));
				break;
			}	
		}
		else
		{
			localarray[0] = *format;
			localarray[1] = '\0';
			strncat(printablearray, localarray, strlen(localarray));
		}
	}
	while(*format++ != '\0');
}
#endif		
