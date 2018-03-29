#include "main.h"
#include "Analog.h"

#ifndef __MAGNET_H
#define __MAGNET_H

typedef struct {
  uint16_t * rawReading;
  Analog adc;
  
} __Magnet, *Magnet;

Magnet Magnet_init(Analog adc);

#endif
