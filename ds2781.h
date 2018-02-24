#include "OneWire.h"
#include "GenericTypeDefs.h"

#ifndef __DS2781_H
#define __DS2781_H

/* ***** net address commands ***** */
#define __DS2781_READ_NETADDRESS 0x33
#define __DS2781_SKIP_NETADDRESS 0xCC


/* ***** function commands ***** */
#define __DS2781_READ_DATA 0x69
#define __DS2781_WRITE_DATA 0x6C
#define	__DS2781_COPY_DATA 0x48
#define	__DS2781_RECALL_DATA 0xB8
#define __DS2781_LOCK_EEPROM 0x6A	//DO NOT USE


/* ***** function prototypes ***** */
/* Function details can be found in the .c file */
uint16_t DS2781_readVoltage (void);
uint16_t DS2781_readCurrent (void);
uint32_t DS2781_readAccumulatedCurrent (void);
void DS2781_resetAccumulatedCurrent (void);
QWORD DS2781_readNetAddress (void);
uint16_t DS2781_readTemperature (void);
BYTE DS2781_readCurrentOffset (void);
void  DS2781_writeCurrentOffset (char offset);
void DS2781_adjustCurrentOffset (void);
void DS2781_updateControlRegister (BYTE control);
BYTE DS2781_readRAM (BYTE addr);
void DS2781_writeRAM (BYTE byte, BYTE addr);
void DS2781_copyEEPROM (BYTE addr);
void DS2781_recallEEPROM (BYTE addr);
void DS2781_delay100MSx(BYTE counts);


#define Delay_100MS()	__delay_ms(100);


#endif
