#include "DigitalOut.h"
#include "PwmOut.h"
#include "QEI.h"
#include <PID.h>
#include "Timer.h"
#include "mbed.h"
#include "PinDetect.h"
#define LEFT_SCALE 1
#define RIGHT_SCALE 1
#define DEMOTIME 1

#define SAMPLETIME 0.01
void setLeftMotor(double val);
void setRightMotor(double val);

Ticker tick;
//PID motorLeftPID(-0.5, 0.5, 0, SAMPLETIME);
//PID motorRightPID(-0.5, 0.5, 0, SAMPLETIME);

PwmOut motorLeft(PC_8);
PwmOut motorRight(PC_9);

DigitalOut motorLeftBp(PC_7);
DigitalOut motorRightBp(PB_10);

DigitalOut motorLeftDir(PB_5);
DigitalOut motorRightDir(PA_10);

DigitalOut motorEn(PA_5);

PinDetect startBtn(PC_13);


Serial esp(PA_11, PA_12);

QEI leftWheel(PA_0, PA_1, NC, 1024);
QEI rightWheel(PA_15, PB_3, NC, 1024);

double motorLeftSpeed = 0;
double motorRightSpeed = 0;

enum Direction {
  forward,
  backward,
  leftFturn,
  rightFturn,
  leftBturn,
  rightBturn,
  stop
};

double speed = 0.5;
Direction direction = stop;
bool demoRunning = false;


void startDemo(){
  demoRunning = true;
  
  // Move forward
  motorEn = 1;
  setLeftMotor(0.2*LEFT_SCALE);
  setRightMotor(0.2*RIGHT_SCALE);
  
  wait(DEMOTIME);
  //Stop
  setLeftMotor(0);
  setRightMotor(0);
  
  wait(0.2*DEMOTIME);
  
  // Move Back
  motorEn = 1;
  setLeftMotor(-0.2*LEFT_SCALE);
  setRightMotor(-0.2*RIGHT_SCALE);
  
  wait(DEMOTIME);
  setLeftMotor(0);
  setRightMotor(0);
  
  wait(0.2*DEMOTIME);
  
  // Move Rigth
  motorEn = 1;
  setLeftMotor(0.2*LEFT_SCALE);
  setRightMotor(0.05*RIGHT_SCALE);
  
  wait(DEMOTIME);
  setLeftMotor(0);
  setRightMotor(0);
  
  wait(0.2*DEMOTIME);
  
  
  // Move Right Back
  motorEn = 1;
  setLeftMotor(-0.05*LEFT_SCALE);
  setRightMotor(-0.2*RIGHT_SCALE);
  
  wait(DEMOTIME);
  setLeftMotor(0);
  setRightMotor(0);
  
  wait(0.2*DEMOTIME);
  
  // Move Left
  motorEn = 1;
  setLeftMotor(0.05*LEFT_SCALE);
  setRightMotor(0.2*RIGHT_SCALE);
  
  wait(DEMOTIME);
  setLeftMotor(0);
  setRightMotor(0);
  
  wait(0.2*DEMOTIME);
  
  
  // Move Left Back
  motorEn = 1;
  setLeftMotor(-0.05*LEFT_SCALE);
  setRightMotor(-0.2*RIGHT_SCALE);
  wait(DEMOTIME);
  demoRunning = false;
}


void updateMotors(Direction dir) {
  switch (dir) {
    case forward:
      motorEn = 1;
      setLeftMotor(speed);
      setRightMotor(speed);
      break;
    case backward:
      motorEn = 1;
      setLeftMotor(-speed);
      setRightMotor(-speed);
      break;
    case leftFturn:
      motorEn = 1;
      setLeftMotor(speed);
      setRightMotor(0.5 * speed);
      break;
    case rightFturn:
      motorEn = 1;
      setLeftMotor(0.5*speed);
      setRightMotor(speed);
      break;
    case leftBturn:
      break;
    case rightBturn:
      break;
    case stop:
      setLeftMotor(0);
      setRightMotor(0);
      break;
  }
}

