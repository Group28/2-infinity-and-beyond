#include "Magnet.h"
#include "stdlib.h"


Magnet Magnet_init(Analog adc){
  Magnet magnet = malloc(sizeof(__Magnet));
  magnet->adc = adc;
  magnet->rawReading = &adc->buffer.buffer[6];
  
  return magnet;
}


MagnetValue Magnet_getValue(Magnet magnet){
  MagnetValue returnValue = MAGNET_ERROR;
  if(*magnet->rawReading > MAGNET_NORTH_THRESHOLD){
    returnValue = MAGNET_NORTH;
  } else if (*magnet->rawReading < MAGNET_SOUTH_THRESHOLD){
    returnValue = MAGNET_SOUTH;
  } else {
    returnValue = MAGNET_NOMAG;
  }
  
  return returnValue;
}
