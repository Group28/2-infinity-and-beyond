#include "LightSensors.h"
#include <stdlib.h>



LightSensors LightSensors_init(Analog adc, SR sr, uint8_t count){
  LightSensors ls = malloc(sizeof(__LightSensors)); 
  ls->adc = adc;
  ls->count = count;
  ls->sr = sr;
  
  return ls;
}
