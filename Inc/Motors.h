#include "PID.h"
#include "Encoders.h"
#include "configuration.h"


#ifndef __MOTORS_H
#define __MOTORS_H


typedef struct {
	PID pid;
	Encoder encoder;
	double speed;
} __Motor, *Motor;;

typedef struct {
	Motor motorLeft;
	Motor motorRight;
	
} __Motors, *Motors;;

Motor Motor_init(PID_Values values, Encoder encoder);

Motors Motors_init(PID_Values valuesL, PID_Values valuesR, Encoder encL, Encoder encR);



void Motor_Left_setSpeed(Motor motor, double speed);
void Motor_Right_setSpeed(Motor motor, double speed);
void Motor_Left_PID_action(Motor motor);
void Motor_Right_PID_action(Motor motor);

void Motors_setSpeed(Motors motors, double speedL, double speedR);
void Motors_PID_action(Motors motors);

void Configure_Motor_Right(void);
void Configure_Motor_Left(void);
void Set_DutyCycle_Motor_Left(float D);
void Set_DutyCycle_Motor_Right(float D);


#endif
