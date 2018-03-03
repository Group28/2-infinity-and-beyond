#include "main.h"

#include "System_Functions.h"

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
	LCD lcd;
	USART usb, esp;
	
	lcd = LCD_init();
	LCD_setFont(lcd, (char *)SmallFont);

	usb = USART_USB_init();
	esp = USART_ESP_init();
	
	// Initialize DMA controller
	DMA_init(DMA_getBuffers(esp, usb, lcd));
	
	
  /* Infinite loop */
	int counter = 0;
  while (1)
  {
		LCD_locate(lcd, 0, 0);
		LCD_cls(lcd);
		LCD_printf(lcd, "Hello %d", counter);
		
		USART_printf(usb, "Hello %d\n", counter);
		USART_printf(esp, "Hello %d\n", counter++);
		delay(0.5);
		
	}
}
