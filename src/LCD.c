#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



LCD * LCD_init(void){
  LCD* lcd = malloc(sizeof(LCD));
  memset(lcd->buffer, 0x00, LCD_BUFFER_SIZE);
  lcd->char_x = 0;
  lcd->char_y = 0;
  lcd->orientation=0;
  
  __LCD_SPI_init();
  __LCD_SPI_Activate();
  
	__LCD_instruction;
	__LCD_reset(0)
	LL_mDelay(1);
	__LCD_reset(1)
	LL_mDelay(5);
  __LCD_writeCommand(0xAE);   //  display off
  __LCD_writeCommand(0xA2);   //  bias voltage
 
  __LCD_writeCommand(0xA0);
  __LCD_writeCommand(0xC8);   //  column normal
 
  __LCD_writeCommand(0x22);   //  voltage resistor ratio
  __LCD_writeCommand(0x2F);   //  power on

  __LCD_writeCommand(0x40);   //  start line = 0
  __LCD_writeCommand(0xAF);   //  display ON
 
  __LCD_writeCommand(0x81);   //  set contrast
  __LCD_writeCommand(0x17);   //  set contrast
 
  __LCD_writeCommand(0xA6);   // display normal	
  return lcd;
}

int LCD_printf(LCD* lcd, const char *format, ...){
  char outputString[32];
  va_list argptr;
  va_start(argptr, format);
  vsprintf(outputString, format, argptr);
  va_end(argptr);
  LCD_puts(lcd, lcd->char_x, lcd->char_y, outputString);
}

int LCD_puts(LCD* lcd, int x, int y, char* stringToSend){
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


int LCD_putc(LCD* lcd, int value)
{
    if (value == '\n') {    // new line
        lcd->char_x = 0;
        lcd->char_y += lcd->font[2];
        if (lcd->char_y >= LCD_height(lcd) - lcd->font[2]) {
            lcd->char_y = 0;
        }
    } else {
        LCD_character(lcd, lcd->char_x, lcd->char_y, value);
        __LCD_copyDataBuffer(lcd);
    }
    return value;
}
 
void LCD_character(LCD* lcd, int x, int y, int c)
{
    unsigned int hor,vert,offset,bpl,j,i,b;
    unsigned char* symbol;
    unsigned char z,w;
 
    if ((c < 31) || (c > 127)) return;   // test char range
 
    // read font parameter from start of array
    offset = lcd->font[0];                       // bytes / char
    hor    = lcd->font[1];                       // get hor size of font
    vert   = lcd->font[2];                       // get vert size of lcd->font
    bpl    = lcd->font[3];                       // bytes per line
 
    if (lcd->char_x + hor > LCD_width(lcd)) {
        lcd->char_x = 0;
        lcd->char_y = += vert;
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
                LCD_pixel(x+i,y+j,0);
            } else {
                LCD_pixel(x+i,y+j,1);
            }
 
        }
    }
 
    lcd->char_x += w;
}
 


