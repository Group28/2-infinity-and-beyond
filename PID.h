
#ifndef __PID_H
#define __PID_H


typedef struct {
	double P;
	double I;
	double D;
} PID_Values;


typedef struct {
	PID_Values values;
	double lastError;
	double error;
} PID;


PID * PID_init(PID_Values values);

double PID_update(PID *pid, double error);
void PID_setP(PID *pid, double P);
void PID_setI(PID *pid, double I);
void PID_setD(PID *pid, double D);

#endif

