#include "main.h"
#include <stdbool.h>


#ifndef __ENCODERS_H
#define __ENCODERS_H


typedef struct {
	double speed;
	double samplePeriod;
	int lastCount;
	double revolutions;
} Encoder;


Encoder * Configure_Encoder_Left(void);
Encoder * Configure_Encoder_Right(void);

double Encoder_getSpeed(Encoder * encoder);


int Get_Encoder_Left(void);
int Get_Encoder_Right(void);




#endif