void LCD_setFont(LCD* lcd, unsigned char* f)
{
    lcd->font = f;
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

void LCD_locate(LCD* lcd, int x, int y)
{
    lcd->char_x = x;
    lcd->char_y = y;
}
 

void LCD_cls(LCD* lcd){
	  memset(lcd->buffer,0x00,LCD_BUFFER_SIZE);  // clear display buffer
	  __LCD_copyDataBuffer(lcd);
}

 
void LCD_pixel(LCD* lcd, uint8_t x, uint8_t y, uint8_t color)
{
    if(x > 128 || y > 32 || x < 0 || y < 0) return;
    if(color == 0)
      lcd->buffer[x] &= ~(1 << (y%32));  // erase pixel
    else
      lcd->buffer[x] |= (1  << (y%32));   // set pixel
}

void LCD_setContrast(LCD* lcd, unsigned int o)
{
    Write_Command_To_LCD(0x81);      
    Write_Command_To_LCD(o & 0x3F);
}





void LCD_fillPage(LCD* lcd, unsigned char Page){
	if(Page == 0){  //Line 1
		for(int i=0;i<32;i++){
			lcd->buffer[i] = 0xffff;
		}
	}
	if(Page == 1){  //Line 2
		for(int i=32;i<64;i++){
			lcd->buffer[i] = 0xffff;
		}
	}
	if(Page == 2){  //Line 3
		for(int i=64;i<96;i++){
			lcd->buffer[i] = 0xffff;
		}
	}
	if(Page == 3){  //Line 4
		for(int i=96;i<128;i++){
			lcd->buffer[i] = 0xffff;
		}
	}
	__LCD_copyDataBuffer(lcd);
}


void __LCD_copyDataBuffer(LCD * lcd)
{    
    //To set 8-bit column address data will start, the address is split into 2 nibbles
	  //to send the lower nibble, 0x0 then the lower nibble of the 8 bit address, so nibble of 0 would be 0x00
	  //to send the higher nibble 0x1 then the higher nibble of the 8 bit address, so nibble of 0 would be 0x10
	  //to set the column start to 0, send 0x00 then 0x10
	
	  int i=0;
	  
	  // Page 0
    __LCD_writeCommand(0x00);      // set column low nibble to 0  
    __LCD_writeCommand(0x10);      // set column hi nibble to 0
    __LCD_writeCommand(0xB0);      // set page address  0
    for(i=0; i<32; i++) {
        __LCD_writeData(lcd->buffer[i]);
    }
 
    // Page 1
    __LCD_writeCommand(0x00);      // set column low nibble to 0
    __LCD_writeCommand(0x10);      // set column hi nibble to 0
    __LCD_writeCommand(0xB1);      // set page address  1
    for(i=32; i<64; i++) {
        __LCD_writeData(lcd->buffer[i]);
    }

    //Page 2
    __LCD_writeCommand(0x00);      // set column low nibble to 0
    __LCD_writeCommand(0x10);      // set column hi nibble to 0
    __LCD_writeCommand(0xB2);      // set page address  2
    for(i=64; i<96; i++) {
        __LCD_writeData(lcd->buffer[i]);
    }
 
    //Page 3
    __LCD_writeCommand(0x00);      // set column low nibble to 0
    __LCD_writeCommand(0x10);      // set column hi nibble to 0
    __LCD_writeCommand(0xB3);      // set page address  3
 
    __LCD_chipSelect(1);
    
    for(i=96; i<128; i++) {
        __LCD_writeData(lcd->buffer[i]);
    }
}


void __LCD_writeData(unsigned char data)
{
    __LCD_data;
    __LCD_chipSelect(0);
	  __LCD_SPI_SendByte(data);
    __LCD_chipSelect(1);
}

void __LCD_writeCommand(unsigned char command)
{
    __LCD_instruction;
    __LCD_chipSelect(0);
	  __LCD_SPI_SendByte(command);
    __LCD_chipSelect(1);
}

void __LCD_SPI_SendByte(uint8_t byte){
		//LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
	  //while(LL_SPI_IsActiveFlag_BSY(SPI1))
		LL_SPI_TransmitData8(SPI3, (unsigned char)byte);
		while(LL_SPI_IsActiveFlag_BSY(SPI3));
	  //LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
}

void __LCD_SPI_init(void){


	//NVIC_SetPriority(SPI1_IRQn, 0);
  //NVIC_EnableIRQ(SPI1_IRQn);
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI3);

  /* Configure SPI1 communication */
  LL_SPI_SetBaudRatePrescaler(SPI3, LL_SPI_BAUDRATEPRESCALER_DIV8);
  LL_SPI_SetTransferDirection(SPI3,LL_SPI_HALF_DUPLEX_TX);
  LL_SPI_SetClockPhase(SPI3, LL_SPI_PHASE_2EDGE);
  LL_SPI_SetClockPolarity(SPI3, LL_SPI_POLARITY_HIGH);
  
	/* Reset value is LL_SPI_MSB_FIRST */
  //LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
  LL_SPI_SetDataWidth(SPI3, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_SetNSSMode(SPI3, LL_SPI_NSS_SOFT); //Chip select handled by software
  LL_SPI_SetMode(SPI3, LL_SPI_MODE_MASTER);
	
	/* Configure SPI1 transfer interrupts */
  /* Enable TXE   Interrupt */
  //LL_SPI_EnableIT_TXE(SPI1);
  /* Enable SPI1 Error Interrupt */
  //LL_SPI_EnableIT_ERR(SPI1);
}

void __LCD_SPI_Activate(void){
	/* Enable SPI1 */
  LL_SPI_Enable(SPI3);
}
