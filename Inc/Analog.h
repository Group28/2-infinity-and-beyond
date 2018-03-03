#include "main.h"
#include "utils.h"

#ifndef __ADC_H
#define __ADC_H

typedef struct {
  ADCBuffer buffer;
  uint8_t enabled:1;
  uint8_t converting:1;
} __Analog, *Analog;

Analog Analog_init(void);

inline void Analog_enable(Analog adc);

inline void Analog_startConversion(Analog adc);

uint16_t * Analog_getValues(Analog adc);

#endif
