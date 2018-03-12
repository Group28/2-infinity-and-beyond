#include "OneWire.h"
#include "GenericTypeDefs.h"

#ifndef __DS2781_H
#define __DS2781_H


typedef struct {
  OW ow;
} __DS2781, *DS2781;



/* ***** function prototypes ***** */
/* Function details can be found in the .c file */
DS2781 DS2781_init(GPIO_TypeDef *port, uint32_t pin);

inline void DS2781_write8bitReg(DS2781 ds, uint8_t addr, uint8_t data)
inline void DS2781_write16bitReg(DS2781 ds, uint8_t addr, uint16_t data)

inline uint8_t DS2781_read8bitReg(DS2781 ds, uint8_t addr)
inline uint16_t DS2781_read16bitReg(DS2781 ds, uint8_t addr)
inline uint32_t DS2781_read32bitReg(DS2781 ds, uint8_t addr)
inline uint65_t DS2781_read64bitReg(DS2781 ds, uint8_t addr)

uint16_t DS2781_readVoltage (DS2781 ds);
uint16_t DS2781_readCurrent (DS2781 ds);

uint32_t DS2781_readAccumulatedCurrent (DS2781 ds);
void DS2781_resetAccumulatedCurrent (DS2781 ds);
uint64_t DS2781_readNetAddress (DS2781 ds);
uint16_t DS2781_readTemperature (DS2781 ds);

uint8_t DS2781_readCurrentOffset (DS2781 ds);
void  DS2781_writeCurrentOffset (DS2781 ds, uint8_t offset);
void DS2781_adjustCurrentOffset (DS2781 ds);
void DS2781_updateControlRegister (DS2781 ds, uint8_t control);

inline uint8_t DS2781_readRAM (DS2781 ds, uint8_t addr);
inline void DS2781_writeRAM (DS2781 ds, uint8_t addr, uint8_t byte);
inline void DS2781_copyEEPROM (DS2781 ds, uint8_t addr);
inline void DS2781_recallEEPROM (DS2781 ds, uint8_t addr);




#endif
