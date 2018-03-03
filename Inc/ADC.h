#include "main.h"
#include "utils.h"

#ifndef __ADC_H
#define __ADC_H

typedef struct {
  ADCBuffer buffer;
  uint8_t enabled:1;
  uint8_t converting:1;
} __ADC, *ADC;

ADC ADC_init(void);

inline void ADC_activate(ADC adc);

inline void ADC_startConversion(ADC adc);

uint16_t * ADC_getValues(ADC adc);

#endif
