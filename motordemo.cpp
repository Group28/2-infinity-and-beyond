#include "DigitalOut.h"
#include "PwmOut.h"
#include "QEI.h"
#include "Timer.h"
#include "mbed.h"

#define LEFT_SCALE 1
#define RIGHT_SCALE 1
#define DEMOTIME 1

PwmOut motorLeft(PC_8);
PwmOut motorRight(PC_9);

DigitalOut motorLeftBp(PC_7);
DigitalOut motorRightBp(PB_10);

DigitalOut motorLeftDir(PB_5);
DigitalOut motorRightDir(PA_10);

DigitalOut motorEn(PA_5);

InterruptIn startBtn(PC_13);


Serial esp(PA_11, PA_12);

QEI leftWheel(PA_0, PA_1, NC, 256);
QEI rightWheel(PA_15, PB3, NC, 256);

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
  motorLeftDir = 0;
  motorRightDir = 0;
  motorLeft.write(0.7*LEFT_SCALE);
  motorRight.write(0.7*RIGHT_SCALE);
  
  wait(DEMOTIME);
  //Stop
  motorEn = 0;
  
  wait(0.2*DEMOTIME);
  
  // Move Back
  motorEn = 1;
  motorLeftDir = 1;
  motorRightDir = 1;
  motorLeft.write(0.7*LEFT_SCALE);
  motorRight.write(0.7*RIGHT_SCALE);
  
  wait(DEMOTIME);
  motorEn = 0;
  
  wait(0.2*DEMOTIME);
  
  // Move Rigth
  motorEn = 1;
  motorLeftDir = 0;
  motorRightDir = 0;
  motorLeft.write(0.7*LEFT_SCALE);
  motorRight.write(0.3*RIGHT_SCALE);
  
  wait(DEMOTIME);
  motorEn = 0;
  
  wait(0.2*DEMOTIME);
  
  
  // Move Right Back
  motorEn = 1;
  motorLeftDir = 1;
  motorRightDir = 1;
  motorLeft.write(0.7*LEFT_SCALE);
  motorRight.write(0.3*RIGHT_SCALE);
  
  wait(DEMOTIME);
  motorEn = 0;
  
  wait(0.2*DEMOTIME);
  
  // Move Left
  motorEn = 1;
  motorLeftDir = 0;
  motorRightDir = 0;
  motorLeft.write(0.3*LEFT_SCALE);
  motorRight.write(0.7*RIGHT_SCALE);
  
  wait(DEMOTIME);
  motorEn = 0;
  
  wait(0.2*DEMOTIME);
  
  
  // Move Left Back
  motorEn = 1;
  motorLeftDir = 1;
  motorRightDir = 1;
  motorLeft.write(0.3*LEFT_SCALE);
  motorRight.write(0.7*RIGHT_SCALE);
  
  demoRunning = false;
}


void updateMotors(Direction dir) {
  switch (dir) {
    case forward:
      motorEn = 1;
      motorLeftDir = 0;
      motorRightDir = 0;
      motorLeft.write(speed);
      motorRight.write(speed);
      break;
    case backward:
      motorEn = 1;
      motorLeftDir = 1;
      motorRightDir = 1;
      motorLeft.write(speed);
      motorRight.write(speed);
      break;
    case leftFturn:
      motorEn = 1;
      motorLeftDir = 0;
      motorRightDir = 0;
      motorLeft.write(speed);
      motorRight.write(0.5 * speed);
      break;
    case rightFturn:
      motorEn = 1;
      motorLeftDir = 0;
      motorRightDir = 0;
      motorLeft.write(0.5*speed);
      motorRight.write(speed);
      break;
    case leftBturn:
      break;
    case rightBturn:
      break;
    case stop:
      motorEn = 0;
      break;
  }
}

int main() {
  esp.baud(115200);
  //esp.attach(&callback);
  
  startBtn.rise(&startDemo);
  
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
