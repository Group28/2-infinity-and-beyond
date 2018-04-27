#include "Encoders.h"
#include "math.h"
#include "stdlib.h"
#include "configuration.h"

static void Encoder_resetCounter(Encoder encoder){
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

Encoder Encoder_init(TIM_TypeDef * timer, double samplePeriod, uint16_t ticks_per_revolution){
	Encoder encoder = malloc(sizeof(__Encoder));
	encoder->speed = 0;
	encoder->samplePeriod = samplePeriod;
	encoder->lastCount = 0;
	encoder->revolutions = 0;
	encoder->ticks_per_revolution = ticks_per_revolution;
	encoder->timer = timer;
	
	Encoder_reset(encoder);
	return encoder;
}

double Encoder_getAngularSpeed(Encoder encoder){
	return encoder->speed;
}
double Encoder_getSpeed(Encoder encoder){
	return encoder->speed * 2 * 3.141592 * WHEEL_RADIUS;
}

double Encoder_getRevolutions(Encoder encoder){
	return encoder->revolutions;
}
double Encoder_getDistance(Encoder encoder){
	return encoder->revolutions * 2 * 3.141592 * WHEEL_RADIUS;
}

void Encoder_reset(Encoder encoder){
	encoder->speed = 0;
	encoder->revolutions = 0;
	encoder->lastCount = 0;
	Encoder_resetCounter(encoder);
	
}


void Encoder_update(Encoder encoder){
	int32_t count = (int32_t) LL_TIM_GetCounter(encoder->timer);
	int difference = count - encoder->lastCount;
	encoder->speed += (difference)/(encoder->samplePeriod*encoder->ticks_per_revolution);
	encoder->speed /=2;
	encoder->revolutions += difference/(double)encoder->ticks_per_revolution;
	encoder->lastCount = count;
}



//**************Handlers and Callbacks*********************//
void TIM5_IRQHandler(void)
{
  /* Check whether CC1 interrupt is pending */
  if(LL_TIM_IsActiveFlag_CC1(TIM5) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_CC1(TIM5);
  } else if(LL_TIM_IsActiveFlag_CC2(TIM5) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_CC2(TIM5);
  }
}

void TIM2_IRQHandler(void)
{
  /* Check whether CC1 interrupt is pending */
  if(LL_TIM_IsActiveFlag_CC1(TIM2) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_CC1(TIM2);
  } else if(LL_TIM_IsActiveFlag_CC2(TIM2) == 1)
  {
    /* Clear the update interrupt flag*/
    LL_TIM_ClearFlag_CC2(TIM2);
  }
}
