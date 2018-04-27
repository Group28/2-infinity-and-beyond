#include "main.h"



#ifndef __ENCODERS_H
#define __ENCODERS_H

/*
 * Encoder handle
 *
 * Used to interface with encoders
 */
typedef struct {
	double speed; 				// Revolutions per second
	double samplePeriod; 	// Time between measuremnts
	int32_t lastCount; 		// Last value of counter
	uint16_t ticks_per_revolution; // Encoder counts per revolution
	double revolutions; 	// Number of total revolutions
	TIM_TypeDef * timer; 	// Timer attached to encoders 
} __Encoder, *Encoder;

/*
 * Initialize the encoder
 *
 * @param timer - hardware timer used as encoder counter
 * @param samplePeriod - time between encoder samples
 * @param ticks - counts per revolution
 * @return handle to encoder
 */
Encoder Encoder_init(TIM_TypeDef * timer, double samplePeriod, uint16_t ticks);

/*
 * Get current angular speed of rotation
 *
 * @param encoder - Encoder handle
 * @return speed in revolutions per second
 */
double Encoder_getAngularSpeed(Encoder encoder);

/*
 * Get current speed of rotation
 *
 * @param encoder - Encoder handle
 * @return speed [m/s]
 */
double Encoder_getSpeed(Encoder encoder);

/*
 * Get number of revolutions since the reset
 *
 * @param encoder - encoder handle
 * @return number of revolutions since last reset
 */
double Encoder_getRevolutions(Encoder encoder);


/*
 * Get distance traveled
 *
 * @param encoder - encoder handle
 * @return distance traveled by wheel [m]
 */
double Encoder_getDistance(Encoder encoder);

/*
 * Reset the revolutions counter and hardware counter
 *
 * @param encoder - Encoder handle
 */
void Encoder_reset(Encoder encoder);

/*
 * Update function - should be called at regular interval to update speed samples
 *
 * @param encoder - Encoder handle
 */
void Encoder_update(Encoder encoder);


#endif
