#include "ds2781.h"
#include "OneWire.h"
#include <stdbool.h>
#include "utils.h"


/* ***** net address commands ***** */
#define DS2781_READ_NETADDRESS  0x33
#define DS2781_SKIP_NETADDRESS  0xCC


/* ***** function commands ***** */
#define DS2781_READ_DATA        0x69
#define DS2781_WRITE_DATA       0x6C
#define	DS2781_COPY_DATA        0x48
#define	DS2781_RECALL_DATA      0xB8
#define DS2781_LOCK_EEPROM      0x6A	//DO NOT USE

#define DS2781_REG_VOLTAGE      0x0C

#define DS2781_REG_CURRENT      0x0E
#define DS2781_REG_CURRENT_LSB  0x0F

#define DS2781_REG_ACCUM_C      0x10
#define DS2781_REG_TEMP         0x0A
#define DS2781_REG_OFFSETC      0x7B
#define DS2781_REG_CTRL         0x60



void DS2781_init(GPIO_TypeDef *port, uint32_t pin){
  DS2781 ds = malloc(sizeof(__DS2781));
  ds->ow = OW_init(port, pin);
  
  
  return ds;
}



void DS2781_write8bitReg(DS2781 ds, uint8_t addr, uint8_t data){
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_WRITE_DATA);
    OW_writeByte(ds->ow, addr);
    OW_writeByte(ds->ow, byte);
  }
}

void DS2781_write16bitReg(DS2781 ds, uint8_t addr, uint16_t data){
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_WRITE_DATA);
    OW_writeByte(ds->ow, addr);
    OW_writeByte(ds->ow, (uint8_t)(data >> 8));
    OW_writeByte(ds->ow, (uint8_t)(data|0x00FF));
  }
}


uint8_t DS2781_read8bitReg(DS2781 ds, uint8_t addr){
  uint8_t result = 0;
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_READ_DATA);
    OW_writeByte(ds->ow, addr);
    result = ((uint8_t)OW_readByte(ds->ow));
    
  }
  return result;
}

uint16_t DS2781_read16bitReg(DS2781 ds, uint8_t addr){
  uint16_t result = 0;
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_READ_DATA);
    OW_writeByte(ds->ow, addr);
    result = ((uint16_t)OW_readByte(ds->ow)) << 8;
    result |= ((uint16_t)OW_readByte(ds->ow));
    
  }
  return result;
}
uint32_t DS2781_read32bitReg(DS2781 ds, uint8_t addr){
  uint32_t result = 0;
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_READ_DATA);
    OW_writeByte(ds->ow, addr);
    result = ((uint32_t)OW_readByte(ds->ow)) << 24;
    result |= ((uint32_t)OW_readByte(ds->ow)) << 16;
    result |= ((uint32_t)OW_readByte(ds->ow)) << 8;
    result |= ((uint32_t)OW_readByte(ds->ow));
    
  }
  return result;
}

uint64_t DS2781_read64bitReg(DS2781 ds, uint8_t addr){
  uint64_t result = 0;
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_READ_DATA);
    OW_writeByte(ds->ow, addr);
    result = ((uint64_t)OW_readByte(ds->ow)) << 56;
    result |= ((uint64_t)OW_readByte(ds->ow)) << 48;
    result |= ((uint64_t)OW_readByte(ds->ow)) << 40;
    result |= ((uint64_t)OW_readByte(ds->ow)) << 32;
    result |= ((uint64_t)OW_readByte(ds->ow)) << 24;
    result |= ((uint64_t)OW_readByte(ds->ow)) << 16;
    result |= ((uint64_t)OW_readByte(ds->ow)) << 8;
    result |= ((uint64_t)OW_readByte(ds->ow));
    
  }
  return result;
}


uint16_t DS2781_readVoltage (DS2781 ds){
  return DS2781_read16bitReg(ds, DS2781_REG_VOLTAGE) >> 5;
}
uint16_t DS2781_readCurrent (DS2781 ds){
  return DS2781_read16bitReg(ds, DS2781_REG_CURRENT);
  
}
uint32_t DS2781_readAccumulatedCurrent (DS2781 ds){
  return DS2781_read32bitReg(ds, DS2781_REG_ACCUM_C) >> 4;
}
void DS2781_resetAccumulatedCurrent (DS2781 ds){
  DS2781_write16bitReg(ds, DS2781_REG_ACCUM_C, 0x0000);
}
uint64_t DS2781_readNetAddress (DS2781 ds){
  return DS2781_read64bitReg(ds, DS2781_READ_NETADDRESS);
}


uint16_t DS2781_readTemperature (DS2781 ds){
  return DS2781_read16bitReg(ds, DS2781_REG_TEMP) >> 5;
}

uint8_t DS2781_readCurrentOffset (DS2781 ds){
  return DS2781_read8bitReg(ds, DS2781_REG_OFFSETC);
  
}
void  DS2781_writeCurrentOffset (DS2781 ds, uint8_t offset){
  DS2781_write8bitReg(ds, DS2781_REG_OFFSETC, offset);
}

void DS2781_adjustCurrentOffset (DS2781 ds){
  uint8_t offset = 0;
  
  DS2781_writeCurrentOffset(ds, offset);
  delay(3.6);
  
  offset = 256 - DS2781_read8bitReg(ds, DS2781_REG_CURRENT_LSB)
  
  DS2781_writeCurrentOffset(ds, offset);
  
}
void DS2781_updateControlRegister (DS2781 ds, uint8_t control){
  DS2781_write8bitReg(ds, DS2781_REG_CTRL, control);
  
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_COPY_DATA);
    OW_writeByte(ds->ow, DS2781_REG_CTRL);
  }
}

uint8_t DS2781_readRAM (DS2781 ds, uint8_t addr){
  return DS2781_read8bitReg(ds, addr)
  
}
void DS2781_writeRAM (DS2781 ds, uint8_t addr, uint8_t byte){
  DS2781_write8bitReg(ds, addr, byte);
}
void DS2781_copyEEPROM (DS2781 ds, uint8_t addr){
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_COPY_DATA);
    OW_writeByte(ds->ow, addr);
  }  
}
void DS2781_recallEEPROM (DS2781 ds, uint8_t addr){
  if(OW_reset(ds->ow)){
    OW_writeByte(ds->ow, DS2781_SKIP_NETADDRESS);
    OW_writeByte(ds->ow, DS2781_RECALL_DATA);
    OW_writeByte(ds->ow, addr);
  }
}
