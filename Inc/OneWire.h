#include "main.h"
#include "GenericTypeDefs.h"
#include <stdbool.h>


#ifndef __ONEWIRE_H
#define __ONEWIRE_H

/**
 * One Wire interface handle
 *
 * used for communication with bettery chip 
 */
typedef struct {
  GPIO_TypeDef * port;  // IO port
	uint32_t pin;        // IO pin
} __OW, *OW;

/**
 * Initialize the one wire interface
 *
 * @param port - IO port
 * @param pin - IO pin
 * @return OW handle
 */
OW OW_init(GPIO_TypeDef *port, uint32_t pin);

/**
 * Reset communication channel
 *
 * @param ow - OW handle
 * @return success
 */
bool OW_reset(OW ow);

/**
 * Writes a bit to the wire
 *
 * @param ow- OW handle
 * @param b - bit to write
 */
void OW_writeBit(OW ow, BYTE b);

/**
 * Read one bit
 *
 * @param ow - OW handle
 * @return read bit 0 or 1
 */
BYTE OW_readBit(OW ow);

/**
 * Write one byte to the wire
 *
 * @param ow - OW handle
 * @param b - byte to write
 */
void OW_writeByte(OW ow, BYTE b);


/**
 * Read a byte from wire
 *
 * @param ow - OW handle
 * @return read byte
 */
BYTE OW_readByte(OW ow);


#endif
