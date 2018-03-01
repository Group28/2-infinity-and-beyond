#include "main.h"
#include "utils.h"
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
} DMA_Buffers;


DMA_Buffers * DMA_getBuffers(USART *esp, USART *usb, LCD *lcd);

void DMA_init(DMA_Buffers *buffers);

int DMA_StartSerialTransfer(USART *usart);


#endif
