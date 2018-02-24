#include "Motors.h"

#include "main.h"

#include <stdbool.h>
#include <stdlib.h>
#include "math.h"


Motor * Motor_init(PID_Values values){
	Motor * motor = (Motor*) malloc(sizeof(Motor));
	motor->pid = PID_init(values);
	motor->speed = 0;
	Configure_Motor_Right();
	
	
	return motor;
}

Motors * Motors_init(PID_Values valuesL, PID_Values valuesR){
	Motors * motors = (Motors*) malloc(sizeof(Motors));
	motors->motorLeft = Motor_init(valuesL);
	motors->motorRight = Motor_init(valuesR);
	
	return motors;
}

void Motor_Left_PID_action(Motor * motor){
	double realSpeed =  Encoder_getSpeed(motor->encoder);
}

void Motor_Right_PID_action(Motor * motor){

}



void Motors_PID_action(Motors * motors){
	Motor_Left_PID_action(motors->motorLeft);
	Motor_Right_PID_action(motors->motorRight);
}

void Motor_Left_setSpeed(Motor * motor, double speed){
	motor->speed = speed;
}
void Motor_Right_setSpeed(Motor * motor, double speed){
	motor->speed = speed;
}

void Motors_setSpeed(Motors * motors, double speedL, double speedR){
	Motor_Left_setSpeed(motors->motorLeft, speedL);
	Motor_Right_setSpeed(motors->motorRight, speedR);
}


void Set_DutyCycle_Motor_Left(float D){
	float P;    //Pulse duration                                           
  float T;    //PWM signal period  
	D = fabs(D);
	
	/* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM3) + 1;
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = ((100.0f-D)*T)/100.0f;

  LL_TIM_OC_SetCompareCH3(TIM3, (uint32_t)P);	
}

void Set_DutyCycle_Motor_Right(float D){
	float P;    //Pulse duration                                           
  float T;    //PWM signal period  
	D = fabs(D);
	
	/* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM3) + 1;
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = ((100.0f-D)*T)/100.0f;

  LL_TIM_OC_SetCompareCH4(TIM3, (uint32_t)P);	
}
