#include "main.h"

#ifndef __LIGHTSENSORS_H
#define __LIGHTSENSORS_H

typedef struct {
  uint8_t count;
  uint16_t * rawReading;
  
  
} __LightSensors, *LightSensors;

LightSensors LightSensors_init(void);

#endif
