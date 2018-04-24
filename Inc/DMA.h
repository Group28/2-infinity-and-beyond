#include "main.h"
#include "utils.h"
#include "Analog.h"
#include "LCD.h"
#include "USART.h"


#ifndef __DMA_H
#define __DMA_H

/*
 * DMA_Buffers 
 *
 * Used to hold pointers to data source and destination buffers
 */
typedef struct {
	Buffer *espRX; //< ESP receive buffer
	Buffer *espTX; //< ESP transmit buffer
	Buffer *usbRX; //< USB receive buffer
	Buffer *usbTX; //< USB transmit buffer
	Buffer *lcdTX; //< LCD transmit buffer
	
	ADCBuffer *adcData; //< ADC conversion data buffer
	
} __DMA_Buffers,  *DMA_Buffers;

/*
 * Extracts the data buffers from relevant peripherals into the DMA_Buffers struct
 *
 * @param esp - ESP USART handle
 * @param usb - USB USART handle
 * @param lcd - LCD handle
 * @param adc - ADC handle
 * @return DMA_Buffers - extracted buffers
 */
DMA_Buffers DMA_getBuffers(USART esp, USART usb, LCD lcd, Analog adc);

/*
 * Initializes DMA controller and sets up source and sink buffers
 *
 * @param buffers - extracted peripheral buffers
 */
void DMA_init(DMA_Buffers buffers);

/*
 * Starts serial transfer for a USART device
 *
 * @param usart - USART device to use
 * @return int - status
 */
int DMA_StartSerialTransfer(USART usart);

#endif
