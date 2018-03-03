#include "DMA.h"
#include "stdlib.h"

static DMA_Buffers buffers;

void DMA_init(DMA_Buffers buffs) {

	buffers = buffs;
  /* (1) Enable the clock of DMA1 and DMA2 */

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
	
	
	/* USART2 - local USB usart transmission;
	*/
	
	NVIC_SetPriority(DMA1_Stream5_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	NVIC_SetPriority(DMA1_Stream6_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Stream6_IRQn);
	
	// USART2 RX
	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_5, LL_DMA_CHANNEL_4); 
	LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_5, 
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY | 
                        LL_DMA_PRIORITY_HIGH              | 
                        LL_DMA_MODE_NORMAL                | 
                        LL_DMA_PERIPH_NOINCREMENT         | 
                        LL_DMA_MEMORY_INCREMENT           | 
                        LL_DMA_PDATAALIGN_BYTE            | 
                        LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_5,
                         (uint32_t)buffers->usbRX->buffer,
                         LL_USART_DMA_GetRegAddr(USART2),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_5));

  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, buffers->usbRX->length);
	
	// USART2 TX
	LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_6, LL_DMA_CHANNEL_4); 


  LL_DMA_ConfigTransfer(DMA1, LL_DMA_STREAM_6, 
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | 
                        LL_DMA_PRIORITY_HIGH              | 
                        LL_DMA_MODE_NORMAL                | 
                        LL_DMA_PERIPH_NOINCREMENT         | 
                        LL_DMA_MEMORY_INCREMENT           | 
                        LL_DMA_PDATAALIGN_BYTE            | 
                        LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_6,
                         (uint32_t)buffers->usbTX->buffer,
                         LL_USART_DMA_GetRegAddr(USART2),
                         LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_6));

  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, buffers->usbTX->length);


  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_5);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_5);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_6);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_6);
	
	
	// USART 6 - Wireless transmission
	
	NVIC_SetPriority(DMA2_Stream1_IRQn, 0);
  NVIC_EnableIRQ(DMA2_Stream1_IRQn);
	NVIC_SetPriority(DMA2_Stream6_IRQn, 0);
  NVIC_EnableIRQ(DMA2_Stream6_IRQn);
	
	// USART6 RX
	LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_1, LL_DMA_CHANNEL_5); 
	LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_1, 
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY | 
                        LL_DMA_PRIORITY_HIGH              | 
                        LL_DMA_MODE_NORMAL                | 
                        LL_DMA_PERIPH_NOINCREMENT         | 
                        LL_DMA_MEMORY_INCREMENT           | 
                        LL_DMA_PDATAALIGN_BYTE            | 
                        LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_1,
                         (uint32_t)buffers->espRX->buffer,
                         LL_USART_DMA_GetRegAddr(USART6),
                         LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_1));

  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, buffers->espRX->length);
	
	// USART6 TX
	LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_6, LL_DMA_CHANNEL_5); 


  LL_DMA_ConfigTransfer(DMA2, LL_DMA_STREAM_6, 
                        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | 
                        LL_DMA_PRIORITY_HIGH              | 
                        LL_DMA_MODE_NORMAL                | 
                        LL_DMA_PERIPH_NOINCREMENT         | 
                        LL_DMA_MEMORY_INCREMENT           | 
                        LL_DMA_PDATAALIGN_BYTE            | 
                        LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_6,
                         (uint32_t)buffers->espTX->buffer,
                         LL_USART_DMA_GetRegAddr(USART6),
                         LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_STREAM_6));

  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_6, buffers->espTX->length);


  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_1);
  LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_1);
  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_6);
  LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_6);
	
	
	/* ADC conversion DMA requests */
	
	NVIC_SetPriority(DMA2_Stream0_IRQn, 1);  /* DMA IRQ lower priority than ADC IRQ */
  NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	
	LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0);
	LL_DMA_ConfigTransfer(DMA2,
										LL_DMA_STREAM_0,
										LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
										LL_DMA_MODE_CIRCULAR              |
										LL_DMA_PERIPH_NOINCREMENT         |
										LL_DMA_MEMORY_INCREMENT           |
										LL_DMA_PDATAALIGN_HALFWORD        |
										LL_DMA_MDATAALIGN_HALFWORD        |
										LL_DMA_PRIORITY_HIGH               );

		/* Set DMA transfer addresses of source and destination */
		LL_DMA_ConfigAddresses(DMA2,
										LL_DMA_STREAM_0,
										 LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
										 (uint32_t)buffers->adcData->buffer,
										 LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

/* Set DMA transfer size */
LL_DMA_SetDataLength(DMA2,
										LL_DMA_STREAM_0,
									 ADC_CHANNEL_COUNT);

/* Enable DMA transfer interruption: transfer complete */
LL_DMA_EnableIT_TC(DMA2,
										LL_DMA_STREAM_0);

/* Enable DMA transfer interruption: transfer error */
LL_DMA_EnableIT_TE(DMA2,
										LL_DMA_STREAM_0);

/*## Activation of DMA #####################################################*/
/* Enable the DMA transfer */
LL_DMA_EnableStream(DMA2,LL_DMA_STREAM_0);
	
}


