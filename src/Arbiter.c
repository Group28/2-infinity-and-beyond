#include "Arbiter.h"
#include "IO.h"
#include "Encoders.h"
#include "configuration.h"
#include "stdlib.h"

Arbiter Arbiter_init(LF lf, LS ls, Magnet magnet, Motors motors){
  Arbiter arbiter = malloc(sizeof(__Arbiter));
	
	arbiter->state = STATE_READY;

	arbiter->lf = lf;
	arbiter->ls = ls;
	arbiter->magnet = magnet;
	arbiter->motors = motors;
	
	arbiter->turningDistance = 0;
	
	return arbiter;
}

void Arbiter_update(Arbiter arbiter){
	switch(arbiter->state){
		case STATE_READY:
			IO_set(IO_MOTOR_EN, 0);
			break;
		case STATE_FORWARD_TRACK:
			IO_set(IO_MOTOR_EN, 1);
			
			Magnet_update(arbiter->magnet);
			if(Magnet_getValue(arbiter->magnet) != MAGNET_NOMAG){
				arbiter->state = STATE_TURN;
				arbiter->turningDistance = Encoder_getRevolutions(arbiter->motors->motorLeft->encoder);
				break;
			}
			
			LF_update(arbiter->lf);
			
			if(LF_lost(arbiter->lf)){
				arbiter->state = STATE_LOST_F;
			}
			
			
			break;
		case STATE_BACK_TRACK:
			IO_set(IO_MOTOR_EN, 1);
		
			if(LF_lost(arbiter->lf)){
				arbiter->state = STATE_LOST_B;
				break;
			}
			LF_update(arbiter->lf);
			break;
		case STATE_TURN:
			IO_set(IO_MOTOR_EN, 1);
		
			if(LS_catchLine(arbiter->ls) || Encoder_getRevolutions(arbiter->motors->motorLeft->encoder)-arbiter->turningDistance > 1.8){
				arbiter->state = STATE_BACK_TRACK;
				LF_reset(arbiter->lf);
				break;
			}
			
			Motors_setSpeed(arbiter->motors, ROTATION_SPEED/1.5, -ROTATION_SPEED/1.5);
			
		
			break;
		case STATE_LOST_F:
			IO_set(IO_MOTOR_EN, 1);
		
			arbiter->state = STATE_STOP;
			break;
		case STATE_LOST_B:
			IO_set(IO_MOTOR_EN, 1);
			arbiter->state = STATE_STOP;
		
			break;
		case STATE_STOP:
		default:
			IO_set(IO_MOTOR_EN, 0);
			break;
	}

}
