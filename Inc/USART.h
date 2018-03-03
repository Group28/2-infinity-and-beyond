#include "main.h"
#include "utils.h"
#include "configuration.h"

#ifndef __USART_H
#define __USART_H


typedef struct {
  USART_TypeDef *usart;
	Buffer buffRX;
  Buffer buffTX;
} __USART, * USART;;

int USART_putc(USART usart, char c);
int USART_putc_buffer(USART usart, char c);

int USART_puts(USART usart, char *string);
int USART_puts_buffer(USART usart, char *string);

int USART_printf(USART usart, const char *format, ...);
int USART_printf_buffer(USART usart, const char *format, ...);

int USART_flushBuffer(USART usart);



USART USART_USB_init(void);
USART USART_ESP_init(void);



#endif
