#include "OneWire.h"
#include "stdlib.h"
#include "utils.h"


#define OW_OUTPUT(ow) LL_GPIO_SetPinMode(ow->port, ow->pin, LL_GPIO_MODE_OUTPUT);
#define OW_INPUT(ow)  LL_GPIO_SetPinMode(ow->port, ow->pin, LL_GPIO_MODE_INPUT);


#define OW_Delay_A()	delay_us(5);
#define OW_Delay_B()  delay_us(64);
#define OW_Delay_C()	delay_us(60);
#define OW_Delay_D()	delay_us(10);

#define OW_Delay_E()	delay_us(10);
#define OW_Delay_F()	delay_us(55);

//Delays Used for OWReset (Confirmed Working)
#define OW_Delay_H()	delay_us(480);
#define OW_Delay_I()	delay_us(68);
#define OW_Delay_J()	delay_us(405);


#define OW_Delay_TimeSlot() delay_us(6)
#define OW_Delay_Recovery() delay_us(1)
#define OW_Delay_Write0LT() delay_us(6)
#define OW_Delay_Write1LT() delay_us(1)
#define OW_Delay_ReadData() delay_us(2)
#define OW_Delay_ResetTHi() delay_us(48)
#define OW_Delay_ResetTLo() delay_us(48)
#define OW_Delay_PresenceH() delay_us(2)
#define OW_Delay_PresenceL() delay_us(8)





OW OW_init(GPIO_TypeDef * port, uint32_t pin){
  OW ow = malloc(sizeof(__OW));
  ow->port = port;
  ow->pin = pin;
  
  OW_INPUT(ow);
  LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);
  LL_GPIO_ResetOutputPin(port, pin);
  
  OW_reset(ow);
  
  return ow;
}

bool OW_reset(OW ow){
  bool result = false;
  
  LL_GPIO_ResetOutputPin(ow->port, ow->pin);
  OW_OUTPUT(ow);
  
  OW_Delay_H();
  
  OW_INPUT(ow);
  
  OW_Delay_I();
  
  if(!LL_GPIO_IsInputPinSet(ow->port, ow->pin)) result = true;
  
  OW_Delay_J();
  return result;
}

void OW_writeBit(OW ow, BYTE b){
  LL_GPIO_ResetOutputPin(ow->port, ow->pin);
  OW_OUTPUT(ow);
  if(b){
    OW_Delay_A();
    OW_INPUT(ow);
    OW_Delay_B();
    
  } else {
    OW_Delay_C();
    OW_INPUT(ow);
    OW_Delay_D();
  }
  
}

BYTE OW_readBit(OW ow){
  BYTE result;
  
  LL_GPIO_ResetOutputPin(ow->port, ow->pin);
  OW_OUTPUT(ow);
  
  OW_Delay_A();
  
  OW_INPUT(ow);
  OW_Delay_E();
  result = (BYTE) LL_GPIO_IsInputPinSet(ow->port, ow->pin);
  
  OW_Delay_F();
  
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
