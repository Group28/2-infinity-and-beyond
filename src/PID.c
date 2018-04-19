#include "PID.h"
#include "stdlib.h"

 


PID PID_init(PID_Values values, double interval, double minEffort, double maxEffort){
	PID pid = malloc(sizeof(__PID));
	pid->values = values;
	pid->lastError = 0;
	pid->error = 0;
  pid->accError = 0;
  
  pid->measured = 0;
  pid->target = 0;
  
  pid->interval = interval;
  pid->minEffort = minEffort;
  pid->maxEffort = maxEffort;
  
	return pid;
}

float PID_compute(PID pid){
  pid->lastError = pid->error; // Set last error
  
  pid->error = pid->target - pid->measured; // Calculate current error from target and measured values
  
  pid->accError += pid->error * pid->interval; // Accumulate error proportional to the update interval
  
  
	double effort = (pid->values.P * pid->error) + 
									(pid->values.I * pid->accError) + 
									(pid->values.D * ((pid->error - pid->lastError)/pid->interval)); // Compute the effort
	
  
  if(effort >= pid->maxEffort) return pid->maxEffort; // Effort is bound by the max and min Effort values
  if(effort <= pid->minEffort) return pid->minEffort;
  
	return (float)effort;
}

void PID_reset(PID pid){
  pid->lastError = 0;
  pid->error = 0;
  pid->accError = 0;
}


void PID_setMeasuredValue(PID pid, double value){
  pid->measured = value;
}
void PID_setTargetValue(PID pid, double value){
  pid->target = value;
}

void PID_setP(PID pid, double P){
	pid->values.P = P;
}
void PID_setI(PID pid, double I){
	pid->values.I = I;
}
void PID_setD(PID pid, double D){
	pid->values.D = D;
}

void PID_setMinEffor(PID pid, double minEffort){
  pid->minEffort = minEffort;
}
void PID_setMaxEffor(PID pid, double maxEffort){
  pid->maxEffort = maxEffort;
}

double PID_getP(PID pid){
  return pid->values.P;
}
double PID_getI(PID pid){
  return pid->values.I;
}
double PID_getD(PID pid){
  return pid->values.D;
}
