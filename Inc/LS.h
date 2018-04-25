#include "main.h"
#include "Analog.h"
#include "SR.h"
#include "configuration.h"
#include "arm_math.h"

#ifndef __LIGHTSENSORS_H
#define __LIGHTSENSORS_H

enum LS_pattern {LS_patternA, LS_patternB};


/**
 * Light sensor handle
 *
 * used to operate the light sensors and process the values form the sensors
 */
typedef struct {
  uint16_t * rawReading;      //< Pointer to raw ADC reading values
  
  float32_t readingsPatternA[IR_SENSOR_COUNT * SENSOR_OVERSAMPLE]; //< processed values from first pattern
  float32_t readingsPatternB[IR_SENSOR_COUNT * SENSOR_OVERSAMPLE]; //< processed values from second pattern
	
	float32_t processedReadings[IR_SENSOR_COUNT];  //< combined filterd values form the pattern
	
	float32_t weightedSum;   // < The weighted sum of the senosr values
  
	
	enum LS_pattern state;   //< Current Sensor pattern
  
  uint8_t oversampleNumber; //< Current oversample Number
	
	bool newData;            //< Is new data available
	
  Analog adc;
  SR sr;
} __LS, *LS;

/**
 * Initialize the Light sensors
 *
 * @param adc - ADC handle
 * @paramsr - Shift register handle
 * @param count - number of sensors
 * @return LightSensors handle
 */
LS LS_init(Analog adc, SR sr);

/**
 * Update the sensor pattern and process the values
 *
 * @param ls - Handle to the Light sensors controller
 * @return 
 */
void LS_update(LS ls);

/**
 * Updates the SR pattern depending on local state
 *
 * @param ls - Handle to the Light sensors controller
 * @return 
 */
void LS_setPattern(LS ls);

/**
 * Update the sensor to the next pattern in sequence
 *
 * @param ls - Handle to the Light sensors controller
 * @return 
 */
void LS_nextPattern(LS ls);

/**
 * Computes weighted sum of the sensors
 *
 * @param ls - Handle to the Light sensors controller
 * @return weighted sum of sensors
 */
float LS_getWeightedSum(LS ls);

/**
 * provieds array of processed values from the sensors
 *
 * @param ls - Handle to the Light sensors controller
 * @param values - array pointer used to return array of processed sensor values. 
 * @return 
 */
void LS_getProcessedValues(LS ls, float32_t * values);


/**
 * Determines if line has be caught
 *
 * @param ls - Handle to the Light sensors controller
 * @return line detected
 */
bool LS_catchLine(LS ls);



#endif
