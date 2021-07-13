#ifndef _ESP8266_H
#define _ESP8266_H

typedef enum
{
	TCP = 1,
	UDP = 2,
	HTTP = 3,
	MQTT = 4,
	SSL = 5
}
ESP8266_VALID_PROTOCOLS;

typedef enum
{
	WAKEUP = 0,
	LIGHT = 1,
	MODEM = 2,
	DEEP = 3
}
ESP8266_SLEEP_MODE;

typedef enum
{
	STATION = 1,
	SOFT_AP = 2,
	STATION_SOFT_AP = 3
}
ESP8266_WIFI_MODE;	

extern void esp_initialize(PIC_UART_CONFIG *uart, unsigned long speed);
extern char esp_found(void);
extern char esp_echo(unsigned char state);
extern char esp_reset(void);
extern char esp_sleep(unsigned char state);

extern char esp_mode(unsigned char state);
extern char esp_connect(const char *ssid, const char *pass);
extern char esp_disconnect(void);
extern char esp_autoconnect(unsigned char state);

extern char esp_multiplex(unsigned char state);
extern char esp_start(unsigned char id, unsigned char level, const char *server, const char *port);
extern char esp_size(unsigned long size);
extern char esp_send(unsigned char id, unsigned char length);
extern char esp_close(unsigned char id);

extern char esp_timeout(unsigned long time);
extern char esp_ping(const char *server);
#endif
