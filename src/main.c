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


#include "Music.h"
#include "Notes.h"

/* Uncomment to use simulator - will compile alternative main function */
//#define USING_SIMULATOR

static void Init_buggy(void);
static void startSound(void);
static void printDebugInfo(void);
static void handleCMD(void);
static void printHelp(void);
static void printMemory(void);

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
	Init_buggy(); // Initialize the buggy
	startSound(); // Play confirmation sound for buggy ready
	
	Motors_setSpeed(motors, 0, 0); // Stop motors
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13); // Clear button interrupt flag triggered by startup=
	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable IRQ for EXTI line 13 in NVIC

	int counter = 0;
	
	/* Infinite loop */
	while(1){
		handleCMD(); // Read commands sent over serial
		
		delay_ms(200);  // Loop dealy
		if(counter%2 == 0){
			printDebugInfo(); // Every other time, print debug infromation to the serial
		}
		counter++;
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
	IO_set(IO_MOTOR_EN, 0);
	
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
	
	// Initialize line follower
	lf = LF_init(motors, ls);
	
	// Initialize memory handler
	memory = Memory_init();
	
	// Initialize main task arbiter
	arbiter = Arbiter_init(lf, ls, magnet, motors, memory);
	
	// Initialize Timers
	Timers_init();
}


// Sound played at Initialization time
void startSound(void){
		for(int freq =200; freq <=800; freq += 100){
		IO_setSpeakerFreq(freq);
		delay(0.1);
	
	}
	for(int freq =700; freq >=400; freq -= 100){
		IO_setSpeakerFreq(freq);
		delay(0.1);
	
	}
	IO_setSpeakerFreq(25000); // Turn the speaker off by playing high frequency

}

// Prints debug information to Wifi Serial
void printDebugInfo(void)
{
	// Init variables used for debugging
	static double voltage = 0;  // Battery voltage
	static double current = 0; // Battery current=
	static double accum = 0;   // Battery accumulated current
	static double temperature = 0;  // Battery temperature
	static uint32_t counter =0; // Loop counter
	
	
	float32_t conv[6]; // Sensorvalues array 
	//DS2781_resetAccumulatedCurrent(battery);
	uint16_t * rawADC = Analog_getValues(adc); // Raw ADC values (used for magnetic sensor)
	
	LS_getProcessedValues(ls, conv); // Get processed values from Line Sensors
	
	// Get battery voltage and current information from chip once every few cycles
	if(counter % 4 == 0){
		voltage = DS2781_readVoltage(battery);
		current = DS2781_readCurrent(battery);
			
		accum = DS2781_readAccumulatedCurrent(battery);
		temperature = DS2781_readTemperature(battery);
	}
	
	// Send data to Wifi using JSON
	USART_printf(esp, "JSON={\"counter\":%d,\"0\":%.4f,\"1\":%.4f,\"2\":%.4f,\"3\":%.4f,\"4\":%.4f,\"5\":%.4f,\"M\":%.4f,",
					counter++, conv[0], conv[1], conv[2], conv[3],conv[4],conv[5], rawADC[6]/4096.0);
	USART_printf(esp, "\"battV\":%.4f,\"battC\":%.4f,\"battA\":%.4f,\"battT\":%.4f,", voltage, current, accum*1000, temperature);
	USART_printf(esp, "\"mSL\":%.4f,\"mSR\":%.4f,\"mDL\":%.4f,\"mDR\":%.4f,\"mEL\":%.4f,\"mER\":%.4f,\"adc\":%d,\"sum\":%.4f,\"state\":%d,", Encoder_getSpeed(encoderLeft), Encoder_getSpeed(encoderRight), Encoder_getDistance(encoderLeft), Encoder_getDistance(encoderRight),motors->motorLeft->effort, motors->motorRight->effort,analogConversions,LS_getWeightedSum(ls),arbiter->state);
	USART_printf(esp, "\"other\":{\"Value1\":%d,\"Value2\":%d,\"Value X\":%d}}\r", 2,3,4);
}

