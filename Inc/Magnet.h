#include "main.h"

#ifndef __MAGNET_H
#define __MAGNET_H

typedef struct {
  uint16_t * rawReading;
  
  
} __Magnet, *Magnet;

Magnet Magnet_init(void);

#endif
