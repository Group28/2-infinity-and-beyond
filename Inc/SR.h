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


/** 
 * Writes the entier buffer to the shift register and latches it to the output 
 *
 * @param sr Shift register pointer
 * @return void
 */
void SR_flushBuffer(SR sr);

/** 
 * Shits one bit into the register without latching it to the output
 *
 * @param sr Shift register pointer
 * @param value binary value 0:1 to be shifted into the register
 * @return void
 */
void SR_shiftIn(SR sr, uint8_t value);

/** 
 * Sets the buffer to 0x00, shifts it out to the register and latches it 
 *
 * @param sr Shift register pointer
 * @return void
 */
void SR_clr(SR sr);

/** 
 * Sets the buffer to value, shifts it out to the register and latches it
 *
 * @param sr Shift register pointer
 * @param value value the register is to be set to
 * @return void
 */
void SR_set(SR sr, uint8_t value);

/** 
 * Rotate the register content left and latch to the output
 *
 * @param sr Shift register pointer
 * @return void
 */
void SR_rotateLeft(SR sr);

/** 
 * Rotate the register to right and latch to the output
 *
 * @param sr Shift register pointer
 * @return void
 */
void SR_rotateRight(SR sr);

/** 
 * Latch the register
 *
 * @return void
 */
void SR_latch(void);

#endif