// Handle CMD input
void handleCMD(){
	//  If there is new data to be read in the buffer
	if(esp->buffRX.send){
		
		// Command arguments
		char cmd[10];    // Command
		char target[10]; // Target of command
		double value0;   // Parameters 0-5
		double value1;
		double value2;
		double value3;
		double value4;
		double value5;
		
		esp->buffRX.send = 0; // Reset the flag
		sscanf(esp->buffRX.buffer, "%s %s %lf %lf %lf %lf %lf %lf", cmd, target, &value0, &value1, &value2, &value3, &value4, &value5); // Parse the message
		
		
		USART_printf(esp, "\n    \n");
		// Branch on command
		if(strcmp(cmd, "stop") == 0 || strcmp(cmd, "STOP") == 0 || strcmp(cmd, "f") == 0){
			IO_set(IO_MOTOR_EN, 0); // Emergency stop
			Motors_setSpeed(motors, 0, 0);
			arbiter->state = STATE_STOP;
		
		
		
		} else if(strcmp(cmd, "w") == 0){ // w s a d, remote controll
			IO_set(IO_MOTOR_EN, 1);
			Motors_setSpeed(motors, 2, 2);
		
		} else if(strcmp(cmd, "s") == 0){
			IO_set(IO_MOTOR_EN, 1);
			Motors_setSpeed(motors, -2, -2);
		
		} else if(strcmp(cmd, "a") == 0){
			IO_set(IO_MOTOR_EN, 1);
			Motors_setSpeed(motors, 0.8, 2);
		
		} else if(strcmp(cmd, "d") == 0){
			IO_set(IO_MOTOR_EN, 1);
			Motors_setSpeed(motors, 2, 0.8);
		
		
		
		} else if(strcmp(cmd, "reset") == 0){ // Resets the buggy for a new race.
			IO_set(IO_MOTOR_EN, 0);
			LF_reset(lf);
			LS_reset(ls);
			Arbiter_reset(arbiter);
			Memory_clear(memory);
		
		
		
		} else if(strcmp(cmd, "clear") == 0){ // Clears the screen
			USART_printf(esp, "\n\n\n\n\n\n\n\n\n\n\n\n");
		
		
		
		} else if(strcmp(cmd, "start") == 0){ // Starts the race
			Arbiter_startRace(arbiter);
		
		
		
		} else if(strcmp(target, "MRP") == 0){   // Motor Right P value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setP(motors->motorRight->pid, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right P: %f\n", PID_getP(motors->motorRight->pid));
		
		
		
		} else if(strcmp(target, "MRI") == 0){ // Motor Right I value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setI(motors->motorRight->pid, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right I: %f\n", PID_getI(motors->motorRight->pid));
		
		
		
		
		} else if(strcmp(target, "MRD") == 0){ // Motor Right D value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setD(motors->motorRight->pid, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right D: %f\n", PID_getD(motors->motorRight->pid));
		
		
		
		} else if(strcmp(target, "MLP") == 0){  // Motor Left P value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setP(motors->motorLeft->pid, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left P: %f\n", PID_getP(motors->motorLeft->pid));
		
		
		
		} else if(strcmp(target, "MLI") == 0){  // Motor Left I value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setI(motors->motorLeft->pid, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left I: %f\n", PID_getI(motors->motorLeft->pid));
		
		
		
		} else if(strcmp(target, "MLD") == 0){  // Motor Left D value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setD(motors->motorLeft->pid, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left D: %f\n", PID_getD(motors->motorLeft->pid));
		
		
		
		} else if(strcmp(target, "LP") == 0){  // Line sensor P value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setP(lf->ctrl, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Light sensor P: %f\n", PID_getD(lf->ctrl));
		
		
		
		} else if(strcmp(target, "LI") == 0){  // Line sensor I value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setI(lf->ctrl, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Light sensor I: %f\n", PID_getI(lf->ctrl));
		
		
		
		} else if(strcmp(target, "LD") == 0){  // Line sensor D value
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				PID_setD(lf->ctrl, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Light sensor D: %f\n", PID_getD(lf->ctrl));
		
		
		
		} else if(strcmp(target, "MRS") == 0){  // Motor Right speed
			if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				IO_set(IO_MOTOR_EN, 1);
				Motor_setSpeed(motors->motorRight, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor right speed: %frev/s\n",  motors->motorRight->speed);
		
		
		
		} else if(strcmp(target, "MLS") == 0){  // Motor left speed
				if(strcmp(cmd, "get") == 0){
				
			} else if(strcmp(cmd, "set") == 0){
				IO_set(IO_MOTOR_EN, 1);
				Motor_setSpeed(motors->motorLeft, value0);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left speed: %frev/s\n",  motors->motorLeft->speed);
			
		
		
		} else if(strcmp(target, "MS") == 0){				// Both motrs speed
			if(strcmp(cmd, "get") == 0){
			
			} else if(strcmp(cmd, "set") == 0){
				IO_set(IO_MOTOR_EN, 1);
				Motors_setSpeed(motors, value0, value1);
			} else {
				printHelp();
			}
			USART_printf(esp, "Motor left speed: %frev/s\n",  motors->motorLeft->speed);
			USART_printf(esp, "Motor right speed: %frev/s\n",  motors->motorRight->speed);
		
		
		
		} else if(strcmp(target, "LS") == 0){  // Line following speed
			if(strcmp(cmd, "get") == 0){
			
			} else if(strcmp(cmd, "set") == 0){
				IO_set(IO_MOTOR_EN, 1);
				lf->speed = value0;
			} else {
				printHelp();
			}
			USART_printf(esp, "Line Followig speed: %frev/s\n",  lf->speed);
		
		
		
		
		} else if(strcmp(target, "MEM") == 0){  // Get memory content
			if(strcmp(cmd, "get") == 0){
				printMemory();
			}
			
			
			 
		} else if(strcmp(target, "all") == 0) {   // Get all
			if(strcmp(cmd, "get") == 0){
				USART_printf(esp, "Motor right P: %f\n", PID_getP(motors->motorRight->pid));
				USART_printf(esp, "Motor right I: %f\n", PID_getI(motors->motorRight->pid));
				USART_printf(esp, "Motor right D: %f\n\n", PID_getD(motors->motorRight->pid));
				
				USART_printf(esp, "Motor left P: %f\n", PID_getP(motors->motorLeft->pid));
				USART_printf(esp, "Motor left I: %f\n", PID_getI(motors->motorLeft->pid));
				USART_printf(esp, "Motor left D: %f\n\n", PID_getD(motors->motorLeft->pid));
				
				USART_printf(esp, "Light sensor P: %f\n", PID_getP(lf->ctrl));
				USART_printf(esp, "Light sensor I: %f\n", PID_getI(lf->ctrl));
				USART_printf(esp, "Light sensor D: %f\n\n", PID_getD(lf->ctrl));
				USART_printf(esp, "Motor left speed: %frev/s\n", motors->motorLeft->speed);
				USART_printf(esp, "Motor right speed: %frev/s\n", motors->motorRight->speed);
				
				USART_printf(esp, "Line Followig speed: %frev/s\n",  lf->speed);
				
				printMemory();
			} else {
				printHelp();
			}
		} else{
			printHelp();
		}
		
		//USART_printf(esp, "Executing %s on %s with value0 %f\n\n", cmd, target, value0);
		
	}

}


// Prints memory content
void printMemory(void){
	Action * act = memory->action;
	char actionName[15];
	int counter = memory->length;
	// Decide on action name
	if(counter == 0){
		USART_printf(esp, "Memory Empty\n"); // If the first action was a NULL, memory was empty
		return;
	}
	USART_printf(esp, "Memory Content:\n");
	while(act != NULL){
		switch(act->actionType){
			case ACTION_START:
				strcpy(actionName,  "Start");
				break;
			case ACTION_STRAIGHT:
				strcpy(actionName,  "Straight");
				break;
			case ACTION_MINOR_L:
				strcpy(actionName,  "Minor Left");
				break;
			case ACTION_MINOR_R:
				strcpy(actionName,  "Minor Right");
				break;
			case ACTION_MAJOR_L:
				strcpy(actionName,  "Major Left");
				break;
			case ACTION_MAJOR_R:
				strcpy(actionName,  "Major Right");
				break;
			case ACTION_RAMP_P_S:
				strcpy(actionName,  "RampPosStart");
				break;
			case ACTION_RAMP_P_E:
				strcpy(actionName,  "RampPosEnd");
				break;
			case ACTION_RAMP_N_S:
				strcpy(actionName,  "RampNegStart");
				break;
			case ACTION_RAMP_N_E:
				strcpy(actionName,  "RampNegEnd");
				break;
			case ACTION_LOST:
				strcpy(actionName,  "Lost");
				break;
			case ACTION_OTHER:
				strcpy(actionName,  "Other");
				break;
		}
		USART_printf(esp, "%d: %s %.4fm\n", --counter, actionName, act->distance);
		
		// Get next action
		act = act->previous;
	}
	
	
}

// Prints command line help
void printHelp(){
	USART_printf(esp, "HELP:\n  set|get [target] [value0]\n\n");
	USART_printf(esp, "Targets:\nML[P|I|D|S] - Motor left [P|I|D|speed]\nMR[P|I|D|S] - Motor right [P|I|D|speed]\nL[P|I|D|S] - Light sensors [P|I|D|speed]\nMEM - print memory\n ");
}

// Button interupt as backup to wifi start
void EXTI15_10_IRQHandler(void){
	
	if(LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
		if(arbiter->state == STATE_READY){
			Arbiter_startRace(arbiter); 
		}
  }
}

// Analog read complet interrupt
void Analog_TransferComplete(){
	analogConversions++; // Count number of conversions
	LS_update(lf->ls); // Update the line sensor
	if(ls->newData){ // If there is new data, update the arbiter
		Arbiter_update(arbiter);
	}
}

// Analog errors
void Analog_TransferError(){
	adcErrors++;
}
