#include "IO.h"
#include "configuration.h"

void IO_init(void){
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOA
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Enable CLK to GPIOB
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); // Enable CLK to GPIOC
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD); // Enable CLK to GPIOD
	
	// Light sensor, Magnetic sensor and Current Sensing
	IO_initAnalogueSensors();
	
	// Shift register
	IO_Output_init(IO_SR_CLK);
	IO_Output_init(IO_SR_DATA);
	IO_Output_init(IO_SR_LATCH);

	
	// Battery voltage sensing
	
	// Encoders
	IO_initEncoders();
	
	// Motor contorlls
	
	
	IO_initMotors();
	
	// ESP Serial
	
	// Usart2 serial
	
	// I2C
	
	// LCD
	
	
	// Other IO
	
	

}

void IO_Output_init(GPIO_TypeDef * port, uint32_t pin){
	LL_GPIO_InitTypeDef gpio_struct;
	
	gpio_struct.Pin = pin;
	gpio_struct.Mode = LL_GPIO_MODE_OUTPUT;
	gpio_struct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	gpio_struct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_struct.Pull = LL_GPIO_PULL_NO;
	
	LL_GPIO_Init(port, &gpio_struct);
	
}

void IO_initMotors(void){
	// Enable pin
	IO_Output_init(IO_MOTOR_EN);
	LL_GPIO_ResetOutputPin(IO_MOTOR_EN); // Disable motors by default;
	
	// Left Motor IO
	IO_Output_init(IO_MOTOR_1_BP);
	IO_Output_init(IO_MOTOR_1_DIR);
	LL_GPIO_ResetOutputPin(IO_MOTOR_1_DIR); // Default direction forward;
	// Servo Motor 1
	
	// Right motor IO
	IO_Output_init(IO_MOTOR_2_BP);
	IO_Output_init(IO_MOTOR_2_DIR);
	LL_GPIO_ResetOutputPin(IO_MOTOR_2_DIR); // Default direction forward
	// Servo Motor 2
	
	// Set the default mode of motors
	if(DEFAULT_MOTOR_MODE){
		LL_GPIO_SetPinMode(IO_MOTOR_1_BP);
		LL_GPIO_SetPinMode(IO_MOTOR_2_BP);
	} else {
		LL_GPIO_ResetOutputPin(IO_MOTOR_1_BP);
		LL_GPIO_ResetOutputPin(IO_MOTOR_2_BP);
	}
	
	// Left Motor PWM
	LL_GPIO_SetPinMode(IO_MOTOR_1_PWM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(IO_MOTOR_1_PWM, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinSpeed(IO_MOTOR_1_PWM, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetAFPin_8_15(IO_MOTOR_1_PWM, LL_GPIO_AF_2);
	
	// Right Motor PWM
	LL_GPIO_SetPinMode(IO_MOTOR_2_PWM, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(IO_MOTOR_2_PWM, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinSpeed(IO_MOTOR_2_PWM, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetAFPin_8_15(IO_MOTOR_2_PWM, LL_GPIO_AF_2);
	
	
	// Enable clock for Timer 3
	LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
	// Set prescaler
	LL_TIM_SetPrescaler(TIM3, __LL_TIM_CALC_PSC(SystemCoreClock, 42000000));
	LL_TIM_EnableARRPreload(TIM3);
	
	/* Set the auto-reload value to have a default counter frequency of 20000 Hz */
	LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM3), DEFAULT_PWM_FREQ));
	//Set Timer Output mode
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
	
	/* Set compare value to 0 of the counter period (0% duty cycle ) */
  
	LL_TIM_OC_SetCompareCH3(TIM3, 0);
	LL_TIM_OC_SetCompareCH4(TIM3, 0);
	/* Enable TIM3_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM3_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
	/* Enable the capture/compare interrupt for channel 4*/
  //LL_TIM_EnableIT_CC1(TIM1);
	/* Enable output channel 3 and4 */
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableAllOutputs(TIM3);
  /* Enable counter */
  LL_TIM_EnableCounter(TIM3);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM3);
}

void IO_initEncoders(void){
	
	/* Left Encoder*/
	LL_GPIO_SetPinMode(		IO_ENC_1_CH_A, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(		IO_ENC_1_CH_A, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(	IO_ENC_1_CH_A, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(	IO_ENC_1_CH_A, LL_GPIO_AF_2);
	
  LL_GPIO_SetPinMode(		IO_ENC_1_CH_B, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(		IO_ENC_1_CH_B, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(	IO_ENC_1_CH_B, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(	IO_ENC_1_CH_B, LL_GPIO_AF_2);
	
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
	LL_TIM_GenerateEvent_UPDATE(TIM5);
	
	
	/* Rigth Encoder */
	
	LL_GPIO_SetPinMode(		IO_ENC_2_CH_A, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(		IO_ENC_2_CH_A, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinSpeed(	IO_ENC_2_CH_A, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetAFPin_8_15(IO_ENC_2_CH_A, LL_GPIO_AF_1);
	

	LL_GPIO_SetPinMode(	 IO_ENC_2_CH_B, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(  IO_ENC_2_CH_B, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinSpeed( IO_ENC_2_CH_B, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetAFPin_0_7(IO_ENC_2_CH_B, LL_GPIO_AF_1);
	
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
	LL_TIM_GenerateEvent_UPDATE(TIM2);
	
	
}

void IO_initAnalogueSensors(void){
	LL_ADC_InitTypeDef ADC_InitStruct;
	LL_GPIO_InitTypeDef ADC_GPIO_InitStruct;
	ADC_GPIO_InitStruct.Pin = IO_ALL_SENSORS;
	ADC_GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	ADC_GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &ADC_GPIO_InitStruct);
	
}


void IO_initLCD(void){
	/* Configure Instruction_Or_Data Pin */

  LL_GPIO_SetPinMode(  IO_LCD_A0, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed( IO_LCD_A0, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(  IO_LCD_A0, LL_GPIO_PULL_UP);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
	
	/* Configure LCD Reset Pin */

  LL_GPIO_SetPinMode(  IO_LCD_RESET, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed( IO_LCD_RESET, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(  IO_LCD_RESET, LL_GPIO_PULL_UP);
	LL_GPIO_SetOutputPin(IO_LCD_RESET);
	
	/* Configure LCD Chip Select Pin */

  LL_GPIO_SetPinMode(  IO_LCD_nCS, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed( IO_LCD_nCS, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(  IO_LCD_nCS, LL_GPIO_PULL_UP);
	LL_GPIO_SetOutputPin(IO_LCD_nCS);
	
	/* Configure SCK Pin */

  LL_GPIO_SetPinMode(  IO_LCD_SCK, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(IO_LCD_SCK, LL_GPIO_AF_6);
  LL_GPIO_SetPinSpeed( IO_LCD_SCK, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(  IO_LCD_SCK, LL_GPIO_PULL_DOWN);

  /* Configure MOSI Pin */

  LL_GPIO_SetPinMode(  IO_LCD_MOSI, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(IO_LCD_MOSI, LL_GPIO_AF_6);
  LL_GPIO_SetPinSpeed( IO_LCD_MOSI, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(  IO_LCD_MOSI, LL_GPIO_PULL_DOWN);
	
	/* Configure MISO Pin */
	LL_GPIO_SetPinMode(  IO_LCD_MISO, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(IO_LCD_MISO, LL_GPIO_AF_6);
  LL_GPIO_SetPinSpeed( IO_LCD_MISO, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(  IO_LCD_MISO, LL_GPIO_PULL_DOWN);
	
}
