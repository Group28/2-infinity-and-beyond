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

// Initialize an output
void IO_Output_init(GPIO_TypeDef * port, uint32_t pin);
void IO_set(GPIO_TypeDef * port, uint32_t pin, uint8_t state);

//void IO_Analog_init();

void IO_initMotors(void);
void IO_initEncoders(void);
void IO_initAnalogueSensors(void);

void IO_initLCD(void);
void IO_initUSART_USB(void);
void IO_initUSART_ESP(void);

void IO_initJoyButton(void);
#endif
