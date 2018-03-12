#include "main.h"
#include "GenericTypeDefs.h"
#include <stdbool.h>


#ifndef __ONEWIRE_H
#define __ONEWIRE_H

typedef struct {
  GPIO_TypeDef * port;
	uint32_t pin;
} __OW, *OW;


OW OW_init(GPIO_TypeDef *port, uint32_t pin);

bool OW_reset(OW ow);

void OW_writeBit(OW ow, BYTE b);

BYTE OW_readBit(OW ow);

void OW_writeByte(OW ow, BYTE b);

BYTE OW_readByte(OW ow);


#endif
