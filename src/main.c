#include "main.h"

#include "System_Functions.h"

#include "Analog.h"
#include "Arbiter.h"
#include "LF.h"
#include "DMA.h"
#include "IO.h"
#include "SR.h"
#include "LS.h"
#include "Magnet.h"
#include "Memory.h"
#include "PID.h"
#include "Motors.h"
#include "Encoders.h"
#include "USART.h"
#include "Timers.h"
#include "ds2781.h"
#include "utils.h"
#include "configuration.h"

#include <stdio.h>


/* Uncomment to use simulator - will compile alternative main function */
//#define USING_SIMULATOR

static void Init_buggy(void);
static void startSound(void);
static void printDebugInfo(void);
static void handleCMD(void);
static void printHelp(void);

DMA_Buffers dma_buffers;
Motors motors;
Encoder encoderLeft, encoderRight;
USART esp;
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
		
		handleCMD();
		
		delay(0.3);
	
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
	//lcd = LCD_init();
	//LCD_setFont(lcd, (char *)SmallFont);
	//LCD_cls(lcd);

	//usb = USART_USB_init();
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
	DMA_init(DMA_getBuffers(esp, adc)); // usb, lcd, adc));
	
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


void printDebugInfo(void)
{

	double voltage = 0;
	double current = 0;
	double accum = 0;
	double temperature = 0;
	static uint8_t counter =0;
	
	
	float32_t conv[6];
	//DS2781_resetAccumulatedCurrent(battery);
	uint16_t * rawADC = Analog_getValues(adc);
	
	LS_getProcessedValues(ls, conv);
	
	
	voltage = DS2781_readVoltage(battery);
	current = DS2781_readCurrent(battery);
			
	accum = DS2781_readAccumulatedCurrent(battery);
	temperature = DS2781_readTemperature(battery);
			
	USART_printf(esp, "JSON={\"counter\":%d,\"0\":%.4f,\"1\":%.4f,\"2\":%.4f,\"3\":%.4f,\"4\":%.4f,\"5\":%.4f,\"M\":%.4f,",
					counter++, conv[0], conv[1], conv[2], conv[3],conv[4],conv[5], rawADC[6]/4096.0);
	USART_printf(esp, "\"battV\":%.4f,\"battC\":%.4f,\"battA\":%.4f,\"battT\":%.4f,", voltage, current, accum*1000, temperature);
	USART_printf(esp, "\"mSL\":%.4f,\"mSR\":%.4f,\"mDL\":%.4f,\"mDR\":%.4f,\"mEL\":%.4f,\"mER\":%.4f,\"adc\":%d,\"sum\":%.4f,\"state\":%d}\r", Encoder_getSpeed(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getSpeed(encoderRight)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderLeft)*2*3.141592*WHEEL_RADIUS, Encoder_getRevolutions(encoderRight)*2*3.141592*WHEEL_RADIUS,motors->motorLeft->effort, motors->motorRight->effort,analogConversions,LS_getWeightedSum(ls),arbiter->state);


	//USART_printf(esp, "Raw ADC \"0\":%d, \"1\":%d, \"2\":%d, \"3\":%d\"4\":%d, \"5\":%d, \"M\":%d\n", rawADC[0], rawADC[1], rawADC[2], rawADC[3],rawADC[4],rawADC[5],rawADC[6]);
	//USART_printf(esp, "Raw LS_A \"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f \"4\":%f, \"5\":%f\n", ls->processedReadingsA[0], ls->processedReadingsA[1], ls->processedReadingsA[2], ls->processedReadingsA[3],ls->processedReadingsA[4],ls->processedReadingsA[5]);
	//USART_printf(esp, "Raw LS_B \"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f \"4\":%f, \"5\":%f\n", ls->processedReadingsB[0], ls->processedReadingsB[1], ls->processedReadingsB[2], ls->processedReadingsB[3],ls->processedReadingsB[4],ls->processedReadingsB[5]);
	//USART_printf(esp, "Raw LS_C \"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f \"4\":%f, \"5\":%f\n", ls->processedReadings[0], ls->processedReadings[1], ls->processedReadings[2], ls->processedReadings[3],ls->processedReadings[4],ls->processedReadings[5]);

	//USART_printf(esp, "USART Buffer: \"%s\"\n", esp->buffRX.buffer);

}



