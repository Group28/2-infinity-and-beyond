#include "LCD.h"
#include "configuration.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define LCD_TEST_SPI				1
#define LCD_SPI 						SPI1
#define LCD_SPI_IRQHandler 	SPI1_IRQHandler
#define LCD_SPI_IRQn 				SPI1_IRQn


static Buffer SPI_TX_BUFFER;


LCD * LCD_init(void){
  LCD* lcd = (LCD*)malloc(sizeof(LCD));
  memset(lcd->buffer, 0x00, sizeof(lcd->buffer[0])*LCD_BUFFER_SIZE);
	
	SPI_TX_BUFFER.buffer = NULL;
	SPI_TX_BUFFER.length = 0;
	SPI_TX_BUFFER.index = 0;
	SPI_TX_BUFFER.send = 0;
	
  lcd->char_x = 0;
  lcd->char_y = 0;
  lcd->orientation=0;
  
  __LCD_SPI_init();
  __LCD_SPI_activate();
  
	__LCD_instruction;
	__LCD_reset(0);
	delay_ms(1);
	__LCD_reset(1);
	delay_ms(5);
	
  __LCD_writeCommand(__LCD_CMD_DISPLAY_OFF);   //  display off
  __LCD_writeCommand(__LCD_CMD_LCD_BIAS_1_9);   //  bias voltage
 
  __LCD_writeCommand(__LCD_CMD_ADC_NORMAL);
  __LCD_writeCommand(__LCD_CMD_COMMON_OUT_REVERSE);   //  column normal
 
  __LCD_writeCommand(__LCD_CMD_RESISTOR_RATIO | 0x02);   //  voltage resistor ratio
  __LCD_writeCommand(__LCD_CMD_PWR_MODE | 0x07);   //  power on

  __LCD_writeCommand(__LCD_CMD_DISPLAY_SL);   //  start line = 0
  __LCD_writeCommand(__LCD_CMD_DISPLAY_ON);   //  display ON
 
  __LCD_writeCommand(__LCD_CMD_CONTRAST);   //  set contrast
  __LCD_writeCommand(0x17);   //  set contrast
 
  __LCD_writeCommand(__LCD_CMD_DISP_NORMAL);   // display normal	
  
	return lcd;
}

int LCD_printf(LCD* lcd, const char *format, ...){
  char outputString[128];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(outputString, format, argptr);
  va_end(argptr);
  return LCD_puts(lcd, lcd->char_x, lcd->char_y, outputString);
}

int LCD_puts(LCD* lcd, uint8_t x, uint8_t y, char* stringToSend){
	LCD_puts_buffer(lcd, x, y, stringToSend);
	__LCD_copyDataBuffer(lcd);
	return 1;
}

int LCD_puts_buffer(LCD* lcd, uint8_t x, uint8_t y, char* stringToSend){
	LCD_locate(lcd, x,y);
	int width_of_font = lcd->font[1]-1;
	int length = strlen(stringToSend);
	for(int i=0; i<length;i++){
		LCD_putc(lcd, stringToSend[i]);
		x+=width_of_font; 
		LCD_locate(lcd, x,y);
	}
	return 1;
}


int LCD_putc(LCD* lcd, char value)
{
    if (value == '\n') {    // new line
        lcd->char_x = 0;
        lcd->char_y += lcd->font[2];
        if (lcd->char_y >= LCD_height(lcd) - lcd->font[2]) {
            lcd->char_y = 0;
        }
    } else {
        LCD_character(lcd, lcd->char_x, lcd->char_y, value);
    }
    return value;
}
 
