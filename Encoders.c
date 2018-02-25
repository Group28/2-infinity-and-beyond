#include "Encoders.h"
#include "math.h"
#include "stdlib.h"
#include "configuration.h"

Encoder * Encoder_init(TIM_TypeDef * timer, double samplePeriod){
	Encoder * encoder = (Encoder*)malloc(sizeof(Encoder));
	encoder->speed = 0;
	encoder->samplePeriod = samplePeriod;
	encoder->lastCount = 0;
	encoder->revolutions = 0;
	encoder->timer = timer;
	
	LL_TIM_SetCounter(encoder->timer, 0);
	return encoder;
}

double Encoder_getSpeed(Encoder * encoder){
	return encoder->speed;
}

double Encoder_getRevolutions(Encoder * encoder){
	return encoder->revolutions;
}

void Encoder_Reset(Encoder * encoder){
	encoder->speed = 0;
	encoder->revolutions=0;
	if(encoder->timer == TIM2){
		NVIC_DisableIRQ(TIM2_IRQn);	
		LL_TIM_SetCounter(encoder->timer, 0);
		NVIC_EnableIRQ(TIM2_IRQn);	
	} else if(encoder->timer == TIM5) {
		NVIC_DisableIRQ(TIM5_IRQn);	
		LL_TIM_SetCounter(encoder->timer, 0);
		NVIC_EnableIRQ(TIM5_IRQn);	
	}
}


void Encoder_Update(Encoder *encoder){
	int32_t count = (int32_t) LL_TIM_GetCounter(encoder->timer);
	
	encoder->speed = (difference)/(samplePeriod*ENCODER_TICKS_PER_REV);
	encoder->revolutions = 0;
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
