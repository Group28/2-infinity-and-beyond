#include "main.h"
#include "LS.h"
#include "Motors.h"
#include "Magnet.h"
#include "Memory.h"
#include "LF.h"
#include "RA.h"

#ifndef __ARBITER_H
#define __ARBITER_H

/*
 * States of the arbiter state machine
 *
 * 
 */
enum buggy_state {
	STATE_READY,
	STATE_FORWARD_TRACK,
	STATE_TURN,
	STATE_BACK_TRACK,
	STATE_STOP,
	STATE_LOST_F,
	STATE_LOST_B,
	STATE_CALIBRATE,
	STATE_RC
};


/*
 * Arbiter handle
 *
 * used to represent the state machine
 */
typedef struct {
	LF lf;
	LS ls;
	Magnet magnet;
	Motors motors;
	Memory memory;
	float turningDistance;
  enum buggy_state state;
	
	Action * latestAction;
	double latestActionStartDistance;
	
	double speed;
	
	int lostCounter;
	
	RAf dAngle;
	
	uint32_t counter;
} __Arbiter, *Arbiter;

/*
 * Initialize the arbiter
 *
 * @return Ponter to the Arbiter handle
 */
Arbiter Arbiter_init(LF lf, LS ls, Magnet magnet, Motors motors, Memory memory);


/*
 * epdate the arbiter (depends on the current stat)
 *
 * @return non 
 */
void Arbiter_update(Arbiter arbiter);

/*
 * Reset Arbiter and all periphs to default state
 *
 * @return 
 */
void Arbiter_reset(Arbiter arbiter);


/*
 * Switch to start the race
 *
 * @return
 */
void Arbiter_startRace(Arbiter arbiter);


/*
 * Start calibration procedure
 *
 * @return
 */
void Arbiter_startCalibration(Arbiter arbiter);



/**
 * Calibration procedure percentage done
 *
 * @return percentage completion
 */
float Arbiter_calibrationDone(Arbiter arbiter);

#endif
