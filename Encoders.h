#include "main.h"
#include <stdbool.h>


#ifndef __ENCODERS_H
#define __ENCODERS_H


typedef struct {
	double speed;
} QEncoder;


double Encoder_getSpeed(QEncoder * encoder);

void Configure_Encoder_Left(void);
int Get_Encoder_Left(void);

void Configure_Encoder_Right(void);
int Get_Encoder_Right(void);




#endif
