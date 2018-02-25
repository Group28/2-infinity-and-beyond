#include  "System_Functions.h"

static volatile uint32_t SysTick_ticks;

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

  /* Set systick to 1us */
  SysTick_Config(84000000 / 1000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = 84000000;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	SysTick_ticks++;
}

void delay_us(uint32_t t){
	uint32_t start, end;
	start = SysTick_ticks;
	end = start + end;
	if(start < end){
		while((SysTick_ticks >= start) && (SysTick_ticks < end));
	} else {
		while((SysTick_ticks >= start) || (SysTick_ticks < end));
	}
}

void delay_ms(uint32_t t){
	for(uint32_t i = 0; i < t; i++){
		delay_us(1000);
	}
}

void delay(double t){
	uint32_t seconds = (uint32_t) t;
	uint32_t ms = (uint32_t)(t - seconds)*1000;
	uint32_t us = (uint32_t)(t - seconds - ms/1000.0)*1000000;
	for(uint32_t i = 0; i< seconds; i++){
		delay_ms(1000);
	}
	delay_ms(ms);
	delay_us(us);
}
