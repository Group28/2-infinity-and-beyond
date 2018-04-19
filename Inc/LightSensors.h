#include "main.h"
#include "Analog.h"
#include "SR.h"
#include "configuration.h"
#include "arm_math.h"

#ifndef __LIGHTSENSORS_H
#define __LIGHTSENSORS_H

enum LS_pattern {LS_patternA, LS_patternB};

typedef struct {
  uint16_t * rawReading;
	
	float32_t processedReadingsA[IR_SENSOR_COUNT];
	float32_t processedReadingsB[IR_SENSOR_COUNT];
	
	float32_t processedReadings[IR_SENSOR_COUNT];
	
	float32_t weightedSum;
	
	uint32_t lcConfidence;
	
	enum LS_pattern state;
	
	bool newData;
	
	
	
  Analog adc;
  SR sr;
} __LS, *LS;

LS LS_init(Analog adc, SR sr);

void LS_update(LS ls);

void LS_setPattern(LS ls);
void LS_nextPattern(LS ls);


float LS_getWeightedSum(LS ls);

void LS_getProcessedValues(LS ls, float32_t * values);

bool LS_catchLine(LS ls);

#endif
