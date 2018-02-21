#include "mbed.h"
#include "PwmOut.h"
#include "Timer.h"
#include "DigitalOut.h"

//AnalogIn pot1 (A0);
PwmOut Motor1(PC_8);
PwmOut Motor2(PC_9);
DigitalOut Enable (PA_5);
DigitalOut Bipolar1(PC_7);
DigitalOut Bipolar2(PB_10);
DigitalOut Direction1(PB_5);
DigitalOut Direction2(PA_10);



//float value;

int main()
{
    while(1) {
			Enable =1;
			Bipolar1 =0;
			Bipolar2 =0;
			Direction1 = 0;
			Direction2 = 1;
			Motor1.period_us(200);
			Motor1.write(0.5); 
			Motor2.period_us(200);
			Motor2.write(0.5); 
			  
    }
}

	