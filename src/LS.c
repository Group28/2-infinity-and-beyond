#include "LS.h"
#include <stdlib.h>
#include "arm_math.h"


static float32_t LS_weights[] = LS_WEIGHTS;
static float32_t LS_pattern[] = {1, -1, -1, 1, 1, -1};

static void averageAndBound(float32_t * src, float32_t * dest, uint32_t length);

LS LS_init(Analog adc, SR sr){
  LS ls = malloc(sizeof(__LS)); 
  ls->adc = adc;
  ls->sr = sr;
	
	ls->state = LS_patternA;
	ls->rawReading = Analog_getValues(ls->adc);
  ls->weightedSum = 0;
	
	ls->newData = false;
	
  ls->oversampleNumber = 0;
  
  // Fill all arrays with zeros;
	for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++) {
    for(uint8_t j = 0; j < SENSOR_OVERSAMPLE; j++){
      ls->readingsPatternA[SENSOR_OVERSAMPLE * j + i] = 0;
      ls->readingsPatternB[SENSOR_OVERSAMPLE * j + i] = 0;
    }
		ls->processedReadings[i] = 0;
    ls->calibrationLow[i] = 0;
    ls->calibrationHigh[i] = 0.49;
	}
  
  return ls;
}


void LS_update(LS ls){
	float32_t * processedReadings;
	
  // Switch to state based on over sampling 
	switch(ls->state){
		case LS_patternA:
			processedReadings = ls->readingsPatternA;
			
			break;
		case LS_patternB:
			processedReadings = ls->readingsPatternB;
			break;
	}
	
	
	for(int i = 0; i< IR_SENSOR_COUNT; i++){
			processedReadings[IR_SENSOR_COUNT * ls->oversampleNumber + i]=ls->rawReading[i]/4096.0;
	}
  
  
	if(ls->state == LS_patternB){
    if(ls->oversampleNumber < (SENSOR_OVERSAMPLE - 1)){
      ls->oversampleNumber++;
    } else {
      ls->oversampleNumber = 0;
      
      float32_t filteredReadings[IR_SENSOR_COUNT * SENSOR_OVERSAMPLE];
      
      // Data processing going on here
      arm_sub_f32(ls->readingsPatternA, ls->readingsPatternB, filteredReadings, IR_SENSOR_COUNT * SENSOR_OVERSAMPLE); // Subtract the values of sensors in the two patterns (Should remove external light sources)
      
      averageAndBound(filteredReadings, ls->processedReadings, IR_SENSOR_COUNT); // Average the samples and keep the values in bounds;
      
      arm_dot_prod_f32(ls->processedReadings, LS_weights, IR_SENSOR_COUNT, &ls->weightedSum);
      
      
      
      
      ls->newData = true;
    }
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
  arm_sub_f32(ls->processedReadings, ls->calibrationLow, values, IR_SENSOR_COUNT);
  for(int i = 0; i < IR_SENSOR_COUNT; i++){
    values[i] /= (ls->calibrationHigh[i] - ls->calibrationLow[i]);
                
    if(values[i] > 1)
      values[i] = 1;
    else if(values[i] < 0)
      values[i] = 0;
  }
}

bool LS_catchLine(LS ls){
  static uint32_t lcConfidence = 0;
	if(ls->processedReadings[1] > 0.6){
		lcConfidence += 1;
	}
	
	if(lcConfidence > 4){
		return true;
	} else {
		return false;
	}

}


void averageAndBound(float32_t * src, float32_t * dest, uint32_t length){
  uint32_t i = length;
  while(i--){
    dest[i] = (src[i] + src[i + IR_SENSOR_COUNT])/ 2.0;

  }
  
  arm_mult_f32(dest, LS_pattern, dest, length); // Flip the negative patterns
  i = length;
  while(i--){
    if(dest[i] < 0) dest[i] = 0;
    if(dest[i] > 1) dest[i] = 1;
  }
}


void LS_calibrate(LS ls){
	for(int i = 0; i< IR_SENSOR_COUNT; i++){
    if(ls->processedReadings[i] < ls->calibrationLow[i]){
      ls->calibrationLow[i] = ls->processedReadings[i];
    } else if(ls->processedReadings[i] > ls->calibrationHigh[i]){
      ls->calibrationHigh[i] = ls->processedReadings[i];
    }
  }
}

void LS_preCallibration(LS ls){
  for(uint8_t i = 0; i< IR_SENSOR_COUNT; i++){
    ls->calibrationLow[i] = 1;
    ls->calibrationHigh[i] = 0;
  }
}


void LS_reset(LS ls){
	ls->state = LS_patternA;
  ls->weightedSum = 0;
	
	ls->newData = false;
	
  ls->oversampleNumber = 0;
  
}
