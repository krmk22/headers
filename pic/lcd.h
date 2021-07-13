#ifndef _LCD_H
#define _LCD_H

#define CLEAR_DISPLAY		0x01
#define RETURN_HOME			0x02
#define ENTRY_MODE			0x04
#define DISPLAY_CONTROL	0x08
#define DISPLAY_SHIFT		0x10
#define FUNCTION_SET		0x20
#define CGRAM_ADDRESS		0x40

#define SHIFT_ENABLE	(ENTRY_MODE | (1 << 0))
#define SHIFT_DISABLE	(ENTRY_MODE &~(1 << 0))
#define CUR_INCREMENT	(ENTRY_MODE | (1 << 1))
#define CUR_DECREMENT	(ENTRY_MODE &~(1 << 1))

#define DISPLAY_ON		(DISPLAY_CONTROL | (1 << 2))
#define DISPLAY_OFF		(DISPLAY_CONTROL &~(1 << 2))
#define CURSOR_ON		  (DISPLAY_ON | (1 << 1))
#define CURSOR_OFF		(DISPLAY_ON &~(1 << 1))
#define BLINK_ON		  (DISPLAY_ON | (1 << 0))
#define BLINK_OFF		  (DISPLAY_ON &~(1 << 0))

#define ENTIRE_LINE		(DISPLAY_SHIFT &~(1 << 3))
#define CURRENT_LINE	(DISPLAY_SHIFT | (1 << 3))
#define SHIFT_LEFT		(DISPLAY_SHIFT &~(1 << 2))
#define SHIFT_RIGHT		(DISPLAY_SHIFT | (1 << 2))

#define EIGHT_BIT		(FUNCTION_SET | (1 << 4))
#define FOUR_BIT		(FUNCTION_SET &~(1 << 4))
#define TWO_LINE		(FUNCTION_SET | (1 << 3))
#define ONE_LINE		(FUNCTION_SET &~(1 << 3))
#define CHAR_SET2		(FUNCTION_SET | (1 << 2))
#define CHAR_SET1		(FUNCTION_SET &~(1 << 2))

typedef struct
{
	unsigned char RS;
	unsigned char EN;
	unsigned char D4;
	unsigned char D5;
	unsigned char D6;
	unsigned char D7;
}
LCD_PIN_CONFIG;

extern void lcd_initialize(char rs, char en, char d4, char d5, char d6, char d7);
extern void lcd_command(unsigned char command);
extern void lcd_function(unsigned char command);

extern void lcd_write(unsigned char command, unsigned char data);
extern void lcd_writeln(unsigned char command, const char *data, unsigned char length);
extern void lcd_disp(unsigned char command, const char *data);

extern void lcd_decimal(unsigned char command, unsigned long data, int length, int base);
extern void lcd_float(unsigned char command, float data, int postfix, int prefix);
extern void lcd_print(unsigned char command, const char *format, ...);

extern void lcd_custom(unsigned char location, const char *data);
extern void lcd_char(unsigned char command, unsigned char location, const char *data);
extern void lcd_allort(unsigned char data);
extern void lcd_enable(void);
#endif
