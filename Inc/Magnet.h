#include "main.h"
#include "Analog.h"

#ifndef __MAGNET_H
#define __MAGNET_H

typedef struct {
  uint16_t * rawReading;
	uint16_t runningAverage;
  Analog adc;
} __Magnet, *Magnet;

typedef int8_t MagnetValue;

#define MAGNET_NORTH 1
#define MAGNET_SOUTH -1
#define MAGNET_NOMAG 0
#define MAGNET_ERROR 2

Magnet Magnet_init(Analog adc);

void Magnet_update(Magnet magnet);

MagnetValue Magnet_getValue(Magnet magnet);

#endif
