#include "main.h"
#include "configuration.h"
#ifndef __UTILS_H
#define __UTILS_H

typedef struct {
	char *buffer;
	uint32_t length;
	uint32_t index;
	uint8_t send:1;
} Buffer;

typedef struct {
	uint16_t buffer[ADC_CHANNEL_COUNT];
	uint8_t newData:1;
	
} ADCBuffer;

void delay_us(uint32_t);
void delay_ms(uint32_t);
void delay(double);


#endif
