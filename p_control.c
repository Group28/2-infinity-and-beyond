#include "mbed.h"
#include "PwmOut.h"
#include "DigitalOut.h"

#define LEFT_SCALE 0.9
#define RIGHT_SCALE 1

#define LEFT_BIAS 0
#define RIGHT_BIAS 0.03

//Pins for sensors
AnalogIn sensor0 (PC_0);
AnalogIn sensor1 (PC_1);
AnalogIn sensor2 (PC_2);
AnalogIn sensor3 (PC_3);
AnalogIn sensor4 (PC_4);
AnalogIn sensor5 (PC_5); 
AnalogIn magnetic (PB_1);

//Motor setting
PwmOut motorLeft(PC_8);
PwmOut motorRight(PC_9);
DigitalOut motorLeftBp(PC_7);
DigitalOut motorRightBp(PB_10);
DigitalOut motorLeftDir(PB_5);
DigitalOut motorRightDir(PA_10);
DigitalOut motorEn(PA_5);

void p_control(double error1);
void turn_around ();



double Kp = 0.1;


int main() {
   
    
 
    
    while(1) {
        double voltage0 = sensor0.read();
        double voltage1 = sensor1.read();
        double voltage2 = sensor2.read();
        double voltage4 = sensor4.read();
        double magnetvoltage = magnetic.read();
        double error1=voltage1-voltage2;
        
        if (magnetvoltage>3) //need to change value
        turn_around();
        else {
        
        
        if(voltage0>2.5 || voltage4>2.5) 
        p_control(error1);
        else
        motorEn = 0;
        }
        wait(1);
    }
}


    
    
void p_control(double error1) {
      motorEn = 1;
      motorLeftDir = 0;
      motorRightDir = 0;
      motorLeft.write(1-(0.5*LEFT_SCALE+LEFT_BIAS)+Kp*error1);
      motorRight.write(1-(0.5*RIGHT_SCALE+RIGHT_BIAS)-Kp*error1);
      }
      
void turn_around () {
      motorEn = 1;
      motorLeftDir = 0;
      motorRightDir = 0;
      motorLeft.write(1);
      motorRight.write(0);
      }
      