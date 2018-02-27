#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#include <math.h>
#include <arm_math.h>
#include <stdio.h>

#include "IO.h"
#include "PID.h"
#include "System_Functions.h"
#include "Motors.h"
#include "Encoders.h"
#include "LCD.h"
#include "USART.h"



int main(void)
{
	/* Configure the system clock to 84 MHz */
	SystemClock_Config();
	
	// Initialize IO
	IO_init();
	
	// Initialize PID controllers for the Motors
	PID_Values motorLeftPID = {10, 1, 1};
	PID_Values motorRightPID = {10, 1, 1};
	
	// Initialize peripherals
	Motors * motors;
	Encoder * encoderLeft, encoderRight;
	LCD * lcd;
	USART * usb, esp;
	
	encoderLeft = Encoder_init(TIM5, 1/MOTOR_SAMPLE_FREQ);
	encoderRight = Encoder_init(TIM2, 1/MOTOR_SAMPLE_FREQ);
	motors = Motors_init(motorLeft, motorRight, encoderLeftPID, encoderRightPID);
	lcd = LCD_init();
	usb = USART_USB_init();
	esp = USART_ESP_init();
	
	
	
  /* Infinite loop */
  while (1)
  {
	}
	free(encoderLeft);
	free(encoderRight);
	free(motors);
	free(lcd);
	free(usb);
	free(esp);
	return 0;
}
