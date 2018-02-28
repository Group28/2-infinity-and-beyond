#include "main.h"


#ifndef __LCD_H
#define __LCD_H

#define LCD_BUFFER_SIZE 512

typedef struct {
  unsigned char buffer[LCD_BUFFER_SIZE];
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
void     LCD_pixel_buffer (LCD* lcd, uint8_t x,uint8_t y, uint8_t colour);
void     LCD_setContrast  (LCD* lcd, unsigned int o);
void     LCD_cls          (LCD* lcd);
void     LCD_cls_buffer   (LCD* lcd);
void     LCD_locate       (LCD* lcd, uint8_t x, uint8_t y);
int      LCD_columns      (LCD* lcd);
int      LCD_rows         (LCD* lcd);
int      LCD_width        (LCD* lcd);
int      LCD_height       (LCD* lcd);
void     LCD_setFont      (LCD* lcd, unsigned char* f);
void     LCD_character    (LCD* lcd, uint8_t x, uint8_t y, char c);
int      LCD_putc         (LCD* lcd, char);
int      LCD_putc_buffer  (LCD* lcd, char);
int      LCD_puts         (LCD* lcd, uint8_t x, uint8_t y, char* stringToSend);
int      LCD_puts_buffer  (LCD* lcd, uint8_t x, uint8_t y, char* stringToSend);
int      LCD_printf       (LCD* lcd, const char *format, ...);
int      LCD_printf_buffer(LCD* lcd, const char *format, ...);

void		 LCD_flushBuffer	(LCD* lcd);

#define  __LCD_reset(x) if(x)                                   \
                          LL_GPIO_SetOutputPin(IO_LCD_RESET);   \
                        else                                    \
                          LL_GPIO_ResetOutputPin(IO_LCD_RESET);
                        

#define  __LCD_chipSelect(x) if(x)                             \
                          LL_GPIO_SetOutputPin(IO_LCD_nCS);    \
                         else                                  \
                          LL_GPIO_ResetOutputPin(IO_LCD_nCS);
                        
                  

#define __LCD_instruction LL_GPIO_ResetOutputPin(IO_LCD_A0)
#define __LCD_data LL_GPIO_SetOutputPin(IO_LCD_A0)

void     __LCD_writeData(unsigned char data);
void     __LCD_writeCommand(unsigned char command);

void     __LCD_copyDataBuffer(LCD* lcd);

// Displa ON/OFF
#define __LCD_CMD_DISPLAY_OFF         0xAE
#define __LCD_CMD_DISPLAY_ON          0xAF

// Display start line set
#define __LCD_CMD_DISPLAY_SL          0x40 // and in 6-bit address

// Page address set
#define __LCD_CMD_PAGE_ADDR           0xB0

// Column address set lower and upper bit
#define __LCD_CMD_LOWER_BIT           0x00
#define __LCD_CMD_UPPER_BIT           0x10


// ADC select/
#define __LCD_CMD_ADC_NORMAL          0xA0
#define __LCD_CMD_ADC_REVERSE         0xA1

// LCD bias set
#define __LCD_CMD_LCD_BIAS_1_9        0xA2
#define __LCD_CMD_LCD_BIAS_1_7        0xA3

// Display all points ON/OFF
#define __LCD_CMD_DISP_POINTS_NORMAL  0xA4
#define __LCD_CMD_DISP_POINTS_ALL     0xA5

// Display normal reverse
#define __LCD_CMD_DISP_NORMAL         0xA6
#define __LCD_CMD_DISP_REVERSE        0xA7

// Read-modify-write
#define __LCD_CMD_CAI_READ            0xE0
#define __LCD_CMD_CAI_WRITE           0xE1

//End
#define __LCD_CMD_END                 0xEE

// Reset
#define __LCD_CMD_RESET               0xE2

// Common output mode select
#define __LCD_CMD_COMMON_OUT_NORMAL   0xC0
#define __LCD_CMD_COMMON_OUT_REVERSE  0xC8

// Power control set
#define __LCD_CMD_PWR_MODE            0x28

// Vo voltage regulator internal resistor ratio set
#define __LCD_CMD_RESISTOR_RATIO      0x20

// Electronic volume mode/register set
#define __LCD_CMD_CONTRAST            0x81

// Sleep mode set
#define __LCD_CMD_SLEEP_ON            0xAC
#define __LCD_CMD_SLEEP_OFF           0xAD

// Booster ratio set
#define __LCD_CMD_BOOSER_RATIO1       0xF8
#define __LCD_CMD_BOOSER_RATIO2_2X    0x00
#define __LCD_CMD_BOOSER_RATIO2_5X    0x01
#define __LCD_CMD_BOOSER_RATIO2_6X    0x03
// NOP

#define __LCD_CMD_NOP                 0xE3




#endif
