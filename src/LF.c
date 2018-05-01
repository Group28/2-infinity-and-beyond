#include "LF.h"
#include <stdlib.h>
#include "configuration.h"

static PID_Values ctrlPID_values = {0.5, 0, 0.001};

LF LF_init(Motors motors, LS ls){
	LF lf = malloc(sizeof(__LF));
	
	lf->motors = motors;
	lf->ls = ls;
	lf->speed = FORWARD_SPEED;
	lf->lost = false;
	lf->lostConfidence = 0;
	lf->effort = 0;
	lf->last = -1;

	lf->ctrl = PID_init(ctrlPID_values, 1/(SENSOR_SAMPLE_FREQ), -1, 1);
	PID_setTargetValue(lf->ctrl, 0);
		
	return lf;

}

void LF_update(LF lf){
	float position;
	float32_t sensorValues[IR_SENSOR_COUNT];
	
	LS_getProcessedValues(lf->ls, sensorValues);
	position = LS_computePosition(lf->ls);
	
	float32_t maxValue = 0;
	uint32_t index = 0;		
	arm_max_f32(sensorValues, IR_SENSOR_COUNT, &maxValue, &index);
		

	if(maxValue < 0.5){
		lf->effort = lf->last * 2.5;
		lf->lostConfidence += 1;
		if(lf->lostConfidence > 60) {
			lf->lost = true;
		}
		//lf->effort *= 1.1;
		
	} else {
		lf->lost = false;
		lf->lostConfidence = 0;
		PID_setMeasuredValue(lf->ctrl, position);
		lf->effort = PID_compute(lf->ctrl);
	}
	
	float motorLeftSpeed =  lf->speed * (0.9 - lf->effort);
	float motorRightSpeed = lf->speed * (0.9 + lf->effort);
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
	lf->effort = 0;
	lf->lostConfidence = 0;
}
