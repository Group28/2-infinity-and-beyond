#include "Encoders.h"
#include "math.h"


double Encoder_getSpeed(QEncoder * encoder){
	return 0;
}

int Get_Encoder_Right(void){
	return LL_TIM_GetCounter(TIM2);
}

int Get_Encoder_Left(void){
	return LL_TIM_GetCounter(TIM5);
}



void Configure_Encoder_Right(void){
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


void Configure_Encoder_Left(void){
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

