#include "Motors_And_Encoders.h"
#include "math.h"


void Set_DutyCycle_Motor_Left(float D){
	float P;    //Pulse duration                                           
  float T;    //PWM signal period  
	D = fabs(D);
	
	/* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM1) + 1;
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = ((100.0f-D)*T)/100.0f;

  LL_TIM_OC_SetCompareCH4(TIM1, (uint32_t)P);	
}

void Set_DutyCycle_Motor_Right(float D){
	float P;    //Pulse duration                                           
  float T;    //PWM signal period  
	D = fabs(D);
	
	/* PWM signal period is determined by the value of the auto-reload register */
  T = LL_TIM_GetAutoReload(TIM4) + 1;
  /* Pulse duration is determined by the value of the compare register.       */
  /* Its value is calculated in order to match the requested duty cycle.      */
  P = ((100.0f-D)*T)/100.0f;

  LL_TIM_OC_SetCompareCH2(TIM4, (uint32_t)P);	
}

void Configure_Motor_Right(void){
	//Setup GPIO PB7 for PWM Output (TMR4 CH2).
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Enable CLK to GPIOB
  LL_GPIO_SetPinMode(GPIOB,   LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOB,   LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOB,  LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_2);
	
	// Timer 4 peripheral Configuration
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4); // Enable clock to Timer 4 peripheral
	//Set the pre-scaler value to have TIM2 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM4, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	LL_TIM_EnableARRPreload(TIM4);
	/* Set the auto-reload value to have a counter frequency of 1000 Hz */
	LL_TIM_SetAutoReload(TIM4, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), 1000));
	//Set Timer Output mode
	LL_TIM_OC_SetMode(TIM4, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
	/* Set compare value to half of the counter period (50% duty cycle ) */
  LL_TIM_OC_SetCompareCH2(TIM4, ( (LL_TIM_GetAutoReload(TIM4) + 1 ) / 2));
	/* Enable TIM4_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM4_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH2);
	/* Enable the capture/compare interrupt for channel 2*/
  //LL_TIM_EnableIT_CC1(TIM4);
	/* Enable output channel 2 */
  LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2);
  /* Enable counter */
  LL_TIM_EnableCounter(TIM4);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM4);
}

void Configure_Motor_Left(void){
		//Setup GPIO PA11 for PWM Output (TMR1 CH4).
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOA
  LL_GPIO_SetPinMode(GPIOA,   LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA,   LL_GPIO_PIN_11, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOA,  LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOA,LL_GPIO_PIN_11, LL_GPIO_AF_1);
	
	// Timer 1 peripheral Configuration
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1); // Enable clock to Timer 1 peripheral
	//Set the pre-scaler value to have TIM1 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM1, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	LL_TIM_EnableARRPreload(TIM1);
	/* Set the auto-reload value to have a counter frequency of 1000 Hz */
	LL_TIM_SetAutoReload(TIM1, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM1), 1000));
	//Set Timer Output mode
	LL_TIM_OC_SetMode(TIM1, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
	/* Set compare value to half of the counter period (50% duty cycle ) */
  LL_TIM_OC_SetCompareCH4(TIM1, ( (LL_TIM_GetAutoReload(TIM1) + 1 ) / 2));
	/* Enable TIM1_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM1_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH4);
	/* Enable the capture/compare interrupt for channel 4*/
  //LL_TIM_EnableIT_CC1(TIM1);
	/* Enable output channel 4 */
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableAllOutputs(TIM1);
  /* Enable counter */
  LL_TIM_EnableCounter(TIM1);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM1);
}

int Get_Encoder_Left(void){
	return LL_TIM_GetCounter(TIM2);
}

int Get_Encoder_Right(void){
	return LL_TIM_GetCounter(TIM5);
}



void Configure_Encoder_Left(void){
	//Setup GPIO PA15 for Encoder Input (TMR2 CH1).
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOA
  LL_GPIO_SetPinMode(  GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(  GPIOA, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed( GPIOA, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_15, LL_GPIO_AF_1);
	
		//Setup GPIO PB3 for Encoder Input (TMR2 CH2).
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Enable CLK to GPIOB
  LL_GPIO_SetPinMode(  GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(  GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed( GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_1);
	
	NVIC_SetPriority(TIM2_IRQn, 0);
  NVIC_EnableIRQ(TIM2_IRQn);	
	
	// Timer 2 peripheral Configuration
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); // Enable clock to Timer 2 peripheral
	//Set the pre-scaler value to have TIM2 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	
	LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetFilter(     TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetFilter(     TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_SetEncoderMode(TIM2, LL_TIM_ENCODERMODE_X4_TI12);
	LL_TIM_EnableIT_CC1(TIM2);
	
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM2);
}


void Configure_Encoder_Right(void){
	//Setup GPIO PA0 for Encoder Input (TMR5 CH1). 
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOA
  LL_GPIO_SetPinMode(   GPIOA,  LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(   GPIOA,  LL_GPIO_PIN_0, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(  GPIOA,  LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7( GPIOA,  LL_GPIO_PIN_0, LL_GPIO_AF_2);
	
		//Setup GPIO PA_1 for Encoder Input (TMR5 CH2).
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOA
  LL_GPIO_SetPinMode(  GPIOA,   LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(  GPIOA,   LL_GPIO_PIN_1, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed( GPIOA,   LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOA,   LL_GPIO_PIN_1, LL_GPIO_AF_2);
	
	NVIC_SetPriority(TIM5_IRQn , 0);
  NVIC_EnableIRQ(TIM5_IRQn );	
	
	// Timer 5 peripheral Configuration
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5); // Enable clock to Timer 5 peripheral
	//Set the pre-scaler value to have TIM2 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM5, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	
	LL_TIM_IC_SetActiveInput(TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetFilter(     TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_IC_SetActiveInput(TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetFilter(     TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_SetEncoderMode(TIM5, LL_TIM_ENCODERMODE_X4_TI12);
	LL_TIM_EnableIT_CC1(TIM5);
	
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM5);
}

