#include "main.h"

#include "System_Functions.h"

#include "Analog.h"
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
	Analog adc;
	
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
	
	adc = Analog_init();
	Analog_enable(adc);
	
	// Initialize DMA controller
	DMA_init(DMA_getBuffers(esp, usb, lcd, adc));
	
	uint16_t *adcValues;
	float conv[ADC_CHANNEL_COUNT];
	
  /* Infinite loop */
  while (1)
  {
		Analog_startConversion(adc);
		delay(0.1);
		
		adcValues = Analog_getValues(adc);
		
		for(int i = 0; i< ADC_CHANNEL_COUNT; i++){
			conv[i]=adcValues[i]/4096.0;
		}
		
		LCD_locate(lcd, 0, 0);
		LCD_cls(lcd);
		LCD_printf(lcd, "0:%.2f 1:%.2f 2:%.2f 3:%.2f\n4:%.2f 5:%.2f A+%.2f A-%.2f\nB+%.2f B-%.2f M:%.2f\n", conv[0], conv[1], conv[2], conv[3],conv[4],conv[5],conv[7],conv[8],conv[9],conv[10],conv[6]);
		

		delay(0.5);
		
	}
}
