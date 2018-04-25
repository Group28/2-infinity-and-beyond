#include "main.h"
#include "LS.h"
#include "Motors.h"
#include "Magnet.h"
#include "Memory.h"
#include "LF.h"

#ifndef __ARBITER_H
#define __ARBITER_H

enum buggy_state {
	STATE_READY,
	STATE_FORWARD_TRACK,
	STATE_TURN,
	STATE_BACK_TRACK,
	STATE_STOP,
	STATE_LOST_F,
	STATE_LOST_B
};

typedef struct {
	LF lf;
	LS ls;
	Magnet magnet;
	Motors motors;
	Memory memory;
	float turningDistance;
  enum buggy_state state;
	Action * latestAction;
	
	uint32_t counter;
} __Arbiter, *Arbiter;


Arbiter Arbiter_init(LF lf, LS ls, Magnet magnet, Motors motors, Memory memory);

void Arbiter_update(Arbiter arbiter);


#endif
