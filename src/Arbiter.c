#include "Arbiter.h"
#include "IO.h"
#include "Encoders.h"
#include "configuration.h"
#include "stdlib.h"

Arbiter Arbiter_init(LF lf, LS ls, Magnet magnet, Motors motors, Memory memory){
  Arbiter arbiter = malloc(sizeof(__Arbiter));
	

  arbiter->state = STATE_READY;
	arbiter->lf = lf;
	arbiter->ls = ls;
	arbiter->magnet = magnet;
	arbiter->motors = motors;
  arbiter->memory = memory;
  
  arbiter->latestAction = Action_init(ACTION_START, 0);
	
	arbiter->turningDistance = 0;
  
  arbiter->counter = 0;
	
	return arbiter;
}

void Arbiter_update(Arbiter arbiter){
  arbiter->counter++; // Increase loop counter
	switch(arbiter->state){
		case STATE_READY:
			IO_set(IO_MOTOR_EN, 0);
      
      // Push Start Action to the beginning of the Memory
      if(Memory_getLength(arbiter->memory) != 1){
        Memory_clear(arbiter->memory);
        
        Memory_push(arbiter->memory, arbiter->latestAction);
        arbiter->latestAction = Action_init(ACTION_STRAIGHT, 0);
      }
      arbiter->counter = 0; // Reset loop counter
			break;
      
		case STATE_FORWARD_TRACK:
			IO_set(IO_MOTOR_EN, 1);
			
      // Check for Magnet update
			Magnet_update(arbiter->magnet);
			
      // Start Turning if Magnet detected
      if(Magnet_getValue(arbiter->magnet) != MAGNET_NOMAG){
				arbiter->state = STATE_TURN;
				arbiter->turningDistance = Encoder_getRevolutions(arbiter->motors->motorLeft->encoder);
				break;
			}
			
      
      // Do line following
			LF_update(arbiter->lf);
			
      
      // If lost, change to lost state;
			if(LF_lost(arbiter->lf)){
				arbiter->state = STATE_LOST_F;
        Memory_pushAction(arbiter->memory, ACTION_LOST, 0);
        break;
			}
			
	
			break;
      
      
		case STATE_BACK_TRACK:
			IO_set(IO_MOTOR_EN, 1);
		  
      // Do line following
			LF_update(arbiter->lf);
      
      
      // If lost, change to lost state 
      if(LF_lost(arbiter->lf)){
        arbiter->state = STATE_LOST_B;
      
        break;
      }
			break;
      
      
		case STATE_TURN:
			IO_set(IO_MOTOR_EN, 1);
		
      // Catch the line or bail after set distance
			if(LS_catchLine(arbiter->ls) || Encoder_getRevolutions(arbiter->motors->motorLeft->encoder)-arbiter->turningDistance > 1.8){
				arbiter->state = STATE_BACK_TRACK;
				LF_reset(arbiter->lf);
				break;
			}
			
			Motors_setSpeed(arbiter->motors, ROTATION_SPEED/1.5, -ROTATION_SPEED/1.5);
			
		
			break;
      
      
		case STATE_LOST_F:
			IO_set(IO_MOTOR_EN, 1);
		  // Look for line in latest direction
      
			//arbiter->state = STATE_STOP;
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
