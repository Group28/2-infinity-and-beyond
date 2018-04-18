#include "PID.h"
#include "Encoders.h"
#include "configuration.h"


#ifndef __MOTORS_H
#define __MOTORS_H

#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

#define MOTOR_FORWARD 1
#define MOTOR_REVERSE 0


typedef struct {
	PID pid;
	Encoder encoder;
	double speed;
	double effort;
	uint8_t position:1;
} __Motor, *Motor;

typedef struct {
	Motor motorLeft;
	Motor motorRight;
	
} __Motors, *Motors;

Motor Motor_init(PID_Values values, Encoder encoder, uint8_t position);

Motors Motors_init(PID_Values valuesL, PID_Values valuesR, Encoder encL, Encoder encR);



void Motor_setSpeed(Motor motor, double speed);
void Motor_PID_action(Motor motor);

void Motors_setSpeed(Motors motors, double speedL, double speedR);
void Motors_PID_action(Motors motors);

void Configure_Motor_Right(void);
void Configure_Motor_Left(void);


#endif
