#include "RA.h"
#include "stdlib.h"

RAf RAf_init(int length){
    RAf ra = malloc(sizeof(__RAf));
    
    ra->values = calloc(length, sizeof(float32_t));
    
    ra->length = length;
    ra->pointer = 0;
    
    return ra;
}



float RAf_getAverage(RAf ra){
  float32_t result;
  arm_mean_f32(ra->values, ra->length, &result);
  return (float) result;
}
void RAf_push(RAf ra, float value){
  ra->values[ra->pointer] = value;
  ra->pointer = (ra->pointer + 1) % ra->length;
}
int RAf_getLength(RAf ra){
  return ra->length;
}

void RAf_setLength(RAf ra, int length){
  ra->length = length;
  ra->pointer = 0;
  realloc(ra->values, length * sizeof(float32_t));
}




RAi RAi_init(int length){
  RAi ra = malloc(sizeof(__RAi));
  
  ra->values = calloc(length, sizeof(int32_t));
  
  ra->length = length;
  ra->pointer = 0;
  
  return ra;
}

int RAi_getAverage(RAi ra){
  q31_t result;
  arm_mean_q31((q31_t*)ra->values, ra->length, &result);
  return (int) result;
  
}

void RAi_push(RAi ra, float value){
  ra->values[ra->pointer] = value;
  ra->pointer = (ra->pointer + 1) % ra->length;
}

int RAi_getLength(RAi ra){
  return ra->length;
}

void RAi_setLength(RAi ra, int length){
  ra->length = length;
  ra->pointer = 0;
  realloc(ra->values, length * sizeof(int32_t));
}
