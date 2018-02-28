#include "main.h"

typedef struct {
	unsigned char *buffer;
	uint32_t length;
	uint32_t index;
	uint8_t send;
} Buffer;

