#include "Memory.h"
#include <stdlib.h>


Memory Memory_init(void){
  Memory mem = malloc(sizeof(__Memory));
  
  return mem;
}
