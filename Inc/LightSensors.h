#include "main.h"
#include "Analog.h"
#include "SR.h"

#ifndef __LIGHTSENSORS_H
#define __LIGHTSENSORS_H

typedef struct {
  uint8_t count;
  uint16_t * rawReading;
  Analog adc;
  SR sr;
  
  
} __LightSensors, *LightSensors;

LightSensors LightSensors_init(Analog adc, SR sr, uint8_t count);

void LightSensors_update(void);

#endif
