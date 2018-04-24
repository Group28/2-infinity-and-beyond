#include "I2C.h"
#include "stdlib.h"



I2C I2C_init(uint8_t address){
	static uint8_t initialized = 0;
	I2C i2c = malloc(sizeof(__I2C)); // Allocate memory for the i2c handle
	i2c->address = address;
	
	
	// Initialize peripherial only once
	if(!initialized){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
		
		NVIC_SetPriority(I2C1_EV_IRQn, 0);  
		NVIC_EnableIRQ(I2C1_EV_IRQn);
		
		NVIC_SetPriority(I2C1_ER_IRQn, 0);  
		NVIC_EnableIRQ(I2C1_ER_IRQn);
		
		LL_I2C_Disable(I2C1);
		LL_I2C_ConfigSpeed(I2C1, SystemCoreClock/2, 400000, LL_I2C_DUTYCYCLE_2);
		LL_I2C_ConfigFilters(I2C1, LL_I2C_ANALOGFILTER_ENABLE, 0x08);
		
		initialized = 1;
	}
	
  return i2c;
  
}



void I2C_enable(I2C i2c){
	LL_I2C_Enable(I2C1);
	
	LL_I2C_EnableIT_EVT(I2C1);
	LL_I2C_EnableIT_ERR(I2C1);
}

uint8_t I2C_readByte(I2C i2c, uint8_t reg){

}

void I2C_writeByte(I2C i2c, uint8_t reg, uint8_t data){

}

void I2C_readBytes(I2C i2c, uint8_t reg, uint8_t *data, uint8_t length){

}

void I2C_writeBytes(I2C i2c, uint8_t reg, uint8_t *data, uint8_t length){

}


