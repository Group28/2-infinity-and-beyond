#include "main.h"

#ifndef __RUNNINGAVERAGE_H
#define __RUNNINGAVERAGE_H



typedef struct {
    float32_t * values;
    uint32_t pointer;
    uint32_t length;
} __RAf, *RAf;

typedef struct {
  int32_t * values;
  uint32_t pointer;
  uint32_t length;
} __RAi, *RAi;


RAf RAf_init(int length);
float RAf_getAverage(RAf ra);
void RAf_push(RAf ra, float value);
int RAf_getLength(RAf ra);

void RAf_setLength(RAf ra, int length);


RAi RAi_init(int length);
int RAi_getAverage(RAi ra);
void RAi_push(RAi ra, float value);
int RAi_getLength(RAi ra);

void RAi_setLength(RAi ra, int length);



#endif
