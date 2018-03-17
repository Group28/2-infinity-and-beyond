#include "IO.h"
#include "configuration.h"
#include "main.h"

/* Local static functions */

// Initialize an output
static void IO_Output_init(GPIO_TypeDef * port, uint32_t pin);
static void IO_Analog_init(GPIO_TypeDef * port, uint32_t pin);

static void IO_initMotors(void);
static void IO_initEncoders(void);
static void IO_initAnalogueSensors(void);

static void IO_initLCD(void);
static void IO_initUSART_USB(void);
static void IO_initUSART_ESP(void);

static void IO_initI2C(void);
<<<<<<< HEAD

static void IO_initJoyButton(void);
=======
>>>>>>> d63471aa57a6da3bf7be69d3786f52377901b957

static void IO_initJoyButton(void);

static void IO_initSpeaker(void);


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
	IO_initUSART_ESP();
	
	// Usart2 serial
	IO_initUSART_USB();
	
	// I2C
	IO_initI2C();
	
	// LCD
	IO_initLCD();
	
	
	// Other IO
	IO_initJoyButton();
	
	IO_initSpeaker();
	
	IO_Output_init(IO_LED_BLUE);
	IO_set(IO_LED_BLUE, 1);
	

}

void IO_Output_init(GPIO_TypeDef * port, uint32_t pin){
	
	LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(port,pin, LL_GPIO_PULL_NO);
	
}

void IO_Analog_init(GPIO_TypeDef * port, uint32_t pin){
	LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_ANALOG);
	// LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	// LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
	// LL_GPIO_SetPinPull(port,pin, LL_GPIO_PULL_NO);
	
}

void IO_set(GPIO_TypeDef * port, uint32_t pin, uint8_t state){
	if(state){
		LL_GPIO_SetOutputPin(port, pin);
	} else {
		LL_GPIO_ResetOutputPin(port, pin);
	}
}

