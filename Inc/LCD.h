#include "main.h"
#include "configuration.h"

#ifndef __LCD_H
#define __LCD_H



typedef struct {
  char buffer[LCD_BUFFER_SIZE];
  char * font;
  uint8_t char_x;
  uint8_t char_y;
  uint8_t orientation;
} __LCD, *LCD;



/**
 * Initializes the LCD controller
 *
 * @param 
 * @return Handle for LCD controller or NULL for error
 */
LCD      LCD_init(void);


/**
 * Gets the number of colums in the LCD
 *
 * @param lcd - Handle to the LCD controller 
 * @return Number of columns in the LCD
 */
int      LCD_columns      (LCD lcd);


/**
 * Gets the number of rows in the LCD
 *
 * @param lcd - Handle to the LCD controller 
 * @return Number of rows in the LCD
 */
int      LCD_rows         (LCD lcd);
int      LCD_width        (LCD lcd);
int      LCD_height       (LCD lcd);


void     LCD_setContrast  (LCD lcd, unsigned int o);
void     LCD_setFont      (LCD lcd, char* f);
void     LCD_fillPage     (LCD lcd, char);

void     LCD_cls          (LCD lcd);
void     LCD_cls_buffer   (LCD lcd);

void     LCD_locate       (LCD lcd, uint8_t x, uint8_t y);

void     LCD_pixel        (LCD lcd, uint8_t x,uint8_t y, uint8_t colour);
void     LCD_pixel_buffer (LCD lcd, uint8_t x,uint8_t y, uint8_t colour);

void     LCD_character    (LCD lcd, uint8_t x, uint8_t y, char c);

int      LCD_putc         (LCD lcd, char);
int      LCD_putc_buffer  (LCD lcd, char);

int      LCD_puts         (LCD lcd, char* stringToSend);
int      LCD_puts_buffer  (LCD lcd, char* stringToSend);

int      LCD_printf       (LCD lcd, const char *format, ...);
int      LCD_printf_buffer(LCD lcd, const char *format, ...);

void		 LCD_flushBuffer	(LCD lcd);

#endif
