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
#include "utils.h"

static void Init_buggy(void);

DMA_Buffers dma_buffers;
Motors motors;
Encoder encoderLeft, encoderRight;
LCD lcd;
USART usb, esp;
Analog adc;
SR sr;


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
  /* Infinite loop */
  while (1)
  {
			IO_set(IO_MOTOR_EN, 1);
		Analog_startConversion(adc);
		delay(0.05);
		
		adcValues = Analog_getValues(adc);
		
		for(int i = 0; i< ADC_CHANNEL_COUNT; i++){
			conv[i]=adcValues[i]/4096.0;
		}
		
		LCD_locate(lcd, 0, 0);
		LCD_cls(lcd);
		
		LCD_printf(lcd, "L Spe: %5.3f, R Spe: %5.3f\nL Rev: %5.3f, R Rev: %5.3f", Encoder_getSpeed(encoderLeft), Encoder_getSpeed(encoderRight), Encoder_getRevolutions(encoderLeft), Encoder_getRevolutions(encoderRight));
		//LCD_printf(lcd, "0:%.2f 1:%.2f 2:%.2f 3:%.2f\n4:%.2f 5:%.2f A+%.2f A-%.2f\nB+%.2f B-%.2f M:%.2f\n", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5],conv[7],conv[8],conv[9],conv[10],conv[6]);
		USART_printf(esp, "JSON={\"0\":%f, \"1\":%f, \"2\":%f, \"3\":%f,\"4\":%f, \"5\":%f, \"A+\":%f, \"A-\":%f,\"B+\":%f, \"B-\":%f, \"M\":%f}", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5],conv[7],conv[8],conv[9],conv[10],conv[6]);
		delay(0.05);
		USART_printf(esp, "A+:%f, A-:%f,\nB+:%f, B-:%f,\n DiffA: %f, DiffB: %f\n", conv[7],conv[8], conv[9], conv[10], conv[7]-conv[8], conv[9]-conv[10]);
		//USART_printf(esp, "%u\n", __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), 50));
		//LCD_printf(lcd, "Frequency: %dHz", freq);
		
		
		
	}
}


void Init_buggy(){

	
	/* Configure the system clock to 84 MHz */
	SystemClock_Config();
	
	// Initialize IO
	IO_init();
	
	// Initialize PID controllers for the Motors
	PID_Values motorLeftPID = {0.25, 0.005, 0.8};
	PID_Values motorRightPID = {0.2, 0.003, 0.8};
	
	// Initialize peripherals

	encoderLeft = Encoder_init(TIM5, 1/MOTOR_SAMPLE_FREQ, ENCODER_TICKS_PER_REV);
	encoderRight = Encoder_init(TIM2, 1/MOTOR_SAMPLE_FREQ, ENCODER_TICKS_PER_REV);
	motors = Motors_init(motorLeftPID, motorRightPID, encoderLeft, encoderRight);
	
	Motors_setSpeed(motors, 1, 1);
	
	// Initialize communication peripherial	
	lcd = LCD_init();
	LCD_setFont(lcd, (char *)SmallFont);
	LCD_cls(lcd);

	usb = USART_USB_init();
	esp = USART_ESP_init();
	
	adc = Analog_init();
	Analog_enable(adc);
	sr = SR_init(6);
	
	// Initialize DMA controller
	DMA_init(DMA_getBuffers(esp, usb, lcd, adc));
	
	Timers_init();
}

void TIM1_BRK_TIM9_IRQHandler(){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM9) == 1)

  {

    /* Clear the update interrupt flag*/

    LL_TIM_ClearFlag_UPDATE(TIM9);
		Encoder_update(encoderRight);
		Encoder_update(encoderLeft);
		Motors_PID_action(motors);

  }

}