DMA_Buffers DMA_getBuffers(USART esp, USART usb, LCD lcd, ADC adc){
	DMA_Buffers dma_buffers = malloc(sizeof(__DMA_Buffers));
	dma_buffers->espRX = &esp->buffRX;
	dma_buffers->espTX = &esp->buffTX;
	
	dma_buffers->usbRX = &usb->buffRX;
	dma_buffers->usbTX = &usb->buffTX;
	
	dma_buffers->lcdTX = NULL; // TODO - implemnt LCD DMA requests
	
	dma_buffers->adcData = &adc->buffer;
	
	return dma_buffers;
}


int DMA_StartSerialTransfer(USART usart) {
	LL_USART_EnableDMAReq_TX(usart->usart);
	usart->buffTX.send = 1;
	if(usart->usart == USART2){
		LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, usart->buffTX.index);
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
	} else if(usart->usart == USART6) {
		LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_6, usart->buffTX.index);
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_6);
	}
	
	return 0;
	

}


// USART2 RX Callback
void DMA1_Stream5_IRQHandler(void){
	if(LL_DMA_IsActiveFlag_TC5(DMA1)){

    LL_DMA_ClearFlag_TC5(DMA1);

    /* Call function Reception complete Callback */

  } else if(LL_DMA_IsActiveFlag_TE5(DMA1)){

    /* Call Error function */

  }
}


// USART2 TX Callback
void DMA1_Stream6_IRQHandler(void){
	if(LL_DMA_IsActiveFlag_TC6(DMA1)) {
    LL_DMA_ClearFlag_TC6(DMA1);
		

    /* Call function Transmission complete Callback */
		buffers->usbTX->index = 0;
		buffers->usbTX->send = 0;
		LL_USART_DisableDMAReq_TX(USART2);

  } else if(LL_DMA_IsActiveFlag_TE6(DMA1)) {

    /* Call Error function */
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);

    

  }
}


// USART6 RX Callback
void DMA2_Stream1_IRQHandler(void){
	if(LL_DMA_IsActiveFlag_TC1(DMA2)){

    LL_DMA_ClearFlag_TC1(DMA2);

    /* Call function Reception complete Callback */

  } else if(LL_DMA_IsActiveFlag_TE1(DMA2)){

    /* Call Error function */

  }
}


// USART6 TX Callback
void DMA2_Stream6_IRQHandler(void){
	if(LL_DMA_IsActiveFlag_TC6(DMA2)) {
    LL_DMA_ClearFlag_TC6(DMA2);
		

    /* Call function Transmission complete Callback */
		buffers->espTX->index = 0;
		buffers->espTX->send = 0;
		LL_USART_DisableDMAReq_TX(USART6);

  } else if(LL_DMA_IsActiveFlag_TE6(DMA2)) {

    /* Call Error function */
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_6);

    

  }
}


// ADC Callback

void DMA2_Stream0_IRQHandler(void)
{
  /* Check whether DMA transfer complete caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TC0(DMA2) == 1)
  {
    /*  Clear Stream  transfer complete flag*/
    LL_DMA_ClearFlag_TC0(DMA2);
    /* Call interruption treatment function */
    //AdcDmaTransferComplete_Callback();
  }
  
  /* Check whether DMA transfer error caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TE0(DMA2) == 1)
  {
    /* Clear flag DMA transfer error */
    LL_DMA_ClearFlag_TE0(DMA2);
    
    /* Call interruption treatment function */
    //AdcDmaTransferError_Callback();
  }
}
