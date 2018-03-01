#include "main.h"

#ifndef __IO_H
#define __IO_H

/*

typedef struct {
	GPIO_TypeDef * port;
	uint32_t pin;
	TIM_TypeDef * timer;
} PWM_pin;

typedef struct{
	GPIO_TypeDef * port;
	uint32_t pin;
} OUT_pin;
*/

// Initialize all IO devices
void IO_init(void);


// Set output high/low
void IO_set(GPIO_TypeDef * port, uint32_t pin, uint8_t state);

//void IO_Analog_init();


#endif
