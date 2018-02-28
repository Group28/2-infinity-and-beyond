#include "utils.h"
#include "System_Functions.h"


void delay_us(uint32_t t){
	uint32_t start, end;
	start = System_micros();
	end = start + t;
	if(start < end){
		while((System_micros() >= start) && (System_micros() < end));
	} else {
		while((System_micros() >= start) || (System_micros() < end));
	}
}

void delay_ms(uint32_t t){
	for(uint32_t i = 0; i < t; i++){
		delay_us(1000);
	}
}

void delay(double t){
	delay_ms((uint32_t) (t*1000.0));
}

