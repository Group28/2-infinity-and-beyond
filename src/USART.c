#include "USART.h"
#include "DMA.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

static USART __USART_init(void);

USART __USART_init(void){
	USART usart = malloc(sizeof(__USART));
  usart->usart = NULL;
	usart->buffRX.buffer = malloc(sizeof(char) * USART_BUFFER_SIZE);
	usart->buffRX.index = 0;
	usart->buffRX.length = USART_BUFFER_SIZE;
	usart->buffRX.send = 0;
	
	usart->buffTX.buffer = malloc(sizeof(char) * USART_BUFFER_SIZE);
	usart->buffTX.index = 0;
	usart->buffTX.length = USART_BUFFER_SIZE;
	usart->buffTX.send = 0;
	return usart;

}


USART USART_USB_init(void){
  USART usart = __USART_init();
	usart->usart = USART2;
   /* Enable USART peripheral clock and clock source ***********************/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  /* Configure USART functional parameters ********************************/
  /* Note: Commented as corresponding to Reset value */
  LL_USART_Disable(USART2);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
 
  LL_USART_SetBaudRate(USART2,SystemCoreClock/2, LL_USART_OVERSAMPLING_16, USART_BAUDRATE); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(USART2);
  return usart;
}

USART  USART_ESP_init(void){
  USART usart = __USART_init();
	usart->usart = USART6;
	
    /* Enable USART peripheral clock and clock source ***********************/
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);
  /* Configure USART functional parameters ********************************/
  /* Note: Commented as corresponding to Reset value */
  LL_USART_Disable(USART6);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART6, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART6, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
 
  LL_USART_SetBaudRate(USART6,SystemCoreClock, LL_USART_OVERSAMPLING_16, USART_BAUDRATE); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(USART6);

  return usart;
}

int USART_putc(USART usart, char c){
		USART_putc_buffer(usart, c);
		USART_flushBuffer(usart);
		return 1;
}

int USART_putc_buffer(USART usart, char c){
	uint8_t complete = 0;
	while(!complete) {
		while(usart->buffTX.send){} // If transmitting, wait until we are done
		if(usart->buffTX.index < usart->buffTX.length){
			usart->buffTX.buffer[usart->buffTX.index++] = c;
			complete = 1;
		} else {
			USART_flushBuffer(usart);
		}
	}
	return 0;
}

int USART_puts(USART usart, char *string){
	USART_puts_buffer(usart, string);
	USART_flushBuffer(usart);
	return 1;
}

int USART_puts_buffer(USART usart, char *string){
	uint8_t done = 0;
	uint32_t start_string = 0;

	while(!done){
		while(usart->buffTX.send){}
		uint32_t available = usart->buffTX.length - usart->buffTX.index;
		uint32_t str_length = strlen(string + start_string);
		if(str_length < available){
			strcpy(usart->buffTX.buffer, string + start_string);
			usart->buffTX.index += str_length;
			done = 1;
		} else {
			strncpy(usart->buffTX.buffer, string+start_string, available);
			usart->buffTX.index += available;
			start_string += available;
			USART_flushBuffer(usart);
		}
	}
	return 0;
}


int USART_printf(USART usart, const char *format, ...){
  char outputString[256];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(outputString, format, argptr);
  va_end(argptr);
  return USART_puts(usart, outputString);
}

int USART_flushBuffer(USART usart) {
	usart->buffTX.send = 1;
	return DMA_StartSerialTransfer(usart);
}
