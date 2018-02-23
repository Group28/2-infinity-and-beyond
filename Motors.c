#include "Motors.h"

#include "main.h"

#include <stdbool.h>
#include <stdlib.h>
#include "math.h"


Motor * Motor_init(PID_Values values){
	Motor * motor = (Motor*) malloc(sizeof(Motor));
	motor->pid = PID_init(values);
	motor->speed = 0;
	Configure_Motor_Right();
	
	
	return motor;
}

Motors * Motors_init(PID_Values valuesL, PID_Values valuesR){
	Motors * motors = (Motors*) malloc(sizeof(Motors));
	motors->motorLeft = Motor_init(valuesL);
	motors->motorRight = Motor_init(valuesR);
	
	return motors;
}

void Motor_Left_PID_action(Motor * motor){
	double realSpeed =  Encoder_getSpeed(motor->encoder);
}

void Motor_Right_PID_action(Motor * motor){

}



void Motors_PID_action(Motors * motors){
	Motor_Left_PID_action(motors->motorLeft);
	Motor_Right_PID_action(motors->motorRight);
}

void Motor_Left_setSpeed(Motor * motor, double speed){
	motor->speed = speed;
}
void Motor_Right_setSpeed(Motor * motor, double speed){
	motor->speed = speed;
}

void Motors_setSpeed(Motors * motors, double speedL, double speedR){
	Motor_Left_setSpeed(motors->motorLeft, speedL);
	Motor_Right_setSpeed(motors->motorRight, speedR);
}


void Set_DutyCycle_Motor_Left(float D){
	float P;    //Pulse duration                                           
  float T;    //PWM signal period  
	D = fabs(D);
	
	/* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM3) + 1;
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = ((100.0f-D)*T)/100.0f;

  LL_TIM_OC_SetCompareCH3(TIM3, (uint32_t)P);	
}

void Set_DutyCycle_Motor_Right(float D){
	float P;    //Pulse duration                                           
  float T;    //PWM signal period  
	D = fabs(D);
	
	/* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM3) + 1;
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = ((100.0f-D)*T)/100.0f;

  LL_TIM_OC_SetCompareCH4(TIM3, (uint32_t)P);	
}

void Configure_Motor_Right(void){

	
	
	// GPIO Outputs
	
	// Motor 2 Bp
	LL_GPIO_SetPinMode(GPIOB,   LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinPull(GPIOB,   LL_GPIO_PIN_10, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOB,  LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10);
	
	// Motor 2 Dir
	LL_GPIO_SetPinMode(GPIOA,   LL_GPIO_PIN_10, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinPull(GPIOA,   LL_GPIO_PIN_10, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOA,  LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_10);
	
	
	
	
	// PWM

	//Setup GPIO PB7 for PWM Output (TMR4 CH2).
	
  LL_GPIO_SetPinMode(GPIOC,   LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOC,   LL_GPIO_PIN_9, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOC,  LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	//if(tiemr == TIM)
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_9, LL_GPIO_AF_2);
	
	// Timer 4 peripheral Configuration
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3); // Enable clock to Timer 4 peripheral
	//Set the pre-scaler value to have TIM2 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM3, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	LL_TIM_EnableARRPreload(TIM3);
	/* Set the auto-reload value to have a counter frequency of 1000 Hz */
	LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM3), 20000));
	//Set Timer Output mode
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
	/* Set compare value to half of the counter period (50% duty cycle ) */
  LL_TIM_OC_SetCompareCH2(TIM3, ( (LL_TIM_GetAutoReload(TIM3) + 1 ) / 2));
	/* Enable TIM4_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM4_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
	/* Enable the capture/compare interrupt for channel 2*/
  //LL_TIM_EnableIT_CC1(TIM4);
	/* Enable output channel 2 */
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
  /* Enable counter */
  LL_TIM_EnableCounter(TIM3);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM3);
}

void Configure_Motor_Left(void){

	
	
	// GPIO Outputs
	
	// Motor 2 Bp
	LL_GPIO_SetPinMode(GPIOC,   LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinPull(GPIOC,   LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOC,  LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);
	
	// Motor 2 Dir
	LL_GPIO_SetPinMode(GPIOB,   LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinPull(GPIOB,   LL_GPIO_PIN_5, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOB,  LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	
	
	// PWM
	
		//Setup GPIO PA11 for PWM Output (TMR1 CH4).
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); // Enable CLK to GPIOA
  LL_GPIO_SetPinMode(GPIOC,   LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOC,   LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOC,  LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOC,LL_GPIO_PIN_8, LL_GPIO_AF_2);
	
	// Timer 1 peripheral Configuration
	LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3); // Enable clock to Timer 1 peripheral
	//Set the pre-scaler value to have TIM1 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM3, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	LL_TIM_EnableARRPreload(TIM3);
	/* Set the auto-reload value to have a counter frequency of 1000 Hz */
	LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM3), 20000));
	//Set Timer Output mode
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
	/* Set compare value to half of the counter period (50% duty cycle ) */
  LL_TIM_OC_SetCompareCH4(TIM3, ( (LL_TIM_GetAutoReload(TIM3) + 1 ) / 2));
	/* Enable TIM1_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM1_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);
	/* Enable the capture/compare interrupt for channel 4*/
  //LL_TIM_EnableIT_CC1(TIM1);
	/* Enable output channel 4 */
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_EnableAllOutputs(TIM3);
  /* Enable counter */
  LL_TIM_EnableCounter(TIM3);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM3);
}

