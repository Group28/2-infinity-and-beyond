#include "Motors.h"
#include "Encoders.h"
#include "main.h"
#include "configuration.h"
#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"
#include "IO.h"
#include "math.h"

/**
 * Sets the PWM duty cycle of a motor according to the effort
 *
 * @param motor - Motor handle
 * @return 
 */
static void Motor_setPWMDutyCycle(Motor motor);


Motor Motor_init(PID_Values values, Encoder encoder, uint8_t position){
	Motor motor = malloc(sizeof(__Motor));
	motor->pid = PID_init(values, MOTOR_SAMPLE_FREQ, -1,1);
	motor->speed = 0;
	motor->position = position;
	motor->encoder = encoder;
	
	
	return motor;
}

Motors Motors_init(PID_Values valuesL, PID_Values valuesR, Encoder encL, Encoder encR){
	Motors motors = malloc(sizeof(__Motors));
	motors->motorLeft = Motor_init(valuesL, encL, MOTOR_LEFT);
	motors->motorRight = Motor_init(valuesR, encR, MOTOR_RIGHT);
	
	return motors;
}

void Motor_PID_action(Motor motor){
	double realSpeed =  Encoder_getAngularSpeed(motor->encoder);
	PID_setMeasuredValue(motor->pid, realSpeed);
	
	motor->effort = PID_compute(motor->pid);
	

	
	Motor_setPWMDutyCycle(motor);
}



void Motors_PID_action(Motors motors){
	Motor_PID_action(motors->motorLeft);
	Motor_PID_action(motors->motorRight);
}

void Motor_setSpeed(Motor motor, double speed){
	motor->speed = speed;
	PID_setTargetValue(motor->pid, speed);
}

void Motors_setSpeed(Motors motors, double speedL, double speedR){
	Motor_setSpeed(motors->motorLeft, speedL);
	Motor_setSpeed(motors->motorRight, speedR);
}


void Motor_setPWMDutyCycle(Motor motor){
	double effort = motor->effort;
	uint8_t direction = MOTOR_FORWARD;
	if(effort < 0){
		effort = fabs(effort);
		direction = MOTOR_REVERSE;
		
	}
	
	uint32_t compareValue = (uint32_t)((1.0-effort)*(LL_TIM_GetAutoReload(TIM3)+1));
	
	if(motor->position == MOTOR_LEFT){
		LL_TIM_OC_SetCompareCH3(TIM3, compareValue);
		IO_set(IO_MOTOR_1_DIR, direction);
	} else if(motor->position == MOTOR_RIGHT){
		LL_TIM_OC_SetCompareCH4(TIM3, compareValue);
		IO_set(IO_MOTOR_2_DIR, direction);
	}
}

void Motors_reset(Motors motors){
	PID_reset(motors->motorLeft->pid);
	PID_reset(motors->motorRight->pid);
	
	Encoder_reset(motors->motorLeft->encoder);
	Encoder_reset(motors->motorRight->encoder);

}


double Motor_getDistance(Motor motor){
	return Encoder_getDistance(motor->encoder);
}


double Motors_getLinearDistance(Motors motors){
	return (Encoder_getDistance(motors->motorLeft->encoder) + Encoder_getDistance(motors->motorRight->encoder))/ 2.0;
}
