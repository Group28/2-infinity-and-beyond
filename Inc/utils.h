#include "main.h"
#include "configuration.h"
#ifndef __UTILS_H
#define __UTILS_H

/**
 * Basic buffer used in DMA
 *
 */
typedef struct {
	char *buffer;
	uint32_t length;
	uint32_t index;
	uint8_t send:1;
} Buffer;

/**
 * ADC buffer used in DMA
 *
 */
typedef struct {
	uint16_t buffer[ADC_CHANNEL_COUNT];
	uint8_t newData:1;
	
} ADCBuffer;

/**
 * Delay for microseconds
 *
 * @param us - number of microseconds to wait
 */
void delay_us(uint32_t);
/**
 * Delay for milliseconds
 *
 * @param ms - number of milliseconds to wait
 */
void delay_ms(uint32_t);

/**
 * Delay for seconds
 *
 * @param us - number of seconds to wait
 */
void delay(double);


#endif
