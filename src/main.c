#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


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
#include "smallfont.h"



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
	LCD * lcd;
	//USART * usb, esp;
	
	//encoderLeft = Encoder_init(TIM5, 1/MOTOR_SAMPLE_FREQ);
	//encoderRight = Encoder_init(TIM2, 1/MOTOR_SAMPLE_FREQ);
	//motors = Motors_init(motorLeft, motorRight, encoderLeftPID, encoderRightPID);
	lcd = LCD_init();
	//usb = USART_USB_init();
	//esp = USART_ESP_init();
	
	LCD_setFont(lcd, (unsigned char *)SmallFont);
	
  /* Infinite loop */
	int counter =0;
  uint32_t lastus = System_micros();
	uint32_t cycles = 0;
	char outputString[18];
  while (1)
  {
		
		
		
		if(lastus+1000000 > lastus){
			if(lastus+1000000 < System_micros()){
				sprintf(outputString, "Freq: %04d", cycles);
				LCD_puts_buffer(lcd, 0,10,  outputString);
				lastus = System_micros();
				cycles = 0;
			}
		} else {
			if(System_micros() < lastus && lastus+1000000 < System_micros() ){
				sprintf(outputString, "Freq: %04d", cycles);
				LCD_puts_buffer(lcd, 0,10,  outputString);
				lastus = System_micros();
				cycles = 0;
			}
		}
		
		sprintf(outputString, "Count: %04d", counter);
		LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_7);
	  LCD_puts_buffer(lcd,0,0,  outputString);
		LL_GPIO_ResetOutputPin(GPIOC,LL_GPIO_PIN_7);
	
		
		LCD_flushBuffer(lcd);
		//LL_mDelay(100);	
		counter++;
		cycles++;
		
		
		
	}
	//free(encoderLeft);
	//free(encoderRight);
	//free(motors);
	//free(lcd);
	//free(usb);
	//free(esp);
	//return 0;
}



