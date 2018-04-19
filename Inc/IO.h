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

/*
 * Initialize all IO devices
 */
void IO_init(void);


/* 
 * Set output high/low
 *
 * @param port - IO port
 * @param pin - IO pin number
 * @param state - state 0 - low, 1 high
 */
void IO_set(GPIO_TypeDef * port, uint32_t pin, uint8_t state);

/*
 * Sets speaker frequency
 *
 * @param freq - frequency
 */
void IO_setSpeakerFreq(int freq);
//void IO_Analog_init();


#endif
