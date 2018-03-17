#include "SR.h"
#include <stdlib.h>


//#define SR_delay __ASM{nop};
#define SR_delay ;
#define SENSOR_OPTIMAL  // Optimize buffer flush for a 6 sensor configuration

/* Buffer
	7 6 5 4 3 2 1 0 <- buffer index (Lower number = most recent shifted bit)
  ---------------
	x x 0 0 0 0 0 0 <- 6 sensor buffer clear state
	0 0 1 0 0 0 0 0 < 6 sensor mask

*/


static inline void shiftBit(uint8_t bit){
	if(bit)
		LL_GPIO_SetOutputPin(IO_SR_DATA);
	else
		LL_GPIO_ResetOutputPin(IO_SR_DATA);

	LL_GPIO_SetOutputPin(IO_SR_CLK);
	SR_delay;
	LL_GPIO_ResetOutputPin(IO_SR_CLK);
}



SR SR_init(uint8_t length){
	SR sr = malloc(sizeof(__SR));
	sr->length = length;
	sr->mask = 0x01 << (sr->length - 1);
	SR_clr(sr);
	
	return sr;
}


void SR_flushBuffer(SR sr){

	#ifdef SENSOR_OPTIMAL
	shiftBit(sr->buffer & 0x20);
	shiftBit(sr->buffer & 0x10);
	shiftBit(sr->buffer & 0x08);
	shiftBit(sr->buffer & 0x04);
	shiftBit(sr->buffer & 0x02);
	shiftBit(sr->buffer & 0x01);
	#else
	uint8_t cpBuffer = sr->buffer;
	for(uint8_t i = 0; i < sr->length; i++){
		shiftBit(cpBuffer & sr->mask);
		cpBuffer <<= 1;
	}
	#endif
	SR_latch();
}

inline void SR_shiftIn(SR sr, uint8_t value){
	sr->buffer = (sr->buffer << 1) | (value & 0x01);
	shiftBit(value & 0x01);
}

void SR_clr(SR sr){
	SR_set(sr, 0);
}

inline void SR_set(SR sr, uint8_t value){
	sr->buffer = value;
	SR_flushBuffer(sr);
}

void SR_rotateLeft(SR sr){
	uint8_t lastBit = !!(sr->buffer & sr->mask);
	SR_shiftIn(sr, lastBit);
	SR_latch();
}

void SR_rotateRight(SR sr){
	uint8_t bit = sr->buffer & 0x01;
	sr->buffer >>= 1;
	sr->buffer |= bit << (sr->length - 1);
	SR_flushBuffer(sr);
}


inline void SR_latch(void){
	LL_GPIO_SetOutputPin(IO_SR_LATCH);
	SR_delay;
	LL_GPIO_ResetOutputPin(IO_SR_LATCH);
}

