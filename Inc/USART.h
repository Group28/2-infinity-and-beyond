#include "main.h"

#ifndef __USART_H
#define __USART_H


typedef struct {
  USART_TypeDef * usart;
  char * bufferIn;
  char * buggerOut;
} USART;


int USART_putc(USART * usart, char c);
int USART_puts(USART * usart, char * string);
int USART_printf(USART * usart, const char * format, ...);



#endif
