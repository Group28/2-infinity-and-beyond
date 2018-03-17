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

#include "Music.h"
#include "Notes.h"

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
	
	delay(1);
	
	Music hallOfMontainKing = Music_create(EGM2_notes, EGM2_delays, EGM2_length);
	Music_play(hallOfMontainKing, 3, 0);
	IO_setSpeakerFreq(25000);
	delay(1);
	//Music_play(mario1, 1);
	//IO_setSpeakerFreq(25000);
	delay(1);
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
	}
	
}
#else

/* Simulated main function -> use to test code */

int main(void){
	while(1){

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
	//LCD_setFont(lcd, (char *)SmallFont);
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
