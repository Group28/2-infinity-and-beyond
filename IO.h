#include "main.h"

#ifndef __IO_H
#define __IO_H


typedef struct {
	GPIO_TypeDef * port;
	uint32_t pin;
	TIM_TypeDef * timer;
} PWM_pin;

typedef struct{
	GPIO_TypeDef * port;
	uint32_t pin;
} OUT_pin;



void IO_init(void);
void IO_Output_init(OUT_pin * pin);

#endif
