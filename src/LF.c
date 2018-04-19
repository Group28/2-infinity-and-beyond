#include "LF.h"
#include <stdlib.h>

static PID_Values ctrlPID_values = {0.4, 0.00001, 0.00001};

LF LF_init(Motors motors, LS ls){
	LF lf = malloc(sizeof(__LF));
	
	lf->motors = motors;
	lf->ls = ls;
	lf->speed = 0.8;
	lf->lost = false;
	lf->lostConfidence = 0;
	lf->effort = 0;

	lf->ctrl = PID_init(ctrlPID_values, 1/(SENSOR_SAMPLE_FREQ/2), -1, 1);
	PID_setTargetValue(lf->ctrl, 0);
	
	
	return lf;

}

void LF_update(LF lf){
	float weightedSum;
	float32_t sensorValues[IR_SENSOR_COUNT];
	
	weightedSum = LS_getWeightedSum(lf->ls);
	
	LS_getProcessedValues(lf->ls, sensorValues);
	float32_t maxValue = 0;
	uint32_t index = 0;
			
	arm_max_f32(sensorValues, IR_SENSOR_COUNT, &maxValue, &index);
		
	if(maxValue < 0.5){
		lf->lostConfidence += 1;
		if(lf->lostConfidence > 60) {
			lf->lost = true;
		}
		lf->effort *= 1.1;
		
	} else {
		lf->lost = false;
		lf->lostConfidence = 0;
		PID_setMeasuredValue(lf->ctrl, weightedSum);
		lf->effort = PID_compute(lf->ctrl);
	
		
	}
	
	float motorLeftSpeed = (-lf->effort + 0.8) * lf->speed;
	float motorRightSpeed = (lf->effort + 0.8) * lf->speed;
	Motors_setSpeed(lf->motors, motorLeftSpeed, motorRightSpeed);
		
	
	
	
	
	
	
	
}

bool LF_lost(LF lf){
	return lf->lost;
}

void LF_setSpeed(LF lf, float speed){
	lf->speed = speed;
}

void LF_reset(LF lf){
	PID_reset(lf->ctrl);
	lf->lost = false;
}
