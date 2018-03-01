#include "main.h"

#ifndef __UTILS_H
#define __UTILS_H

typedef struct {
	char *buffer;
	uint32_t length;
	uint32_t index;
	uint8_t send;
} Buffer;


void delay_us(uint32_t);
void delay_ms(uint32_t);
void delay(double);


#endif
