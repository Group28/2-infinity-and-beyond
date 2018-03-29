#include "Magnet.h"


Magnet Magnet_init(Analog adc){
  Magnet magnet = malloc(sizeof(__Magnet));
  magnet->adc = adc
  magent->rawReading = &adc->buffer.buffer[6]
  
  return magnet;
}
