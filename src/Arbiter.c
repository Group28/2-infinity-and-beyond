#include "Arbiter.h"
#include "IO.h"
#include "Encoders.h"
#include "configuration.h"
#include "stdlib.h"
#include "Memory.h"
#include "math.h"

// Angular velocity thresholds
#define MINOR_SPEED_TH 1.5        // Minor vs Major turn
#define STRAIGHT_SPEED_TH 1 // Straight speed

static void Arbiter_updateCurrentAction(Arbiter arbiter);
static void Arbiter_startNewAction(Arbiter arbiter, uint8_t action);
static float Arbiter_getCurrentActionDistance(Arbiter arbiter);

static uint32_t calibrationCounter = 0; // Counter used for calibration procedure

Arbiter Arbiter_init(LF lf, LS ls, Magnet magnet, Motors motors, Memory memory){
  Arbiter arbiter = malloc(sizeof(__Arbiter));
	
  
  arbiter->state = STATE_READY;
	arbiter->lf = lf;
	arbiter->ls = ls;
	arbiter->magnet = magnet;
	arbiter->motors = motors;
  arbiter->memory = memory;
  
  arbiter->latestAction = NULL;
  arbiter->latestActionStartDistance = 0;
	
	arbiter->turningDistance = 0;
  
  arbiter->counter = 0;
  
  arbiter->speed = FORWARD_SPEED;
  
  arbiter->dAngle = RAf_init(10);
  
  
  arbiter->lostCounter = 0;
  
	
	return arbiter;
}

void Arbiter_update(Arbiter arbiter){
  static int stopCount = 0;
  arbiter->counter++; // Increase loop counter
  float speed = arbiter->speed;
	switch(arbiter->state){ // Decide on state
		case STATE_READY:  // Ready state
			//IO_set(IO_MOTOR_EN, 0);
      Magnet_update(arbiter->magnet);
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
    
      
      if(Motors_getLinearSpeed(arbiter->motors) < 0.4){
        speed *= 1.5;
      }
			
      
      // Do line following
      LF_setSpeed(arbiter->lf, speed);
			LF_update(arbiter->lf);
      
      // Add current actio to memmory if change
			Arbiter_updateCurrentAction(arbiter);
      
      
      
      // If lost, change to lost state;
			if(LF_lost(arbiter->lf)){
				arbiter->state = STATE_LOST_F;
        arbiter->lostCounter = arbiter->counter;
        Arbiter_startNewAction(arbiter, ACTION_LOST);
        break;
			}
			
	
			break;
      
      
		case STATE_BACK_TRACK:
			IO_set(IO_MOTOR_EN, 1);
      
      if(Motors_getLinearSpeed(arbiter->motors) > 1){
        speed *= -0.3;
      }
		  
      // Do line following
      LF_setSpeed(arbiter->lf, speed);
			LF_update(arbiter->lf);
      
      
      // If lost, change to lost state 
      if(LF_lost(arbiter->lf)){
        arbiter->state = STATE_LOST_B;
        stopCount = arbiter->counter;
      
        break;
      }
			break;
      
      
		case STATE_TURN:
			IO_set(IO_MOTOR_EN, 1);
		  
      // Catch the line or bail after set distance
			if(LS_catchLine(arbiter->ls) || Encoder_getRevolutions(arbiter->motors->motorLeft->encoder)-arbiter->turningDistance > ROTATION_DISTANCE){
				arbiter->state = STATE_BACK_TRACK;
        Motors_setSpeed(arbiter->motors, 1, 1);
				LF_reset(arbiter->lf);
				break;
			}
			
			Motors_setSpeed(arbiter->motors, ROTATION_SPEED, -ROTATION_SPEED);
			
		
			break;
      
      
		case STATE_LOST_F:
			IO_set(IO_MOTOR_EN, 1);
		  // Look for line in latest direction
      
      LF_setSpeed(arbiter->lf, FORWARD_SPEED_LOST);
      LF_update(arbiter->lf);
      
      if(!LF_lost(arbiter->lf)){
        Arbiter_startNewAction(arbiter, ACTION_STRAIGHT);
        arbiter->state = STATE_FORWARD_TRACK;
      }
    
			break;
      
      
		case STATE_LOST_B:
			IO_set(IO_MOTOR_EN, 1);
      Motors_setSpeed(arbiter->motors, -4, -4);
      if(stopCount > 600)
			   arbiter->state = STATE_STOP;
      stopCount++;
		
			break;
      
    case STATE_CALIBRATE:
      IO_set(IO_MOTOR_EN, 0);
      LS_calibrate(arbiter->ls);
      
      if(++calibrationCounter > CALIBRATION_TIME * SENSOR_SAMPLE_FREQ){
        arbiter->state = STATE_READY;
      }
      break; 
    case STATE_RC:
      break;
      
		case STATE_STOP:
		default:
			IO_set(IO_MOTOR_EN, 0);
      Motors_setSpeed(arbiter->motors, 0, 0);
			break;
	}

}

