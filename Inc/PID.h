
#ifndef __PID_H
#define __PID_H

// Structure used to represent the PID values for a PID controller
typedef struct {
  double P; // Proportional term
  double I; // Integral term
  double D; // Differential term
} PID_Values;


// PID controller handle
typedef struct {
  PID_Values values; // PID coefficints
  double lastError;  // Last calculated error
  double error;      // Current calculated error
  double accError;   // Accumulated error

  double measured; // Latest measured value from the feedback
  double target;   // Target value

  double interval; // Sample interval

  double minEffort; // Minimal effort
  double maxEffort; // Maximal effort

} __PID, * PID;

// Initialize a pointer to a PID controller
PID PID_init(PID_Values values, double interval, double minEffort,
              double maxEffort);

// Compute the effort based on the latest measured value and the target value
float PID_compute(PID pid);

// Reset the integral and derivative error counters
void PID_reset(PID pid);

// Set real value from the feedback
void PID_setMeasuredValue(PID pid, double value);
// Set the desired value expected on the output
void PID_setTargetValue(PID pid, double value);

// Update the PID values
void PID_setP(PID pid, double P);
void PID_setI(PID pid, double I);
void PID_setD(PID pid, double D);

// Set minimal and maximal effort
void PID_setMinEffor(PID pid, double minEffort);
void PID_setMaxEffor(PID pid, double maxEffort);

// Get last set measured value
double PID_getMeasuredValue(PID pid);

// Get last set target value
double PID_getTargetValue(PID pid);


// Get PID values
double PID_getP(PID pid);
double PID_getI(PID pid);
double PID_getD(PID pid);

#endif
