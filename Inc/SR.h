#include "main.h"
#include "configuration.h"


#ifndef __SR_H
#define __SR_H

typedef struct {
	uint8_t buffer;
	uint8_t length;
	uint8_t mask;
} __SR, *SR;

SR SR_init(uint8_t length);

void SR_flushBuffer(SR sr);

void SR_shiftIn(SR sr, uint8_t value);

void SR_clr(SR sr);

void SR_set(SR sr, uint8_t value);

void SR_rotateLeft(SR sr);

void SR_rotateRight(SR sr);

void SR_latch(void);
#endif
