#include "main.h"

#ifndef __USART_H
#define __USART_H


typedef struct {
  USART_TypeDef * usart;
} USART;


int USART_printf(USART * usart);


#endif
