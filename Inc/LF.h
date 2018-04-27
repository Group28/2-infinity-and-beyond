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

/*
 * Initialize the Line folloer
 *
 * @return Ponter to the LF handle
 */
LF LF_init(Motors motors, LS ls);

/*
 * Update the line folloer
 *
 * @param Ponter to the LF handle
 */
void LF_update(LF lf);


/*
 * Set speed of line following
 *
 * @param LF handle
 */
void LF_setSpeed(LF lf, float speed);

/*
 * Are we lost
 *
 * @return Ponter to the Analog handle
 */
bool LF_lost(LF lf);

/*
 * Reset
 *
 * @return Ponter to the Analog handle
 */
void LF_reset(LF lf);



#endif
