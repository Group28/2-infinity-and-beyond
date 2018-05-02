#include "PID.h"
#include "Encoders.h"
#include "configuration.h"


#ifndef __MOTORS_H
#define __MOTORS_H

// Motor position
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

#define MOTOR_FORWARD 1
#define MOTOR_REVERSE 0

/**
 * Motor handle
 *
 * interface for individual motors
 *
 */
typedef struct {
	PID pid;          //< PID controller
	Encoder encoder;  //< Encoder handle
	double speed;     //< Set speed
	double effort;    //< Current effort
	uint8_t position:1; //< Motor position (Left or Right)
} __Motor, *Motor;;


/**
 * Motors handle
 *
 * interface for both motors
 */
typedef struct {
	Motor motorLeft;   //< Left motor handle
	Motor motorRight;  //< Right motor handle
	
} __Motors, *Motors;


/**
 * Initialize one motor
 *
 * @param values - PID values for the motor controller
 * @param encoder - encoder of this motor
 * @param position - motor position (Left or Right)
 * @return Motor handle
 */
Motor Motor_init(PID_Values values, Encoder encoder, uint8_t position);

/**
 * Initialie both motors
 *
 * @param valuesL - PID values for left motor
 * @param valuesR - PID values for right motor
 * @param encL - Left encoder
 * @param encR - Right encoder
 * @return Motors handle
 */
Motors Motors_init(PID_Values valuesL, PID_Values valuesR, Encoder encL, Encoder encR);


/**
 * Sets speed for motor
 *
 * @param motor - Motor handle
 * @param speed - speed (revolutions per second)
 */
void Motor_setSpeed(Motor motor, double speed);

/**
 * Update the effort according the the PID feedback
 *
 * @param motor - Motor handle
 * @return 
 */
void Motor_PID_action(Motor motor);

/**
 * Sets speed for both motors
 *
 * @param motors - Motors handle
 * @param speedL - speed of left motor
 * @param speedR - speed of right motor
 */
void Motors_setSpeed(Motors motors, double speedL, double speedR);

/**
 * Update the effort according to the PID feedback for both motors
 *
 * @param motors handle
 * @return 
 */
void Motors_PID_action(Motors motors);

/**
 * Resets the PID controlle of the motors
 *
 * @param motors handle
 * @return 
 */
void Motors_reset(Motors motors);

/**
 * Gets distance traveled by motor
 *
 * @param motor handle
 * @return distance traveled [m]
 */
double Motor_getDistance(Motor motor);


	/**
	 * Gets linear distance traveled by both motors
	 *
	 * @param motors handle
	 * @return distance traveled [m]
	 */
double Motors_getLinearDistance(Motors motors);


double Motors_getLinearSpeed(Motors motors);
#endif
