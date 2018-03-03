#include "main.h"
#include "utils.h"
#include "Analog.h"
#include "LCD.h"
#include "USART.h"


#ifndef __DMA_H
#define __DMA_H

typedef struct {
	Buffer *espRX;
	Buffer *espTX;
	Buffer *usbRX;
	Buffer *usbTX;
	Buffer *lcdTX;
	
	ADCBuffer *adcData;
	
} __DMA_Buffers,  *DMA_Buffers;


DMA_Buffers DMA_getBuffers(USART esp, USART usb, LCD lcd, Analog adc);

void DMA_init(DMA_Buffers buffers);

int DMA_StartSerialTransfer(USART usart);


#endif
