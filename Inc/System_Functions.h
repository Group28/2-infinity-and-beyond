#include "main.h"


#ifndef __SYSTEM_FUNCTIONS_H
#define __SYSTEM_FUNCTIONS_H

/**
 * Configure system clocks
 *
 */
void SystemClock_Config(void);

/**
 * SysTick Handler
 *
 */
void SysTick_Handler(void);

/**
 * Get system microseconds count
 *
 * @return number of microseconds since system start
 */
inline uint32_t System_micros(void);

#endif