void LCD_character(LCD* lcd, uint8_t x, uint8_t y, char c)
{
    uint16_t hor,vert,offset,bpl,j,i,b;
    char* symbol;
    char z,w;
 
    if ((c < 31) || (c > 127)) return;   // test char range
 
    // read font parameter from start of array
    offset = lcd->font[0];                       // bytes / char
    hor    = lcd->font[1];                       // get hor size of font
    vert   = lcd->font[2];                       // get vert size of lcd->font
    bpl    = lcd->font[3];                       // bytes per line
 
    if (lcd->char_x + hor > LCD_width(lcd)) {
        lcd->char_x = 0;
        lcd->char_y += vert;
        if (lcd->char_y >= LCD_height(lcd) - lcd->font[2]) {
            lcd->char_y = 0;
        }
    }
 
    symbol = &lcd->font[((c -32) * offset) + 4]; // start of char bitmap
    w = symbol[0];                          // width of actual char
                                            // construct the char into the buffer
    for (j=0; j<vert; j++) {                //  vert line
        for (i=0; i<hor; i++) {             //  horz line
            z =  symbol[bpl * i + ((j & 0xF8) >> 3)+1];
            b = 1 << (j & 0x07);
            if (( z & b ) == 0x00) {
                LCD_pixel(lcd, x+i,y+j,0);
            } else {
                LCD_pixel(lcd, x+i,y+j,1);
            }
 
        }
    }
 
    lcd->char_x += w;
}
 
void LCD_flushBuffer(LCD* lcd){
	__LCD_copyDataBufferFast(lcd);
}


void LCD_setFont(LCD* lcd, char* f)
{
    lcd->font = &f[0];
}

int LCD_width(LCD* lcd)
{
    if (lcd->orientation == 0 || lcd->orientation == 2) return 32;
    else return 128;
}
 
int LCD_height(LCD* lcd)
{
    if (lcd->orientation == 0 || lcd->orientation == 2) return 128;
    else return 32;
}

int LCD_columns(LCD* lcd)
{
    return LCD_width(lcd) / lcd->font[1];
}
 
 
 
int LCD_rows(LCD* lcd)
{
    return LCD_height(lcd) / lcd->font[2];
}

void LCD_locate(LCD* lcd, uint8_t x, uint8_t y)
{
    lcd->char_x = x;
    lcd->char_y = y;
}
 

void LCD_cls(LCD* lcd){
	  memset(lcd->buffer,0x00,sizeof(lcd->buffer[0])*LCD_BUFFER_SIZE);  // clear display buffer
	  __LCD_copyDataBuffer(lcd);
}

 
void LCD_pixel(LCD* lcd, uint8_t x, uint8_t y, uint8_t color)
{
		if(x >= 128 || y >= 32) return;
    if(color == 0)
      lcd->buffer[x + ((y/8) * 128)] &= ~(1 << (y%8));  // erase pixel
    else
      lcd->buffer[x + ((y/8) * 128)] |= (1  << (y%8));
}


void LCD_setContrast(LCD* lcd, unsigned int o)
{
    __LCD_writeCommand(__LCD_CMD_CONTRAST);      
    __LCD_writeCommand(o & 0x3F);
}

void LCD_fillPage(LCD* lcd, char page){
	if(page == 0){  //Line 1
		for(int i=0;i<128;i++){
			lcd->buffer[i] = 0xff;
		}
	}
	if(page == 1){  //Line 2
		for(int i=128;i<256;i++){
			lcd->buffer[i] = 0xff;
		}
	}
	if(page == 2){  //Line 3
		for(int i=256;i<384;i++){
			lcd->buffer[i] = 0xff;
		}
	}
	if(page == 3){  //Line 4
		for(int i=384;i<512;i++){
			lcd->buffer[i] = 0xff;
		}
	}
	__LCD_copyDataBuffer(lcd);
}


void __LCD_copyDataBuffer(LCD * lcd) {   
  
  // Loop through all pages
  for(uint8_t page = 0; page < 4; page++){
    __LCD_writeCommand(__LCD_CMD_LOWER_BIT | 0x00);
    __LCD_writeCommand(__LCD_CMD_UPPER_BIT | 0x00);
    __LCD_writeCommand(__LCD_CMD_PAGE_ADDR | page);
    
    // Copy the local buffer to the 
    for(uint8_t bufferIndex = 0; bufferIndex < 128; bufferIndex++){
      __LCD_writeData(lcd->buffer[128*page + bufferIndex]);
    }
  }
  
  // End transmission
  //__LCD_writeCommand(__LCD_CMD_END);
}

