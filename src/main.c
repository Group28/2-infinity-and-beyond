#include "main.h"

#include "System_Functions.h"

#include "Analog.h"
#include "Arbiter.h"
#include "LF.h"
#include "DMA.h"
#include "IO.h"
#include "SR.h"
#include "LightSensors.h"
#include "Magnet.h"
#include "Memory.h"
#include "PID.h"
#include "Motors.h"
#include "Encoders.h"
#include "LCD.h"
//#include "smallfont.h"
#include "USART.h"
#include "Timers.h"
#include "ds2781.h"
#include "utils.h"
#include "configuration.h"


/* Uncomment to use simulator - will compile alternative main function */
//#define USING_SIMULATOR

static void Init_buggy(void);
static void startSound(void);
static void printDebugInfo(void);

DMA_Buffers dma_buffers;
Motors motors;
Encoder encoderLeft, encoderRight;
LCD lcd;
USART usb, esp;
Arbiter arbiter;
LF lf;

Analog adc;
SR sr;
DS2781 battery;
LS ls;
Magnet magnet;
Memory memory;


int analogConversions = 0;
int adcErrors = 0;

#ifndef USING_SIMULATOR
/* Default main function */

int main(void){
	Init_buggy();
	startSound();
	
	Motors_setSpeed(motors, 0, 0);
  
	
	/* Infinite loop */
	while(1){
		printDebugInfo();
		delay(0.5);
	
	}
}

#else

/* Simulated main function -> use to test code */

int main(void){
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
	//LCD_setFont(lcd, (char *)SmallFont);
	LCD_cls(lcd);

	usb = USART_USB_init();
	esp = USART_ESP_init();
	
	
	// Initialize sensors
	adc = Analog_init();
	Analog_enable(adc);
	
	
	// Light sensors
	sr = SR_init(IR_SENSOR_COUNT);
	ls = LS_init(adc, sr);
	SR_set(sr, 0x26);
	
  // Magnetic sensor
	magnet = Magnet_init(adc);
	
	// Initialize battery voltage and current sensing
	battery = DS2781_init(IO_MICROLAN);
	
	// Initialize DMA controller
	DMA_init(DMA_getBuffers(esp, usb, lcd, adc));
	
	lf = LF_init(motors, ls);
	
	memory = Memory_init();
	
	// Initialize main task arbiter
	arbiter = Arbiter_init(lf, ls, magnet, motors, memory);
	
	// Initialize Timers
	Timers_init();
}

void startSound(void){
		for(int freq =200; freq <=800; freq += 100){
		IO_setSpeakerFreq(freq);
		delay(0.1);
	
	}
	for(int freq =700; freq >=400; freq -= 100){
		IO_setSpeakerFreq(freq);
		delay(0.1);
	
	}
	IO_setSpeakerFreq(25000);

}

void Analog_TransferComplete(){
	analogConversions++;
	LS_update(lf->ls);
	if(ls->newData){
		Arbiter_update(arbiter);
	}
}

void Analog_TransferError(){
	adcErrors++;
}

