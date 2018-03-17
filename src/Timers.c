#include "Timers.h"
#include "Encoders.h"
#include "Motors.h"
#include "configuration.h"

extern Motors motors;
extern Encoder encoderLeft;
extern Encoder encoderRight;


static inline void Timer_updateMotors(void);
static inline void Timer_updateSensors(void);

void Timers_init(void){
	
	/* Configure motor update interupt timer */

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





void Timer_updateMotors(){

		Encoder_update(encoderRight);
		Encoder_update(encoderLeft);
		Motors_PID_action(motors);

}

void Timer_updateSensors() {



}




/* Timer interrupts
 *
 *
 *
 *
*/


void TIM1_BRK_TIM9_IRQHandler(){
	if(LL_TIM_IsActiveFlag_UPDATE(TIM9) == 1)
  {
		 /* Clear the update interrupt flag*/
		  LL_TIM_ClearFlag_UPDATE(TIM9);
			Timer_updateMotors();
  }

}
