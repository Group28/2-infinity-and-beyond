#include "Analog.h"
#include "configuration.h"
#include "utils.h"

#include <stdlib.h>

Analog Analog_init(){
  Analog adc = malloc(sizeof(__Analog));
  
  NVIC_SetPriority(ADC_IRQn, 0); /* ADC IRQ greater priority than DMA IRQ */
  NVIC_EnableIRQ(ADC_IRQn);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1); // Turn on all the clocks
  
  LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV2); // Set ADC clock
  
  
  #if (ADC_CHANNEL_COUNT == 9)
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), (LL_ADC_PATH_INTERNAL_NONE));
  LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_9RANKS);
  
  #elif (ADC_CHANNEL_COUNT == 10)
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), (LL_ADC_PATH_INTERNAL_TEMPSENSOR));
    delay_us(LL_ADC_DELAY_TEMPSENSOR_STAB_US);
    LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_ENABLE_10RANKS);
  #else
    #error Wrong number of ADC channels
  #endif
  
  
  // Set ADC resolution
  LL_ADC_SetResolution(ADC1, ADC_RESOLUTION);
  LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
  
  // Turn on sequencer
  LL_ADC_SetSequencersScanMode(ADC1, LL_ADC_SEQ_SCAN_ENABLE);
  
  // Set trigger
  LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
  
  LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
  
  LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
  
  // Configure the sequencer ranks
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1,   ADC_CH_SENSOR_0);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2,   ADC_CH_SENSOR_2);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3,   ADC_CH_SENSOR_4);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4,   ADC_CH_SENSOR_5);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5,   ADC_CH_SENSOR_3);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_6,   ADC_CH_SENSOR_1);
  
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_7,   ADC_CH_MAGNETIC);
  
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_8,   ADC_CH_A);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_9,   ADC_CH_B);
  //LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_10,  ADC_CH_B_PLUS);
  //LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_11,  ADC_CH_B_MINUS);
  
  
  
  
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_SENSOR_0, ADC_SENSOR_SAMPLETIME);
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_SENSOR_1, ADC_SENSOR_SAMPLETIME);
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_SENSOR_2, ADC_SENSOR_SAMPLETIME);
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_SENSOR_3, ADC_SENSOR_SAMPLETIME);
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_SENSOR_4, ADC_SENSOR_SAMPLETIME);
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_SENSOR_5, ADC_SENSOR_SAMPLETIME);
  
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_MAGNETIC, ADC_MAGNETIC_SAMPLETIME);
  
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_A, ADC_SENS_SAMPLETIME);
  LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_B, ADC_SENS_SAMPLETIME);
  //LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_B_PLUS, ADC_SENS_SAMPLETIME);
  //LL_ADC_SetChannelSamplingTime(ADC1, ADC_CH_B_MINUS, ADC_SENS_SAMPLETIME);
  
  return adc;
}


void Analog_enable(Analog adc){  
  adc->enabled = 1; 
  adc->converting = 0;  // We are not converting
  if (!LL_ADC_IsEnabled(ADC1)) LL_ADC_Enable(ADC1); // Enable the ADC
}


void Analog_startConversion(Analog adc){
  if(LL_ADC_IsEnabled(ADC1)) {
    adc->converting = 1;
    LL_ADC_REG_StartConversionSWStart(ADC1); // triggers the Sequencer
  }
}

uint16_t * Analog_getValues(Analog adc){
  return adc->buffer.buffer; // Get pointer to buffer
}


void ADC_IRQHandler(void)
{
  /* Check whether ADC group regular overrun caused the ADC interruption */
  if(LL_ADC_IsActiveFlag_OVR(ADC1) != 0)
  {
    /* Clear flag ADC group regular overrun */
    LL_ADC_ClearFlag_OVR(ADC1);
    
    /* Call interruption treatment function */
    //AdcGrpRegularOverrunError_Callback();
  }
  /* Check whether ADC group regular end of sequence conversions caused       */
  /* the ADC interruption.                                                    */
  /* Note: On this STM32 serie, ADC group regular end of conversion           */
  /*       must be selected among end of unitary conversion                   */
  /*       or end of sequence conversions.                                    */
  /*       Refer to function "LL_ADC_REG_SetFlagEndOfConversion()".           */
  if(LL_ADC_IsActiveFlag_EOCS(ADC1) != 0)
  {
    /* Clear flag ADC group regular end of sequence conversions */
    LL_ADC_ClearFlag_EOCS(ADC1);
    
    /* Call interruption treatment function */
    //AdcGrpRegularSequenceConvComplete_Callback();
  }
}
