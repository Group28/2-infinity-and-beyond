#include "main.h"

#include "System_Functions.h"

#include "ADC.h"
#include "DMA.h"
#include "IO.h"
#include "PID.h"
#include "Motors.h"
#include "Encoders.h"
#include "LCD.h"
#include "smallfont.h"
#include "USART.h"
#include "utils.h"


DMA_Buffers dma_buffers;

int main(void)
{
	LCD lcd;
	USART usb, esp;
	ADC adc;
	
	/* Configure the system clock to 84 MHz */
	SystemClock_Config();
	
	// Initialize IO
	IO_init();
	
	// Initialize PID controllers for the Motors
	//PID_Values motorLeftPID = {10, 1, 1};
	//PID_Values motorRightPID = {10, 1, 1};
	
	// Initialize peripherals
	//Motors * motors;
	//Encoder * encoderLeft, encoderRight;
	//encoderLeft = Encoder_init(TIM5, 1/MOTOR_SAMPLE_FREQ);
	//encoderRight = Encoder_init(TIM2, 1/MOTOR_SAMPLE_FREQ);
	//motors = Motors_init(motorLeft, motorRight, encoderLeftPID, encoderRightPID);
	
	
	// Initialize communication peripherials
	lcd = LCD_init();
	LCD_setFont(lcd, (char *)SmallFont);

	usb = USART_USB_init();
	esp = USART_ESP_init();
	
	adc = ADC_init();
	ADC_activate(adc);
	
	// Initialize DMA controller
	DMA_init(DMA_getBuffers(esp, usb, lcd, adc));
	
	uint16_t *adcValues;
	
  /* Infinite loop */
  while (1)
  {
		ADC_startConversion(adc);
		delay(0.1);
		
		adcValues = ADC_getValues(adc);
		
		LCD_locate(lcd, 0, 0);
		LCD_cls(lcd);
		LCD_printf(lcd, "0: %u, 1: %u, 2: %u, 3: %u", adcValues[0], adcValues[1], adcValues[2], adcValues[3]);
		

		delay(0.5);
		
	}
}
