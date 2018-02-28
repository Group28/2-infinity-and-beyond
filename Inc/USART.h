#include "main.h"
#include "utils.h"

#ifndef __USART_H
#define __USART_H

#define DEFAULT_BUFFER_SIZE 128

typedef struct {
  USART_TypeDef *usart;
	Buffer bufferIn;
  Buffer buggerOut;
} USART;


int USART_putc(USART *usart, unsigned char c);
int USART_puts(USART *usart, unsigned char *string);
int USART_printf(USART *usart, const char *format, ...);

static USART * __USART_init(void);
USART *USART_USB_init(void);
USART *USART_ESP_init(void);



#endif
