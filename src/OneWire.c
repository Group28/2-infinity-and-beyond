#include "OneWire.h"
#include "stdlib.h"



OW OW_init(GPIO_TypeDef * port, uint32_t pin){
  OW ow = malloc(sizeof(__OW));
  ow->port = port;
  ow->pin = pin;
  
  __OW_INPUT();
  LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_DOWN);
  LL_GPIO_ResetOutputPin(port, pin);
  
  OW_reset(ow)
  
  return ow;
}

bool OW_reset(OW ow){
  bool result = false;
  
  LL_GPIO_ResetOutputPin(ow->port, ow->pin);
  __OW_OUTPUT();
  
  __OW_Delay_H();
  
  __OW_INPUT();
  
  __OW_Delay_I();
  
  if(LL_GPIO_IsInputPinSet(ow->port, ow->pin)) result = true;
  
  __OW_Delay_J();
  return result;
}

void OW_writeBit(OW ow, BYTE b){
  LL_GPIO_ResetOutputPin(ow->port, ow->pin);
  __OW_OUTPUT();
  if(b){
    __OW_Delay_A();
    __OW_INPUT();
    __OW_Delay_B();
    
  } else {
    __OW_Delay_C();
    __OW_INPUT();
    __OW_Delay_D();
  }
  
}

BYTE OW_readBit(OW ow){
  BYTE result;
  
  LL_GPIO_ResetOutputPin(ow->port, ow->pin);
  __OW_OUTPUT();
  
  __OW_Delay_A();
  
  __OW_INPUT();
  __OW_Delay_E();
  result = (BYTE) LL_GPIO_IsInputPinSet(ow->port, ow->pin);
  
  __OW_Delay_F();
  
  return result & 0x01;
}

void OW_writeByte(OW ow, BYTE byte){
  for(uint8_t i= 0; i<8; i++){
    OW_writeBit(ow, byte & 0x01);
    byte >>= 1;
  }
}

BYTE OW_readByte(OW ow){
  BYTE result = 0;
  for(uint8_t i = 0; i < 8; i++){
    result >>=1;
    if(OW_readBit(ow)) result |= 0x80;
  }
  return result;
}