void handleCMD(){
	if(esp->buffRX.send){
		esp->buffRX.send = 0;
		char cmd[10];
		char target[10];
		double value;
		
		sscanf(esp->buffRX.buffer, "%s %s %lf", cmd, target, &value);
		
		
		USART_printf(esp, "\n\n\n\n");
		if(strcmp(cmd, "stop") == 0){
		
			IO_set(IO_MOTOR_EN, 0);
		
		
		} else if(strcmp(target, "MRP") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setP(motors->motorRight->pid, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right P: %f\n", PID_getP(motors->motorRight->pid));
		} else if(strcmp(target, "MRI") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setI(motors->motorRight->pid, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right I: %f\n", PID_getI(motors->motorRight->pid));
		} else if(strcmp(target, "MRD") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setD(motors->motorRight->pid, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right D: %f\n", PID_getD(motors->motorRight->pid));
		} else if(strcmp(target, "MLP") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setP(motors->motorLeft->pid, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left P: %f\n", PID_getP(motors->motorLeft->pid));
		} else if(strcmp(target, "MLI") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setI(motors->motorLeft->pid, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left I: %f\n", PID_getI(motors->motorLeft->pid));
		} else if(strcmp(target, "MLD") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setD(motors->motorLeft->pid, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left D: %f\n", PID_getD(motors->motorLeft->pid));
		} else if(strcmp(target, "LP") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setP(lf->ctrl, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Light sensor P: %f\n", PID_getD(lf->ctrl));
		} else if(strcmp(target, "LI") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setI(lf->ctrl, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Light sensor I: %f\n", PID_getI(lf->ctrl));
		} else if(strcmp(target, "LD") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setD(lf->ctrl, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Light sensor D: %f\n", PID_getD(lf->ctrl));
		} else if(strcmp(target, "MRS") == 0){
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				IO_set(IO_MOTOR_EN, 1);
				Motor_setSpeed(motors->motorRight, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right speed: %frev/s\n", value);
		} else if(strcmp(target, "MLS") == 0){
				if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				IO_set(IO_MOTOR_EN, 1);
				Motor_setSpeed(motors->motorLeft, value);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left speed: %frev/s\n", value);
		} else if(strcmp(target, "all") == 0){
			if(strcmp(cmd, "get") == 0){
				USART_printf(esp, "Motor right P: %f\n", PID_getP(motors->motorRight->pid));
				USART_printf(esp, "Motor right I: %f\n", PID_getI(motors->motorRight->pid));
				USART_printf(esp, "Motor right D: %f\n\n", PID_getD(motors->motorRight->pid));
				
				USART_printf(esp, "Motor left P: %f\n", PID_getP(motors->motorLeft->pid));
				USART_printf(esp, "Motor left I: %f\n", PID_getI(motors->motorLeft->pid));
				USART_printf(esp, "Motor left D: %f\n\n", PID_getD(motors->motorLeft->pid));
				
				USART_printf(esp, "Light sensor P: %f\n", PID_getP(lf->ctrl));
				USART_printf(esp, "Light sensor I: %f\n", PID_getI(lf->ctrl));
				USART_printf(esp, "Light sensor D: %f\n", PID_getD(lf->ctrl));
			} else {
				printHelp();
			}
		} else{
			printHelp();
		}
		
		//USART_printf(esp, "Executing %s on %s with value %f\n\n", cmd, target, value);
		USART_printf(esp, "\r");
	}

}



void printHelp(){
	USART_printf(esp, "HELP:\n  set|get [target] [value]\n\n");
	USART_printf(esp, "Targets:\nML[P|I|D] - Motor left [P|I|D]\nMR[P|I|D] - Motor right [P|I|D]\nL[P|I|D] - Light sensors [P|I|D]\n");
}


void EXTI15_10_IRQHandler(void){
	
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
		if(arbiter->state == STATE_READY){
			arbiter->state = STATE_FORWARD_TRACK;
		} else if(arbiter->state == STATE_STOP){
			arbiter->state = STATE_READY;
		} 
  }
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