void __LCD_copyDataBufferFast(LCD *lcd) {
	SPI_TX_BUFFER.buffer = lcd->buffer;
	SPI_TX_BUFFER.send = 1;
	//SPI_TX_BUFFER.length = LCD_BUFFER_SIZE;
	
	for(uint8_t page = 0; page < 4; page++){
		__LCD_writeCommand(__LCD_CMD_LOWER_BIT | 0x00);
    __LCD_writeCommand(__LCD_CMD_UPPER_BIT | 0x00);
    __LCD_writeCommand(__LCD_CMD_PAGE_ADDR | page);
    
    // Copy the local buffer to the 
		
		SPI_TX_BUFFER.index = page * 128;
		SPI_TX_BUFFER.length = SPI_TX_BUFFER.index + 128;
		SPI_TX_BUFFER.send = 1;
		__LCD_SPI_sendDataBuffer();
		while(LL_SPI_IsActiveFlag_BSY(LCD_SPI));
	}
}



void __LCD_writeData(unsigned char data) {
    __LCD_data;
    __LCD_chipSelect(0);
	  __LCD_SPI_sendByte(data);
    __LCD_chipSelect(1);
}

void __LCD_writeCommand(unsigned char command) {
    __LCD_instruction;
    __LCD_chipSelect(0);
	  __LCD_SPI_sendByte(command);
    __LCD_chipSelect(1);
}

void __LCD_SPI_sendDataBuffer(void){
	
	__LCD_data;
	__LCD_chipSelect(0);
	LL_SPI_EnableIT_TXE(LCD_SPI);
	LL_SPI_TransmitData8(LCD_SPI, (unsigned char)SPI_TX_BUFFER.buffer[SPI_TX_BUFFER.index]);
}


void __LCD_SPI_sendByte(uint8_t byte){

		LL_SPI_TransmitData8(LCD_SPI, (unsigned char)byte);
		while(LL_SPI_IsActiveFlag_BSY(LCD_SPI));
}

void __LCD_SPI_init(void){


	
	#if LCD_TEST_SPI
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
	#else
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
	#endif

  /* Configure SPI1 communication */
  LL_SPI_SetBaudRatePrescaler(LCD_SPI, LL_SPI_BAUDRATEPRESCALER_DIV256);
  LL_SPI_SetTransferDirection(LCD_SPI,LL_SPI_HALF_DUPLEX_TX);
  LL_SPI_SetClockPhase(LCD_SPI, LL_SPI_PHASE_2EDGE);
  LL_SPI_SetClockPolarity(LCD_SPI, LL_SPI_POLARITY_HIGH);
  
	/* Reset value is LL_SPI_MSB_FIRST */
  //LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
  LL_SPI_SetDataWidth(LCD_SPI, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_SetNSSMode(LCD_SPI, LL_SPI_NSS_SOFT); //Chip select handled by software
  LL_SPI_SetMode(LCD_SPI, LL_SPI_MODE_MASTER);
	
	
	NVIC_SetPriority(LCD_SPI_IRQn, 5);
  NVIC_EnableIRQ(LCD_SPI_IRQn);
	
	/* Configure SPI1 transfer interrupts */
  /* Enable TXE   Interrupt */
  LL_SPI_DisableIT_TXE(LCD_SPI); // Diable by default
  /* Enable SPI1 Error Interrupt */
  LL_SPI_EnableIT_ERR(LCD_SPI);
}

void __LCD_SPI_activate(void){
	/* Enable SPI1 */
  LL_SPI_Enable(LCD_SPI);
}


void LCD_SPI_IRQHandler(void)
{
  /* Check TXE flag value in ISR register */
  if(LL_SPI_IsActiveFlag_TXE(LCD_SPI))
  {
		if(SPI_TX_BUFFER.send){
			if(++SPI_TX_BUFFER.index < SPI_TX_BUFFER.length){
				LL_SPI_TransmitData8(LCD_SPI, (unsigned char)SPI_TX_BUFFER.buffer[SPI_TX_BUFFER.index]);
			} else {
				SPI_TX_BUFFER.send = 0;
				__LCD_chipSelect(1);
				LL_SPI_DisableIT_TXE(LCD_SPI);
			}
		}
  }
}
