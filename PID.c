#include "PID.h"
#include "stdlib.h"

 


PID *PID_init(PID_Values values){
	PID * pid = (PID*)malloc(sizeof(PID));
	pid->values = values;
	pid->lastError = 0;
	pid->error = 0;
	return pid;
}

double PID_update(PID *pid, double error){
	double effort = (pid->values.P * error) + 
									(pid->values.I * pid->error) + 
									(pid->values.D * (pid->lastError - error));
	pid->error += error;
	pid->lastError = error;
	return effort;
}
void PID_setP(PID *pid, double P){
	pid->values.P = P;
}
void PID_setI(PID *pid, double I){
	pid->values.I = I;
}
void PID_setD(PID *pid, double D){
	pid->values.D = D;
}

