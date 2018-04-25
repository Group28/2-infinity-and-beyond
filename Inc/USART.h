#include "main.h"
#include "utils.h"
#include "configuration.h"

#ifndef __USART_H
#define __USART_H

/**
 * Usart handle
 *
 * Used for communication through USART
 */
typedef struct {
  USART_TypeDef *usart; //< USART interface
	Buffer buffRX;       // < USART RX buffer
  Buffer buffTX;       //< USART TX buffer
} __USART, * USART;   


/**
 * Put charecter into USART interface
 *
 * @param usart - USART handle
 * @param c - char to send
 * @return success
 */
int USART_putc(USART usart, char c);
int USART_putc_buffer(USART usart, char c);

/**
 * Put sctring into USART interface
 *
 * @param usart - USART handle
 * @param string - string to send
 * @return success
 */
int USART_puts(USART usart, char *string);
int USART_puts_buffer(USART usart, char *string);

/**
 * Send formated string through USART
 *
 * @param usart - USART handle
 * @param format - formating followed by valeus
 * @return status
 */
int USART_printf(USART usart, const char *format, ...);
int USART_printf_buffer(USART usart, const char *format, ...);

/**
 * Flush the usart buffer to the device
 *
 * @param usart - USART handle
 */
int USART_flushBuffer(USART usart);


/**
 * Initialize USB usart
 *
 * @return USB usart handle
 */
//USART USART_USB_init(void);
/**
 * Initialize ESP usart
 *
 * @return ESP usart handle
 */
USART USART_ESP_init(void);



#endif
