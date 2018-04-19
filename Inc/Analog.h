#include "main.h"
#include "utils.h"

#ifndef __ADC_H
#define __ADC_H

/* 
 *  Analog - ADC handle structure
 * 
 * used to represent and interface with the raw values from
 * the analog to digital converter 
 */
typedef struct {
  ADCBuffer buffer;       //< Array used to receive the raw conversions
  uint8_t enabled:1;      //< Set to 1 when ADC is enabled 
  uint8_t converting:1;   //< Set to 1 when ADC conversion is in progress 
} __Analog, *Analog;

/*
 * Initialize the ADC
 *
 * @return Ponter to the Analog handle
 */
Analog Analog_init(void);


/*
 * Enable the ADC
 *
 * @param Analog - ADC handle 
 */
inline void Analog_enable(Analog adc);


/*
 * Starts a sequential conversion
 *
 * @param Analog - ADC handle 
 */
inline void Analog_startConversion(Analog adc);

/*
 * Gets pointer to most recent raw analog values
 *
 * @param Analog - ADC handle 
 * @return uint16_t* - pointer to array of raw analog conversions
 */
uint16_t * Analog_getValues(Analog adc);



#endif
