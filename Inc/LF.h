#include "main.h"
#include "Motors.h"
#include "IO.h"
#include "LS.h"
#include "PID.h"


#ifndef __LF_H
#define __LF_H

typedef struct {
	Motors motors;
	LS ls;
	
	PID ctrl;
	bool lost;
	int lostConfidence;
	float speed;
	float effort;
	
	float values[10];
} __LF, *LF;

LF LF_init(Motors motors, LS ls);

void LF_update(LF lf);

void LF_setSpeed(LF lf, float speed);

bool LF_lost(LF lf);

void LF_reset(LF lf);

#endif