void Arbiter_startCalibration(Arbiter arbiter){
  LS_preCallibration(arbiter->ls);
  arbiter->state = STATE_CALIBRATE;
  calibrationCounter = 0;
  
}

float Arbiter_calibrationDone(Arbiter arbiter){
  if(calibrationCounter == 0) {
    return 100.0;
  } else {
    return 100.0 * calibrationCounter/(CALIBRATION_TIME * SENSOR_SAMPLE_FREQ);
  }
  
}


void Arbiter_reset(Arbiter arbiter){
	arbiter->state = STATE_READY;
	
  
  // Free latest action preventing memory leak
  if(arbiter->latestAction != NULL)
    free(arbiter->latestAction);
  
	arbiter->latestAction = NULL;
  arbiter->latestActionStartDistance = 0;
	
	arbiter->turningDistance = 0;
  
  arbiter->counter = 0;
}

void Arbiter_startRace(Arbiter arbiter) {
  Arbiter_reset(arbiter);
  
  // Reset all periphs
  Motors_reset(arbiter->motors);
  LS_reset(arbiter->ls);
  LF_reset(arbiter->lf);
  Memory_clear(arbiter->memory);
  
  
  // Free latest action 
  if(arbiter->latestAction != NULL)
    free(arbiter->latestAction);
  
  // Push Start Action to the beginning of the Memory
  Memory_pushAction(arbiter->memory, ACTION_START, 0);
  
  // Make latest action going straight
  arbiter->latestAction = Action_init(ACTION_STRAIGHT, 0);
  
  arbiter->counter = 0; // Reset loop counter
  
  arbiter->state = STATE_FORWARD_TRACK;
}


void Arbiter_updateCurrentAction(Arbiter arbiter){
  if(Arbiter_getCurrentActionDistance(arbiter) > 0.1) {
    double speedL = Encoder_getSpeed(arbiter->motors->motorLeft->encoder);
    double speedR = Encoder_getSpeed(arbiter->motors->motorRight->encoder);
    RAf_push(arbiter->dAngle, 2 * (speedL - speedR)/(speedL+speedR)/ BUGGY_WIDTH); // Angular velocity of truning - Positive turning Right, Negative Turning Left
    float dAngle = RAf_getAverage(arbiter->dAngle);
    // dAngle = dAngle/arbiter->speed; // Normalize dAngle with speed;
    
    if(fabs(dAngle) < STRAIGHT_SPEED_TH){ // Speed the same -> straight
      if(arbiter->latestAction->actionType != ACTION_STRAIGHT){
        Arbiter_startNewAction(arbiter, ACTION_STRAIGHT);
      }
    } else if(dAngle > -MINOR_SPEED_TH) {// Left slightly faster
      if(arbiter->latestAction->actionType != ACTION_MINOR_L){
        Arbiter_startNewAction(arbiter, ACTION_MINOR_L);
      }
    } else if(dAngle < MINOR_SPEED_TH) { // Right slightly faster
      if(arbiter->latestAction->actionType != ACTION_MINOR_R){
        Arbiter_startNewAction(arbiter, ACTION_MINOR_R);
      }
    } else if(dAngle < -MINOR_SPEED_TH) { // Left much faster
      if(arbiter->latestAction->actionType != ACTION_MAJOR_L){
        Arbiter_startNewAction(arbiter, ACTION_MAJOR_L);
      }
    } else if(dAngle > MINOR_SPEED_TH) { // Right much faster
      if(arbiter->latestAction->actionType != ACTION_MAJOR_R){
        Arbiter_startNewAction(arbiter, ACTION_MAJOR_R);
      }
    }
  }
  
}

void Arbiter_startNewAction(Arbiter arbiter, uint8_t action){
  arbiter->latestAction->distance = Arbiter_getCurrentActionDistance(arbiter); // Compute distance of latest action
  Memory_push(arbiter->memory, arbiter->latestAction);
  arbiter->latestAction = Action_init(action, 0);
  arbiter->latestActionStartDistance = Motors_getLinearDistance(arbiter->motors);
}

float Arbiter_getCurrentActionDistance(Arbiter arbiter){
  return (float) Motors_getLinearDistance(arbiter->motors) - arbiter->latestActionStartDistance;
}
