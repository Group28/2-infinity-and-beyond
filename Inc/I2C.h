#include "main.h"
#include "utils.h"

#ifndef __I2C_H
#define __I2C_H

typedef struct {
  Buffer bufferRX;
  Buffer bufferTX;
} __I2C, *I2C;

#endif
