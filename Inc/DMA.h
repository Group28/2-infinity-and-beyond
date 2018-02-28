#include "main.h"
#include "utils.h"


#ifndef __DMA_H
#define __DMA_H

typedef struct {
	Buffer *espRX;
	Buffer *espTX;
	Buffer *usbRX;
	Buffer *usbTX;
} DMA_Buffers;

void DMA_init(DMA_Buffers buffers);


#endif
