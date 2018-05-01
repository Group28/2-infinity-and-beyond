#include "Magnet.h"
#include "stdlib.h"



Magnet Magnet_init(Analog adc){
  Magnet magnet = malloc(sizeof(__Magnet));
  magnet->adc = adc;
  magnet->rawReading = &adc->buffer.buffer[6];
	magnet->runningAverage = RAf_init(10);
  
  for(int i = 0; i < 10; i++){
    RAf_push(magnet->runningAverage, 0.5);
  }
  
  return magnet;
}

void Magnet_update(Magnet magnet) {
	RAf_push(magnet->runningAverage, (float32_t) *magnet->rawReading/4096.0);
}


float Magnet_rawValue(Magnet magnet){
  return RAf_getAverage(magnet->runningAverage);
}


MagnetValue Magnet_getValue(Magnet magnet){
  MagnetValue returnValue = MAGNET_ERROR;
  float value = RAf_getAverage(magnet->runningAverage);
  if(value > MAGNET_NORTH_THRESHOLD){
    returnValue = MAGNET_NORTH;
  } else if (value < MAGNET_SOUTH_THRESHOLD){
    returnValue = MAGNET_SOUTH;
  } else {
    returnValue = MAGNET_NOMAG;
  }
  
  return returnValue;
}
