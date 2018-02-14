#include "main.h"

void SystemClock_Config(void);
void Configure_Timer2_pwm(int T, double D);
void Config_LED(void);

int x;
uint32_t uwMeasuredDutyCycle = 0; 

int main(void)
{
  /* Configure the system clock to 84 MHz */
  SystemClock_Config();
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOA
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Enable CLK to GPIOB
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); // Enable CLK to GPIOC
	
	Config_LED();
	int T = 1865; 
	double D = 99;
	Configure_Timer2_pwm(T,D);
	while(1);
}
	
	
  /* Infinite loop */
 


/**
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  */
void SystemClock_Config(void)
{
  // To understand this code, look at the system block diagram. The SoC, by default, has all peripherals Clock gated (off).
  // The oscillator can either be HSI (High speed internal oscillator which is 16 MHz 1% accuracy) or it can be HSE (High Speed External Oscillator)
  // which on the NUCLEO_F401RE is the 8 MHz external crystal on the ST-LINK part of the board.
  // The flash memory of the MCU can be read at different latencies and must be setup before it can be used. 
  // The 8 MHz oscillator is sent through a phase locked look (PLL) and clocked up to 84 MHz by the Reset and Clock Control Register (RCC)
  // The peripherals such as USART, Timers, ADC communicate to the core through the APB1 and APB2 bus structure

  /* Enable HSE oscillator */
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1){};

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_4);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1){};

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL){};

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms */
  SysTick_Config(84000000 / 1000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 84000000;
}

void Configure_Timer2_pwm(int T, double D ) {
//Functions for setting up clocks to peripherals
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  LL_TIM_SetPrescaler(TIM2,0 );
	LL_TIM_EnableARRPreload(TIM2);
	LL_TIM_SetAutoReload(TIM2, T );
	LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetCompareCH1(TIM2, (100-D)*T/100 );
	LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableIT_CC1(TIM2);
	NVIC_SetPriority(TIM2_IRQn, 0);
	NVIC_EnableIRQ(TIM2_IRQn); 
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_GenerateEvent_UPDATE(TIM2);
}

void Config_LED(void) {
	LL_GPIO_SetPinMode (GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT); //set PA5 as output
	LL_GPIO_SetPinOutputType (GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL); //output type is push-pull
	LL_GPIO_SetPinPull (GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_NO); //set PA5 as NO pull
	LL_GPIO_SetPinSpeed (GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_NO);
}
void TIM2_IRQHandler(void) {
		if(LL_TIM_IsActiveFlag_UPDATE(TIM2)) {
			LL_TIM_ClearFlag_UPDATE(TIM2);
			uwMeasuredDutyCycle = (LL_TIM_GetCounter(TIM2) * 100 )/ (LL_TIM_GetAutoReload(TIM2) +1);
			LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
		}
	}
	

