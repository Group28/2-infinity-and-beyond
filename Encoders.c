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
