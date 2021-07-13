#ifndef _I2C_H
#define _I2C_H

#ifndef I2C_DEFAULT_PIN
#define I2C_DEFAULT_PIN
#define DEFAULT_I2C_SCL	PC3
#define DEFAULT_I2C_SDA PC4
#endif

typedef struct
{
	unsigned char type;
	unsigned char scl;
	unsigned char sda;
	unsigned int speed;
	unsigned char address;
}
PIC_I2C_CONFIG;

#ifndef I2C0
#define I2C0
PIC_I2C_CONFIG i2c0 = {0, DEFAULT_I2C_SCL, DEFAULT_I2C_SDA, 100};
#endif

#ifndef I2C1
#define I2C1
PIC_I2C_CONFIG i2c1 = {1};
#endif

#define BusFreeTime 	5
#define StartHoldTime	4
#define ClockLowTime 	5
#define ClockHighTime   5
#define RestartHoldTime 5
#define DataHoldTime	1
#define DataSetupTime   1
#define StopHoldTime    5

extern void i2c_start(PIC_I2C_CONFIG *i2c);
extern void i2c_stop(PIC_I2C_CONFIG *i2c);
extern void i2c_restart(PIC_I2C_CONFIG *i2c);
extern void i2c_clock(PIC_I2C_CONFIG *i2c);

extern void i2c_setaddress(PIC_I2C_CONFIG *i2c, unsigned char address);
extern void i2c_setgpio(PIC_I2C_CONFIG *i2c, unsigned char scl, unsigned char sda);
extern void i2c_setspeed(PIC_I2C_CONFIG *i2c, unsigned int speed);
extern void i2c_initialize(PIC_I2C_CONFIG *i2c);

extern unsigned char i2c_write(PIC_I2C_CONFIG *i2c, unsigned char data);
extern unsigned char i2c_read(PIC_I2C_CONFIG *i2c, unsigned char ack);
extern unsigned char i2c_register_write(PIC_I2C_CONFIG *i2c, unsigned char address, unsigned char data);
extern unsigned char i2c_register_read(PIC_I2C_CONFIG *i2c, unsigned char address);
extern void i2c_sequence_write(PIC_I2C_CONFIG *i2c, unsigned char address, unsigned char *array, unsigned char length);
extern void i2c_sequence_read(PIC_I2C_CONFIG *i2c, unsigned char address, unsigned char *array, unsigned char length);
#endif