void updatePID(void){
     static int lastPulsesLeft = 0;
     static int lastPulsesRight = 0;
     static float effortLeft  = 0;
     static float effortRight = 0;
     
     static int itter = 0;
     itter++; 
     int currentPulsesLeft = leftWheel.getPulses();
     int currentPulsesRight = rightWheel.getPulses();
     
     double leftSpeed = (currentPulsesLeft - lastPulsesLeft) / SAMPLETIME;
     double rightSpeed = -(currentPulsesRight - lastPulsesRight) / SAMPLETIME;
     lastPulsesLeft = currentPulsesLeft;
     lastPulsesRight = currentPulsesRight;
        
     effortRight += 0.5 * (motorRightSpeed - (rightSpeed/5000));
     effortLeft += 0.5 * (motorLeftSpeed - (rightSpeed/5000));
     
     if(effortRight >= 1) effortRight = 1;
     if(effortLeft >= 1) effortLeft = 1;
     if(effortRight <= -1) effortRight = -1;
     if(effortLeft <= -1) effortLeft = -1;
     //motorLeftPID.setProcessValue(leftSpeed/5000.0);
     //motorRightPID.setProcessValue(rightSpeed/5000.0);
     
     //double efforLeft = motorLeftPID.compute();
     //double efforRight = motorRightPID.compute();
     if(effortLeft < 0) {
         motorLeftDir = 1;
     } else {
         motorLeftDir = 0;    
     }
     if(effortRight < 0) {
         motorRightDir = 1;
     } else {
         motorRightDir = 0;    
     }
     
     motorLeft.write(1-fabs(effortLeft));
     motorRight.write(1-fabs(effortRight));
     
     if(itter%((int)(1/SAMPLETIME)*2) == 0){
        esp.printf("Speed Left: %lf\\n\n", leftSpeed/5000);
        esp.printf("Speed Right: %lf\\n\n", rightSpeed/5000);
        esp.printf("Computed L R: %lf %lf \\n\n",effortLeft,effortRight);
        esp.printf("Desired Speed: %lf %lf \\n\n", motorLeftSpeed, motorRightSpeed);
     }
}

void setRightMotor(double val){
    //if(val < 0) {
    //    motorRightDir = 1;    
    //} else {
    //    motorRightDir = 0;  
    //}
    motorRightSpeed = val;
    //motorRightPID.setSetPoint(val);
    
}

void setLeftMotor(double val){
    //if(val < 0) {
    //    motorLeftDir = 1;    
    //} else {
    //    motorLeftDir = 0;  
    //}
    motorLeftSpeed = val;
    //motorLeftPID.setSetPoint(val);
}

int main() {
  esp.baud(115200);
  //esp.attach(&callback);
  
  //motorLeftPID.setInputLimits(-1, 1);
  //motorLeftPID.setOutputLimits(-1, 1);
  //motorRightPID.setInputLimits(-1, 1);
  //motorRightPID.setOutputLimits(-1, 1);
  
  
  startBtn.mode(PullUp);
  startBtn.attach_asserted(&startDemo);
  startBtn.setSampleFrequency();
  
  tick.attach(&updatePID, SAMPLETIME);
  
  motorLeftBp = 0;
  motorRightBp = 0;
   
  motorLeftDir = 0;
  motorRightDir = 0;
   
  motorLeft.period(1.0/20000);
  motorRight.period(1.0/20000);
   
  motorEn = 0;
  
  while (1) {
    if (esp.readable() && !demoRunning) {
      switch (esp.getc()) {
      case '\n':
        break;
      case 'w':
        direction = forward;
        break;
      case 's':
        direction = backward;
        break;
      case 'a':
        direction = leftFturn;
        break;
      case 'd':
        direction = rightFturn;
        break;
      case 'f':
        direction = stop;
        break;
      case 'q':
        if (speed + 0.1 <= 1)
          speed += 0.1;
        break;
      case 'e':
        if (speed - 0.1 >= 0)
          speed -= 0.1;
        break;
      case 'x':
        startDemo();
        break;
      }
    }
    updateMotors(direction);
  }
}
