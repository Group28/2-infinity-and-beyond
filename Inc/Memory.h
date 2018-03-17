#include "main.h"


#ifndef __MEMORY_H
#define __MEMORY_H

typedef struct {
  uint32_t length;
} __Memory, *Memory;

Memory Memory_init(void);

#endif
