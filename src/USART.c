#include "USART.h"
#include "configuration.h"

#include <stdlib.h>

USART * __USART_init(void){
	USART * usart = malloc(sizeof(USART));
  usart->usart = NULL;
	usart->bufferIn.buffer = malloc(sizeof(unsigned char) * DEFAULT_BUFFER_SIZE);
	usart->bufferIn.index = 0;
	usart->bufferIn.length = DEFAULT_BUFFER_SIZE;
	usart->bufferIn.send = 0;
	
	usart->buggerOut.buffer = malloc(sizeof(unsigned char) * DEFAULT_BUFFER_SIZE);
	usart->buggerOut.index = 0;
	usart->buggerOut.length = DEFAULT_BUFFER_SIZE;
	usart->buggerOut.send = 0;
	return usart;

}


USART * USART_USB_init(void){
  USART * usart = __USART_init();
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

USART *  USART_ESP_init(void){
  USART * usart = __USART_init();
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
