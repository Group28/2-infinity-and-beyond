#include "main.h"
#include "GenericTypeDefs.h"
#include <stdbool.h>

#ifndef __ONEWIRE_H
#define __ONEWIRE_H

typedef struct {
  GPIO_TypeDef * port;
	uint32_t pin;
} OW;

OW * OW_init(GPIO_TypeDef * port, uint32_t pin);

bool OW_reset(OW *ow);

void OW_writeBit(OW *ow, BYTE b);

BYTE OW_readBit(OW *ow);

void OW_writeByte(OW *ow, BYTE b);

BYTE OW_readByte(OW *ow);

#define __OW_OUTPUT() LL_GPIO_SetPinMode(ow->port, ow->pin, LL_GPIO_MODE_OUTPUT);
#define __OW_INPUT()  LL_GPIO_SetPinMode(ow->port, ow->pin, LL_GPIO_MODE_INPUT);


#define __OW_Delay_A()	__delay_us(5);
#define __OW_Delay_B()	__delay_us(64);
#define __OW_Delay_C()	__delay_us(60);
#define __OW_Delay_D()	__delay_us(10);

#define __OW_Delay_E()	__delay_us(10);
#define __OW_Delay_F()	__delay_us(55);

//Delays Used for OWReset (Confirmed Working)
#define __OW_Delay_H()	__delay_us(480);
#define __OW_Delay_I()	__delay_us(68);
#define __OW_Delay_J()	__delay_us(405);


#endif
