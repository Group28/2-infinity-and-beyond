#include "main.h"



#ifndef __ENCODERS_H
#define __ENCODERS_H


typedef struct {
	double speed; 				// Revolutions per second
	double samplePeriod; 	// Time between measuremnts
	int lastCount; 				// Last value of counter
	uint16_t ticks_per_revolution;
	double revolutions; 	// Number of total revolutions
	TIM_TypeDef * timer; 	// Timer attached to encoders 
} Encoder;


Encoder * Encoder_init(TIM_TypeDef * timer, double samplePeriod, uint16_t ticks_per_revolution);

double Encoder_getSpeed(Encoder * encoder);

double Encoder_getRevolutions(Encoder *encoder);

void Encoder_reset(Encoder *encoder);

void Encoder_update(Encoder *encoder);


#endif
