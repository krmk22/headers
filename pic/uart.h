#ifndef _UART_H
#define _UART_H

typedef struct
{
	unsigned char id;
	unsigned long speed;
	unsigned char transmitter;
	unsigned char receiver;
	unsigned char array[100];
	unsigned char count;
}
PIC_UART_CONFIG;

#ifndef UART
#define UART
PIC_UART_CONFIG uart0 = {0, 9600, PB6, PB7};
#endif

#ifndef UART1
#define UART1
PIC_UART_CONFIG uart1 = {1, 9600, PC0, PC1};
#endif

extern void serial_initialize(PIC_UART_CONFIG *uart, unsigned long speed);
extern void serial_callback(PIC_UART_CONFIG *uart, void *function);
extern void serial_baudrate(PIC_UART_CONFIG *uart, unsigned long speed);

extern void serial_send(PIC_UART_CONFIG *uart, unsigned char data);
extern void serial_write(PIC_UART_CONFIG *uart, const char *data);
extern void serial_writeln(PIC_UART_CONFIG *uart, const char *data, unsigned int length);

extern void serial_decimal(PIC_UART_CONFIG *uart, unsigned long data, int length, int base);
extern void serial_float(PIC_UART_CONFIG *uart, float data, int postfix, int prefix);
extern void serial_print(PIC_UART_CONFIG *uart, const char *function, ...);
extern void serial_flush(PIC_UART_CONFIG *uart);

extern void serial_enable(PIC_UART_CONFIG *uart, unsigned char state);
extern unsigned char serial_receive(PIC_UART_CONFIG *uart);
extern unsigned char serial_talkback(PIC_UART_CONFIG *uart, int newline, 
const char *expdata, unsigned char length, unsigned int timedelay);
#endif
	
