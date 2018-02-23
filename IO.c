#include "IO.h"

void IO_init(void){
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA); // Enable CLK to GPIOB
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB); // Enable CLK to GPIOB
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC); // Enable CLK to GPIOC

}

void IO_Output_init(OUT_pin * pin){
	LL_GPIO_InitTypeDef pin = {
		pin->pin,
		
	
	}

}
