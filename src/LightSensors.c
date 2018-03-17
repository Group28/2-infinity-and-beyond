#include "LightSensors.h"
#include <stdlib.h>



LightSensors LightSensors_init(void){
  LightSensors ls = malloc(sizeof(__LightSensors)); 
  
  return ls;
}
