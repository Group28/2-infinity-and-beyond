#include "main.h"
#include "Analog.h"
#include "SR.h"

#ifndef __LIGHTSENSORS_H
#define __LIGHTSENSORS_H

/**
 * Light sensor handle
 *
 * used to operate the light sensors and process the values form the sensors
 */
typedef struct {
  uint8_t count;
  uint16_t * rawReading;
  Analog adc;
  SR sr;
  
  
} __LightSensors, *LightSensors;

/**
 * Initialize the Light sensors
 *
 * @param adc - ADC handle
 * @paramsr - Shift register handle
 * @param count - number of sensors
 * @return LightSensors handle
 */
LightSensors LightSensors_init(Analog adc, SR sr, unit8_t count);

/**
 * Update the sensor pattern and process the values
 *
 * @param ls - Handle to the Light sensors controller
 * @return 
 */
void LightSensors_update(LightSensors ls)

#endif