void printDebugInfo(void)
{

	double voltage = 0;
	double current = 0;
	double accum = 0;
	double temperature = 0;
	static uint8_t counter =0;
	
	float speed = 1.5;
	float32_t conv[6];
	//DS2781_resetAccumulatedCurrent(battery);
	uint16_t * rawADC = Analog_getValues(adc);

	
	LS_getProcessedValues(ls, conv);
	
	
	voltage = DS2781_readVoltage(battery);
	current = DS2781_readCurrent(battery);
			
	accum = DS2781_readAccumulatedCurrent(battery);
	temperature = DS2781_readTemperature(battery);
			
			
			//LCD_cls_buffer(lcd);
			//LCD_locate(lcd, 0, 0);
			//LCD_printf(lcd, "LS:%5.2fms, RS:%5.2fms\nLRev:%6.3f, RRev:%6.3f\n", Encoder_getSpeed(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getSpeed(encoderRight)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderLeft), Encoder_getRevolutions(encoderRight));
			//LCD_printf(lcd, "EffL:%5.2f, EffR:%5.2f, Sp:%.2f", motors->motorLeft->effort, motors->motorRight->effort, speed);
			//LCD_printf(lcd, "0:%.2f 1:%.2f 2:%.2f 3:%.2f\n4:%.2f 5:%.2f A+%.2f A-%.2f\nB+%.2f B-%.2f M:%.2f\n", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5],conv[7],conv[8],conv[9],conv[10],conv[6]);
	USART_printf(esp, "JSON={\"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f,\"4\":%f, \"5\":%f, \"M\":%f}\r", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5], rawADC[6]/4096.0);
			
			
			
	//USART_printf(esp, "A+:%f, A-:%f,\nB+:%f, B-:%f,\n DiffA: %f, DiffB: %f\n", conv[7],conv[8], conv[9], conv[10], conv[7]-conv[8], conv[9]-conv[10]);
	USART_printf(esp, "\n\n\n\n\n\n\n         Message %d\n",counter++);
			
	USART_printf(esp, "Battery\n---------\n  Voltage: %.3fV, Current: %.4fA \n  Accumulated current: %.2fmAh, Temperature: %.3f*C\n\n", voltage, current, accum*1000, temperature);
			
	USART_printf(esp, "Motors\n----------\n  Left               Right\n");

	USART_printf(esp, "  Speed:   %5.2fm/s  Speed:%5.2fm/s\n  Distance:%6.3fm   Distance:%6.3fm\n", Encoder_getSpeed(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getSpeed(encoderRight)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderRight)*2*3.141592*WHEEL_RADIUS);
	USART_printf(esp, "  Effort:  %5.2f     Effort:%5.2f        Set speed :%.2f\n", motors->motorLeft->effort, motors->motorRight->effort, speed);
	USART_printf(esp, "  Encoder: %d         Encoder: %d\n",TIM5->CNT, TIM2->CNT);
	USART_printf(esp, "  ADC Conversions: %d, ADC Errors: %d\n", analogConversions, adcErrors);
	//USART_printf(esp, "Raw ADC \"0\":%d, \"1\":%d, \"2\":%d, \"3\":%d\"4\":%d, \"5\":%d, \"M\":%d\n", rawADC[0], rawADC[1], rawADC[2], rawADC[3],rawADC[4],rawADC[5],rawADC[6]);
	//USART_printf(esp, "Raw LS_A \"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f \"4\":%f, \"5\":%f\n", ls->processedReadingsA[0], ls->processedReadingsA[1], ls->processedReadingsA[2], ls->processedReadingsA[3],ls->processedReadingsA[4],ls->processedReadingsA[5]);
	//USART_printf(esp, "Raw LS_B \"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f \"4\":%f, \"5\":%f\n", ls->processedReadingsB[0], ls->processedReadingsB[1], ls->processedReadingsB[2], ls->processedReadingsB[3],ls->processedReadingsB[4],ls->processedReadingsB[5]);
	//USART_printf(esp, "Raw LS_C \"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f \"4\":%f, \"5\":%f\n", ls->processedReadings[0], ls->processedReadings[1], ls->processedReadings[2], ls->processedReadings[3],ls->processedReadings[4],ls->processedReadings[5]);
	USART_printf(esp, "Sum: %f\n", LS_getWeightedSum(ls));
	switch(arbiter->state){
		case STATE_READY:
			USART_printf(esp, "State: Ready\n");
			break;
		case STATE_FORWARD_TRACK:
			USART_printf(esp, "State: Forwardtrack\n");
			break;
		case STATE_BACK_TRACK:
			USART_printf(esp, "State: Backtrack\n");
			break;
		case STATE_TURN:
			USART_printf(esp, "State: Turn\n");
			break;
		case STATE_LOST_F:
			USART_printf(esp, "State: Lost forward\n");
			break;
		case STATE_LOST_B:
			USART_printf(esp, "State: Lost back\n");
			break;
		case STATE_STOP:
		default:
			USART_printf(esp, "State: Stop\n");
			break;
	}
}


void EXTI15_10_IRQHandler(void){
	
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
		//for(int freq =200; freq <=800; freq += 100){
			//	IO_setSpeakerFreq(freq);
			//	delay(0.1);
		//	}
		if(arbiter->state == STATE_READY){
			arbiter->state = STATE_FORWARD_TRACK;
		} else if(arbiter->state == STATE_STOP){
			arbiter->state = STATE_READY;
		} 
  }
}
