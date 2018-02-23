#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "System_Functions.h"
#include "Motors_And_Encoders.h"
#include <math.h>
#include <arm_math.h>
#include <stdio.h>


/* 
		Pins used for this example
		Quadrature Encoder Left  on Timer 5 CH1 = PA0,  Timer 5 CH2 = PA1.
		Quadrature Encoder Right on Timer 2 CH1 = PA15, Timer 2 CH2 = PB3.
		Motor Right PWM                         = PB7,  Timer 4 CH2 
		Motor Left  PWM                         = PA11, Timer 1 CH4
		Nucleo User LED                         = PA5
		Nucleo User Button                      = PC13
*/

/* Function Prototypes */
void Reset_Encoders(void);

/* Global Variables */

/* Main Loop */

int main(void)
{

  /* Configure the system clock to 84 MHz */
  SystemClock_Config();
	
	//Right Motor and Encoder Initialisation
	Configure_Encoder_Right();                 //Configure Encoder input on PA15,PA1 (TIMER 2 CH1, CH2)
	Configure_Motor_Right();                   //Setup PB7 (TIMER 4 CH2) as PWM Output for Right Motor
	Set_DutyCycle_Motor_Right(0);
	
	//Left Motor and Encoder Initialisation
	Configure_Encoder_Left();                  //Configure Encoder input on PB4, PB5 (TIMER 3 CH1, CH2)
	Configure_Motor_Left();                    //Setup PA11(TIMER 1 CH4) as PWM Output for Left Motor
	Set_DutyCycle_Motor_Left(0);
	
	//Nucleo Board Functions
	Configure_User_LED();
	Configure_User_Button();
	
	Set_DutyCycle_Motor_Left(50);
	Set_DutyCycle_Motor_Right(50);	
	
  /* Infinite loop */
  while (1)
  {
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reset_Encoders(void){
	NVIC_DisableIRQ(TIM2_IRQn);	
	NVIC_DisableIRQ(TIM5_IRQn);

	LL_TIM_SetCounter(TIM2, 0);
	LL_TIM_SetCounter(TIM5, 0);
	
	NVIC_EnableIRQ(TIM2_IRQn);	
	NVIC_EnableIRQ(TIM5_IRQn);	
}


//**************Handlers and Callbacks*********************//
void TIM5_IRQHandler(void)
{
  /* Check whether CC1 interrupt is pending */
  if(LL_TIM_IsActiveFlag_CC1(TIM5) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_CC1(TIM5);
  }
}

void TIM2_IRQHandler(void)
{
  /* Check whether CC1 interrupt is pending */
  if(LL_TIM_IsActiveFlag_CC1(TIM2) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_CC1(TIM2);
  }
}

void EXTI15_10_IRQHandler(void){
	static volatile bool onoff;
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
}

void SysTick_Callback(void){
	//use this as a 1 ms interrupt
}


