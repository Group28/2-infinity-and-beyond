#include "main.h"

#ifndef __SYSTEM_FUNCTIONS_H
#define __SYSTEM_FUNCTIONS_H
void SystemClock_Config(void);

void delay_us(uint32_t);
void delay_ms(uint32_t);
void delay(double);

uint32_t System_micros(void);

#endif
