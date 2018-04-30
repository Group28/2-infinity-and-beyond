#include "main.h"
#include "Analog.h"
#include "RA.h"

#ifndef __MAGNET_H
#define __MAGNET_H

/**
 * Magnet
 *
 * hande used to interface with the magnet
 */
typedef struct {

	RAf runningAverage; //< Running average used to filter the magnetic input
  uint16_t * rawReading; //< Raw magnet reading
  Analog adc;            //< ADC handle
  
} __Magnet, *Magnet;

typedef int8_t MagnetValue; //< Value used to represent the value of the magnet

// Defined magnet values
#define MAGNET_NORTH 1
#define MAGNET_SOUTH -1
#define MAGNET_NOMAG 0
#define MAGNET_ERROR 2

/**
 * Initialize the magnetic sensor
 *
 * @param adc -ADC handle
 * @return Magnet handle
 */
Magnet Magnet_init(Analog adc);

void Magnet_update(Magnet magnet);

MagnetValue Magnet_getValue(Magnet magnet);

#endif
