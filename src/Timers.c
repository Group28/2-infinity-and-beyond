#include "Timers.h"
#include "configuration.h"

void Timers_init(void){
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM9); 
	LL_TIM_SetCounterMode(TIM9, LL_TIM_COUNTERMODE_UP);

  LL_TIM_SetPrescaler(TIM9, __LL_TIM_CALC_PSC(SystemCoreClock, 1000));

  LL_TIM_EnableARRPreload(TIM9);
  LL_TIM_SetAutoReload(TIM9,  __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM9), MOTOR_SAMPLE_FREQ));

  

  /* Enable the update interrupt */

  LL_TIM_EnableIT_UPDATE(TIM9);

  

  /* Configure the NVIC to handle TIM2 update interrupt */

  NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 5);
	NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);


  /* Enable counter */

  LL_TIM_EnableCounter(TIM9);

  

  /* Force update generation */

  LL_TIM_GenerateEvent_UPDATE(TIM9);
}

