#include "LightSensors.h"
#include <stdlib.h>
#include "arm_math.h"


static float32_t LS_weights[] = LS_WEIGHTS;


LS LS_init(Analog adc, SR sr){
  LS ls = malloc(sizeof(__LS)); 
  ls->adc = adc;
  ls->sr = sr;
	
	ls->weightedSum = 0;
	ls->state = LS_patternA;
	ls->rawReading = Analog_getValues(ls->adc);
	
	ls->lcConfidence = 0;
	
	ls->newData = false;
	
	for (size_t i = 0; i < IR_SENSOR_COUNT; i++) {
    ls->processedReadingsA[i] = 0;
		ls->processedReadingsB[i] = 0;
		ls->processedReadings[i] = 0;
	}
  
  return ls;
}


void LS_update(LS ls){
	float32_t * processedReadings;
	
	switch(ls->state){
		case LS_patternA:
			processedReadings = ls->processedReadingsA;
			
			break;
		case LS_patternB:
			processedReadings = ls->processedReadingsB;
			break;
	}
	
	
	for(int i = 0; i< IR_SENSOR_COUNT; i++){
			processedReadings[i]=ls->rawReading[i]/4096.0;
	}
	
	
	if(ls->state == LS_patternB){
		
		// Data processing going on here
		arm_sub_f32(ls->processedReadingsA, ls->processedReadingsB, ls->processedReadings, IR_SENSOR_COUNT);
		arm_abs_f32(ls->processedReadings, ls->processedReadings, IR_SENSOR_COUNT);
		
	
		
		arm_dot_prod_f32(ls->processedReadings, LS_weights, IR_SENSOR_COUNT, &ls->weightedSum);
		
		ls->newData = true;
	}
	
	LS_nextPattern(ls);
}

void LS_setPattern(LS ls){
	switch(ls->state){
		case LS_patternA:
			SR_set(ls->sr, 0x19);
			break;
		case LS_patternB:
			SR_set(ls->sr, 0x26);
			break;
	}
}

void LS_nextPattern(LS ls) {
	switch(ls->state){
		case LS_patternA:
			SR_shiftIn(ls->sr, 0x01);
			SR_shiftIn(ls->sr, 0x00);
			ls->state = LS_patternB;
			break;
		case LS_patternB:
			SR_shiftIn(ls->sr, 0x00);
			SR_shiftIn(ls->sr, 0x01);
			ls->state = LS_patternA;
			break;
	}
	SR_latch();
}

float LS_getWeightedSum(LS ls){
	ls->newData = false;
	return (float) ls->weightedSum;
}

void LS_getProcessedValues(LS ls, float32_t * values){
	ls->newData = false;
	memcpy(values, ls->processedReadings, IR_SENSOR_COUNT * sizeof(float32_t));
}

bool LS_catchLine(LS ls){
	if(ls->processedReadings[1] > 0.6){
		ls->lcConfidence += 1;
	}
	
	if(ls->lcConfidence > 4){
		return true;
	} else {
		return false;
	}

}

