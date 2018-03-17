#include "main.h"

#include "System_Functions.h"

#include "Analog.h"
#include "DMA.h"
#include "IO.h"
#include "SR.h"
#include "PID.h"
#include "Motors.h"
#include "Encoders.h"
#include "LCD.h"
#include "smallfont.h"
#include "USART.h"
#include "Timers.h"
#include "ds2781.h"
#include "utils.h"
#include "configuration.h"
#include "arm_math.h"

/* Uncomment to use simulator - will compile alternative main function */
//#define USING_SIMULATOR

static void Init_buggy(void);

DMA_Buffers dma_buffers;
Motors motors;
Encoder encoderLeft, encoderRight;
LCD lcd;
USART usb, esp;
Analog adc;
SR sr;
DS2781 battery;




#ifndef USING_SIMULATOR
/* Default main function */

int main(void)
{
	Init_buggy();
	uint16_t *adcValues;
	float conv[ADC_CHANNEL_COUNT];
	for(int freq =200; freq <=800; freq += 100){
		IO_setSpeakerFreq(freq);
		delay(0.1);
	
	}
	for(int freq =700; freq >=400; freq -= 100){
		IO_setSpeakerFreq(freq);
		delay(0.1);
	
	}
	IO_setSpeakerFreq(25000);
	Motors_setSpeed(motors, 0, 0);
  /* Infinite loop */
	
	double voltage = 0;
	double current = 0;
	double accum = 0;
	double temperature = 0;
	uint8_t counter =0;
	
	float speed = 0;
	uint8_t dir = 1;
	
	//DS2781_resetAccumulatedCurrent(battery);
	SR_set(sr, 0x26);
	SR_set(sr, 0xFF);
  while (1)
  {
		SR_rotateLeft(sr);
		SR_rotateLeft(sr);
		//SR_rotateLeft(sr);
		LCD_cls_buffer(lcd);
		
		voltage = DS2781_readVoltage(battery);
		current = DS2781_readCurrent(battery);
		
		accum = DS2781_readAccumulatedCurrent(battery);
		temperature = DS2781_readTemperature(battery);
		
		//LCD_locate(lcd, 0,0);
		
	
		
		IO_set(IO_MOTOR_EN, 1);
		Analog_startConversion(adc);
		delay(0.05);
		
		
		Motors_setSpeed(motors, speed, speed);
		for(int i = 0; i< 10; i++){
			adcValues = Analog_getValues(adc);
		
			for(int i = 0; i< ADC_CHANNEL_COUNT; i++){
				conv[i]=adcValues[i]/4096.0;
			}
			LCD_cls(lcd);
			LCD_locate(lcd, 0, 0);
			LCD_printf(lcd, "LS:%5.2fms, RS:%5.2fms\nLRev:%6.3f, RRev:%6.3f\n", Encoder_getSpeed(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getSpeed(encoderRight)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderLeft), Encoder_getRevolutions(encoderRight));
			LCD_printf(lcd, "EffL:%5.2f, EffR:%5.2f, Sp:%.2f", motors->motorLeft->effort, motors->motorRight->effort, speed);
			//LCD_printf(lcd, "0:%.2f 1:%.2f 2:%.2f 3:%.2f\n4:%.2f 5:%.2f A+%.2f A-%.2f\nB+%.2f B-%.2f M:%.2f\n", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5],conv[7],conv[8],conv[9],conv[10],conv[6]);
			USART_printf(esp, "JSON={\"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f,\"4\":%f, \"5\":%f, \"A+\":%f, \"A-\":%f,\"B+\":%f, \"B-\":%f, \"M\":%f}\r", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5],conv[7],conv[8],conv[9],conv[10],conv[6]);
			
			//USART_printf(esp, "A+:%f, A-:%f,\nB+:%f, B-:%f,\n DiffA: %f, DiffB: %f\n", conv[7],conv[8], conv[9], conv[10], conv[7]-conv[8], conv[9]-conv[10]);
			USART_printf(esp, "\n\n\n\n\n\n\n         Message %d\n",counter++);
			USART_printf(esp, "Battery\n---------\n  Voltage: %.3fV, Current: %.4fA \n  Accumulated current: %.2fmAh, Temperature: %.3f*C\n\n", voltage, current, accum*1000, temperature);
			
			USART_printf(esp, "Motors\n----------\n  Left               Right\n");

			USART_printf(esp, "  Speed:   %5.2fm/s  Speed:%5.2fm/s\n  Distance:%6.3fm   Distance:%6.3fm\n", Encoder_getSpeed(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getSpeed(encoderRight)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderRight)*2*3.141592*WHEEL_RADIUS);
			USART_printf(esp, "  Effort:  %5.2f     Effort:%5.2f        Set speed :%.2f\n", motors->motorLeft->effort, motors->motorRight->effort, speed);
			USART_printf(esp, "  Encoder: %d         Encoder: %d",TIM5->CNT, TIM2->CNT);
			Analog_startConversion(adc);
			delay(0.05);
		}
		//USART_printf(esp, "%u\n", __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), 50));
		//LCD_printf(lcd, "Frequency: %dHz", freq);
		
		
		if(speed > 3) {
			dir = 0;
		}
		if(speed < -3){
			dir = 1;
		}
		
		if(dir){
			speed += 0.5;
		}else {
			speed -= 0.5;
		}
	}
	
}
#else

/* Simulated main function -> use to test code */

int main(void){
	int y1=0, y2=0, x = 0, sum=0;
	while(1){
		y1 = arm_sin_f32(2*3.1415*x*100);
		y2 = arm_cos_f32(2*3.1415*x*100);
		x++;
		sum=y1+y2;
		if(x==100)
			x=0;
	}
}
#endif

/* Initialize all peripherials of the buggy */
void Init_buggy(){

	
	/* Configure the system clock to 84 MHz */
	SystemClock_Config();
	
	// Initialize IO
	IO_init();
	
	// Initialize PID controllers for the Motors
	PID_Values motorLeftPID = LEFT_MOTOR_PID_VALUES;
	PID_Values motorRightPID = RIGHT_MOTOR_PID_VALUES;
	
	// Initialize peripherals

	encoderLeft = Encoder_init(TIM5, 1/MOTOR_SAMPLE_FREQ, ENCODER_TICKS_PER_REV);
	encoderRight = Encoder_init(TIM2, 1/MOTOR_SAMPLE_FREQ, ENCODER_TICKS_PER_REV);
	motors = Motors_init(motorLeftPID, motorRightPID, encoderLeft, encoderRight);
	
	Motors_setSpeed(motors, 0, 0);
	
	// Initialize communication peripherial	
	lcd = LCD_init();
	LCD_setFont(lcd, (char *)SmallFont);
	LCD_cls(lcd);

	usb = USART_USB_init();
	esp = USART_ESP_init();
	
	adc = Analog_init();
	Analog_enable(adc);
	
	sr = SR_init(IR_SENSOR_COUNT);
	
	// Initialize battery voltage and current sensing
	battery = DS2781_init(IO_MICROLAN);
	
	// Initialize DMA controller
	DMA_init(DMA_getBuffers(esp, usb, lcd, adc));
	
	Timers_init();
}