void IO_initMotors(void){
	// Enable pin
	IO_Output_init(IO_MOTOR_EN);
	LL_GPIO_ResetOutputPin(IO_MOTOR_EN); // Disable motors by default;
	
	// Left Motor IO
	IO_Output_init(IO_MOTOR_1_BP);
	IO_Output_init(IO_MOTOR_1_DIR);
	LL_GPIO_SetOutputPin(IO_MOTOR_1_DIR); // Default direction forward;
	// Servo Motor 1
	
	// Right motor IO
	IO_Output_init(IO_MOTOR_2_BP);
	IO_Output_init(IO_MOTOR_2_DIR);
	LL_GPIO_SetOutputPin(IO_MOTOR_2_DIR); // Default direction forward
	// Servo Motor 2
	
	// Set the default mode of motors
	if(DEFAULT_MOTOR_MODE){
		LL_GPIO_SetOutputPin(IO_MOTOR_1_BP);
		LL_GPIO_SetOutputPin(IO_MOTOR_2_BP);
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
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
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
  LL_GPIO_SetPinPull(		IO_ENC_1_CH_A, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(	IO_ENC_1_CH_A, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(	IO_ENC_1_CH_A, LL_GPIO_AF_2);
	
  LL_GPIO_SetPinMode(		IO_ENC_1_CH_B, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(		IO_ENC_1_CH_B, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(	IO_ENC_1_CH_B, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetAFPin_0_7(	IO_ENC_1_CH_B, LL_GPIO_AF_2);
	
	NVIC_SetPriority(TIM5_IRQn , 0);
  NVIC_EnableIRQ(TIM5_IRQn );	
	
	// Timer 5 peripheral Configuration
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5); // Enable clock to Timer 5 peripheral
	//Set the pre-scaler value to have TIM2 counter clock equal to 42 MHz */
  LL_TIM_SetPrescaler(TIM5, __LL_TIM_CALC_PSC(SystemCoreClock, 84000000));
	
	LL_TIM_IC_SetActiveInput(TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetFilter(     TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM5, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_IC_SetActiveInput(TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetFilter(     TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM5, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_FALLING);
	

	LL_TIM_SetEncoderMode(TIM5, LL_TIM_ENCODERMODE_X4_TI12);
	//LL_TIM_EnableIT_CC1(TIM5);
	//LL_TIM_EnableIT_CC2(TIM5);
	
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH2);
	TIM5->CCMR1 = 0xF1F1;
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
	LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 84000000));
	
	
	LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_INDIRECTTI);
	LL_TIM_IC_SetFilter(     TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_INDIRECTTI);
	LL_TIM_IC_SetFilter(     TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_IC_SetPrescaler(  TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(   TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
	
	LL_TIM_SetEncoderMode(TIM2, LL_TIM_ENCODERMODE_X4_TI12);
	LL_TIM_EnableIT_CC1(TIM2);
	
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	TIM5->CCMR1 = 0xF1F1;
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_GenerateEvent_UPDATE(TIM2);

	
	
}

void IO_initAnalogueSensors(void){
	IO_Analog_init(IO_SENSOR_0);
	IO_Analog_init(IO_SENSOR_1);
	IO_Analog_init(IO_SENSOR_2);
	IO_Analog_init(IO_SENSOR_3);
	IO_Analog_init(IO_SENSOR_4);
	IO_Analog_init(IO_SENSOR_5);
	
	IO_Analog_init(IO_MAGNETIC);
	
	IO_Analog_init(IO_SENS_A_PLUS);
	IO_Analog_init(IO_SENS_A_MINUS);
	IO_Analog_init(IO_SENS_B_PLUS);
	IO_Analog_init(IO_SENS_B_MINUS);	
}

void IO_initLCD(void){
	
	IO_Output_init(IO_LCD_A0);
	IO_Output_init(IO_LCD_RESET);
	IO_Output_init(IO_LCD_nCS);
	
	
	/* Configure SCK Pin */

  LL_GPIO_SetPinMode(  IO_LCD_SCK, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(IO_LCD_SCK, LL_GPIO_AF_6);
  LL_GPIO_SetPinSpeed( IO_LCD_SCK, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinOutputType(IO_USART2_TX, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(  IO_LCD_SCK, LL_GPIO_PULL_NO);

  /* Configure MOSI Pin */

  LL_GPIO_SetPinMode(  IO_LCD_MOSI, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(IO_LCD_MOSI, LL_GPIO_AF_6);
  LL_GPIO_SetPinSpeed( IO_LCD_MOSI, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetPinOutputType(IO_USART2_TX, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(  IO_LCD_MOSI, LL_GPIO_PULL_NO);
	
	/* Configure MISO Pin */
	//LL_GPIO_SetPinMode(  IO_LCD_MISO, LL_GPIO_MODE_ALTERNATE);
  //LL_GPIO_SetAFPin_0_7(IO_LCD_MISO, LL_GPIO_AF_6);
  //LL_GPIO_SetPinSpeed( IO_LCD_MISO, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  //LL_GPIO_SetPinPull(  IO_LCD_MISO, LL_GPIO_PULL_DOWN);
	
}

void IO_initUSART_USB(void){
	LL_GPIO_SetPinMode(IO_USART2_TX, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(IO_USART2_TX, LL_GPIO_AF_7);        //See AF Mapping PDF in Useful Information Folder
  LL_GPIO_SetPinSpeed(IO_USART2_TX, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(IO_USART2_TX, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(IO_USART2_TX,LL_GPIO_PULL_NO);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(IO_USART2_RX, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(IO_USART2_RX, LL_GPIO_AF_7);        //See AF Mapping PDF in Useful Information Folder
  LL_GPIO_SetPinSpeed(IO_USART2_RX, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(IO_USART2_RX, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(IO_USART2_RX, LL_GPIO_PULL_NO);
}

void IO_initUSART_ESP(void){
	
	  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
	  LL_GPIO_SetPinMode(IO_ESP_TX, LL_GPIO_MODE_ALTERNATE);
	  LL_GPIO_SetAFPin_8_15(IO_ESP_TX, LL_GPIO_AF_8);        //See AF Mapping PDF in Useful Information Folder
	  LL_GPIO_SetPinSpeed(IO_ESP_TX, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	  LL_GPIO_SetPinOutputType(IO_ESP_TX, LL_GPIO_OUTPUT_PUSHPULL);
	  LL_GPIO_SetPinPull(IO_ESP_TX,LL_GPIO_PULL_NO);

	  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
	  LL_GPIO_SetPinMode(IO_ESP_RX, LL_GPIO_MODE_ALTERNATE);
	  LL_GPIO_SetAFPin_8_15(IO_ESP_RX, LL_GPIO_AF_8);        //See AF Mapping PDF in Useful Information Folder
	  LL_GPIO_SetPinSpeed(IO_ESP_RX, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	  LL_GPIO_SetPinOutputType(IO_ESP_RX, LL_GPIO_OUTPUT_PUSHPULL);
	  LL_GPIO_SetPinPull(IO_ESP_RX, LL_GPIO_PULL_NO);


}
<<<<<<< HEAD
=======


void IO_initI2C(void){
>>>>>>> d63471aa57a6da3bf7be69d3786f52377901b957

/* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
LL_GPIO_SetPinMode(IO_I2C_SCL, LL_GPIO_MODE_ALTERNATE);
LL_GPIO_SetAFPin_8_15(IO_I2C_SCL, LL_GPIO_AF_4);
LL_GPIO_SetPinSpeed(IO_I2C_SCL, LL_GPIO_SPEED_FREQ_HIGH);
LL_GPIO_SetPinOutputType(IO_I2C_SCL, LL_GPIO_OUTPUT_OPENDRAIN);
LL_GPIO_SetPinPull(IO_I2C_SCL, LL_GPIO_PULL_UP);


void IO_initI2C(void){

/* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
LL_GPIO_SetPinMode(IO_I2C_SCL, LL_GPIO_MODE_ALTERNATE);
LL_GPIO_SetAFPin_8_15(IO_I2C_SCL, LL_GPIO_AF_4);
LL_GPIO_SetPinSpeed(IO_I2C_SCL, LL_GPIO_SPEED_FREQ_HIGH);
LL_GPIO_SetPinOutputType(IO_I2C_SCL, LL_GPIO_OUTPUT_OPENDRAIN);
LL_GPIO_SetPinPull(IO_I2C_SCL, LL_GPIO_PULL_UP);


/* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
LL_GPIO_SetPinMode(IO_I2C_SDA, LL_GPIO_MODE_ALTERNATE);
LL_GPIO_SetAFPin_8_15(IO_I2C_SDA, LL_GPIO_AF_4);
LL_GPIO_SetPinSpeed(IO_I2C_SDA, LL_GPIO_SPEED_FREQ_HIGH);
LL_GPIO_SetPinOutputType(IO_I2C_SDA, LL_GPIO_OUTPUT_OPENDRAIN);
LL_GPIO_SetPinPull(IO_I2C_SDA, LL_GPIO_PULL_UP);
	
}

void IO_initJoyButton(void){
  //Setup User Button GPIO PC13
	LL_GPIO_SetPinMode(IO_CENTER_JOY, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(IO_CENTER_JOY, LL_GPIO_PULL_UP);
	
	//Setup Interrupts for Line 13 EXTI
	//LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);                  \
  //LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13); 
	//LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
	//LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
	//NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable IRQ for EXTI line 13 in NVIC
}

void IO_initSpeaker(void){
// Right Motor PWM
	LL_GPIO_SetPinMode(IO_SPEAKER, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinPull(IO_SPEAKER, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinSpeed(IO_SPEAKER, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	LL_GPIO_SetAFPin_0_7(IO_SPEAKER, LL_GPIO_AF_2);
	
	
	// Enable clock for Timer 3
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	// Set prescaler
	LL_TIM_SetPrescaler(TIM4, 10);
	LL_TIM_EnableARRPreload(TIM4);
	
	/* Set the auto-reload value to have a default counter frequency of 20000 Hz */
	LL_TIM_SetAutoReload(TIM4, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), 440));
	//Set Timer Output mode
	LL_TIM_OC_SetMode(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
	//LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);
	
	/* Set compare value to 0 of the counter period (0% duty cycle ) */
  
	LL_TIM_OC_SetCompareCH1(TIM4, 0);
	//LL_TIM_OC_SetCompareCH4(TIM3, 0);
	/* Enable TIM3_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM3_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH1);
  //LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
	/* Enable the capture/compare interrupt for channel 4*/
  //LL_TIM_EnableIT_CC1(TIM1);
	/* Enable output channel 3 and4 */
  LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
  //LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_EnableAllOutputs(TIM3);
  /* Enable counter */
  LL_TIM_EnableCounter(TIM4);
  /* Force update generation */
  LL_TIM_GenerateEvent_UPDATE(TIM4);
	
}

void IO_setSpeakerFreq(int freq){
		LL_TIM_SetAutoReload(TIM4, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), freq));
		LL_TIM_GenerateEvent_UPDATE(TIM4);
		LL_TIM_OC_SetCompareCH1(TIM4, (50)/100.0 * LL_TIM_GetAutoReload(TIM4));
}
