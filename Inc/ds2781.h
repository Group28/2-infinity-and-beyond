#include "OneWire.h"
#include "GenericTypeDefs.h"

#ifndef __DS2781_H
#define __DS2781_H

/*
 * DS2781 sensor handle
 *
 */
typedef struct {
  OW ow;  //< One wire interface handle
} __DS2781, *DS2781;


/*
 * Initialize the battery voltage and current sensor
 *
 * @param port - Port of data line
 * @param pin - Pin number of data line
 * @return handle to the sensor
 */
DS2781 DS2781_init(GPIO_TypeDef *port, uint32_t pin);


/*
 * Writes a 8bit value to the addressed register
 *
 * @param ds - sensor handle
 * @param addr - register address
 * @param data - 8bit data value
 * @return 
 */
inline void DS2781_write8bitReg(DS2781 ds, uint8_t addr, uint8_t data);

/*
 * Writes a 16bit value to the addressed register
 *
 * @param ds - sensor handle
 * @param addr - register address
 * @param data - 16bit data value
 * @return 
 */
inline void DS2781_write16bitReg(DS2781 ds, uint8_t addr, uint16_t data);


/*
 * Reads a 8bit value to the addressed register
 *
 * @param ds - sensor handle
 * @param addr - register address
 * @return 8bit data value at the address
 */
inline uint8_t DS2781_read8bitReg(DS2781 ds, uint8_t addr);

/*
 * Reads a 16bit value to the addressed register
 *
 * @param ds - sensor handle
 * @param addr - register address
 * @return 16bit data value at the address
 */
inline uint16_t DS2781_read16bitReg(DS2781 ds, uint8_t addr);

/*
 * Reads a 32bit value to the addressed register
 *
 * @param ds - sensor handle
 * @param addr - register address
 * @return 32bit data value at the address
 */
inline uint32_t DS2781_read32bitReg(DS2781 ds, uint8_t addr);

/*
 * Reads a 64bit value to the addressed register
 *
 * @param ds - sensor handle
 * @param addr - register address
 * @return 64bit data value at the address
 */
inline uint64_t DS2781_read64bitReg(DS2781 ds, uint8_t addr);


/*
 * Reads the battery voltage
 *
 * @param ds - sensor handle
 * @return battery voltage (V)
 */
double DS2781_readVoltage (DS2781 ds);

/*
 * Reads the battery current
 *
 * @param ds - sensor handle
 * @return battery current (A)
 */
double DS2781_readCurrent (DS2781 ds);


/*
 * Reads the accumulated battery current - used for capacity estimation
 *
 * @param ds - sensor handle
 * @return accumulated battery current mAh
 */
double DS2781_readAccumulatedCurrent (DS2781 ds);

/*
 * Resets the accumulated battery current
 *
 * @param ds - sensor handle
 */
void DS2781_resetAccumulatedCurrent (DS2781 ds);

/*
 * Reads net address
 *
 * @param ds - sensor handle
 * @return 64bit net address
 */
uint64_t DS2781_readNetAddress (DS2781 ds);

/*
 * Reads the temperature
 *
 * @param ds - sensor handle
 * @return temperature (*C)
 */
double DS2781_readTemperature (DS2781 ds);


/*
 * Reads current callibration offset
 *
 * @param ds - sensor handle
 * @return offset value
 */
uint8_t DS2781_readCurrentOffset (DS2781 ds);

/*
 * Writes current callibration offset
 *
 * @param ds - sensor handle
 * @param offset - offset value
 */
void  DS2781_writeCurrentOffset (DS2781 ds, uint8_t offset);

/*
 * Unknown
 *
 * @param 
 * @return 
 */
void DS2781_adjustCurrentOffset (DS2781 ds);

/*
 * Updates the controll regiseter
 *
 * @param 
 * @return 
 */
void DS2781_updateControlRegister (DS2781 ds, uint8_t control);


/*
 * Unused
 *
 * @param 
 * @return 
 */
inline uint8_t DS2781_readRAM (DS2781 ds, uint8_t addr);


/*
 * Unused
 *
 * @param 
 * @return 
 */
inline void DS2781_writeRAM (DS2781 ds, uint8_t addr, uint8_t byte);

/*
 * Copies EEPROM content to address
 *
 * @param ds - sensor handle
 * @param addr - register address
 */
inline void DS2781_copyEEPROM (DS2781 ds, uint8_t addr);

/*
 * Unused
 *
 * @param 
 * @return 
 */
 inline void DS2781_recallEEPROM (DS2781 ds, uint8_t addr);




#endif
