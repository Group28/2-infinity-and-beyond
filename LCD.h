#include "main.h"


#ifndef __LCD_H
#define __LCD_H

#define LCD_BUFFER_SIZE 128

typedef struct {
  uint32_t buffer[LCD_BUFFER_SIZE];
  unsigned char * font;
  uint8_t char_x;
  uint8_t char_y;
  uint8_t orientation;
} LCD;


void     __LCD_SPI_init(void);
void     __LCD_SPI_activate(void);
void     __LCD_SPI_sendByte(uint8_t);

LCD*     LCD_init(void);

void     LCD_fillPage     (LCD* lcd, unsigned char);
void     LCD_pixel        (LCD* lcd, uint8_t x,uint8_t y, uint8_t colour);
void     LCD_setContrast  (LCD* lcd, unsigned int o);
void     LCD_cls          (LCD* lcd);
void     LCD_locate       (LCD* lcd, uint8_t x, uint8_t y);
int      LCD_columns      (LCD* lcd);
int      LCD_rows         (LCD* lcd);
int      LCD_width        (LCD* lcd);
int      LCD_height       (LCD* lcd);
void     LCD_setFont      (LCD* lcd, unsigned char* f);
void     LCD_character    (LCD* lcd, uint8_t x, uint8_t y, int c);
int      LCD_putc         (LCD* lcd, char);
int      LCD_puts         (LCD* lcd, uint8_t x, uint8_t y, char* stringToSend);
int      LCD_printf       (LCD* lcd, const char *format, ...);


#define  __LCD_reset(x) if(x)                                   \
                          LL_GPIO_SetOutputPin(IO_LCD_RESET);   \
                        else                                    \
                          LL_GPIO_ResetOutputPin(IO_LCD_RESET)
                        

#define  __LCD_chipSelect(x) if(x)                             \
                          LL_GPIO_SetOutputPin(IO_LCD_nCS);    \
                         else                                  \
                          LL_GPIO_ResetOutputPin(IO_LCD_nCS)
                        
                  

#define __LCD_instruction LL_GPIO_ResetOutputPin(IO_LCD_A0)
#define __LCD_data LL_GPIO_SetOutputPin(IO_LCD_A0)

void     __LCD_writeData(unsigned char data);
void     __LCD_writeCommand(unsigned char command);

void     __LCD_copyDataBuffer(LCD* lcd);


#endif
