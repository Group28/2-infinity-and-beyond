#include "main.h"
#include "configuration.h"

#ifndef __LCD_H
#define __LCD_H


/**
 * LCD handle
 *
 */
typedef struct {
  char buffer[LCD_BUFFER_SIZE]; //<LCD Buffer
  char * font;                  //<Current font used for rendering text
  uint8_t char_x;               //<Current X position
  uint8_t char_y;               //<Current Y position
  uint8_t orientation;          //<orientation Unused
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

/**
 * Gets pixel width in the LCD
 *
 * @param lcd - Handle to the LCD controller 
 * @return Number of row pixels in the LCD
 */
int      LCD_width        (LCD lcd);

/**
 * Gets pixel height in the LCD
 *
 * @param lcd - Handle to the LCD controller 
 * @return Number of column pixels in the LCD
 */
int      LCD_height       (LCD lcd);

/**
 * Sets screen contrast
 *
 * @param lcd - Handle to the LCD controller 
 * @param o - contrast value
 */
void     LCD_setContrast  (LCD lcd, unsigned int o);

/**
 * Sets font to be used
 *
 * @param lcd - Handle to the LCD controller 
 * @param f - Pointer to the font 
 */
void     LCD_setFont      (LCD lcd, char* f);

/**
 * Used to fill page - test
 *
 * @param lcd - Handle to the LCD controller 
 * @return 
 */
void     LCD_fillPage     (LCD lcd, char);


/**
 * Clear the screen buffer and flushes it to the screen
 *
 * @param lcd - Handle to the LCD controller 
 * @return 
 */
void     LCD_cls          (LCD lcd);
void     LCD_cls_buffer   (LCD lcd);

/**
 * Locate to a position in the LCD 
 *
 * @param lcd - Handle to the LCD controller 
 * @param x - x coordinate
 * @param y - y coordinate
 * @return 
 */
void     LCD_locate       (LCD lcd, uint8_t x, uint8_t y);

/**
* Set pixel at location to a color and flush to buffer to the screen
*
* @param lcd - Handle to the LCD controller
* @param x - x coordiante
* @param y - y coordinate
* @param colour - color (0: white, 1: black); 
* @return 
*/
void     LCD_pixel        (LCD lcd, uint8_t x,uint8_t y, uint8_t colour);
void     LCD_pixel_buffer (LCD lcd, uint8_t x,uint8_t y, uint8_t colour);


/**
 * Puts a character on the screen in the location
 *
 * @param lcd - Handle to the LCD controller
 * @param x - x coordiante
 * @param y - y coordinate
 * @param c - character
 * @return 
 */
void     LCD_character    (LCD lcd, uint8_t x, uint8_t y, char c);

/**
 * Draws a character on the screen and flushes it
 *
 * @param lcd - Handle to the LCD controller
 * @param char - character
 * @return status
 */
int      LCD_putc         (LCD lcd, char);
int      LCD_putc_buffer  (LCD lcd, char);

/**
 * Writes a scring to the screen and flushes it
 *
 * @param lcd - Handle to the LCD controller
 * @param stringToSend - string pointer to be sent
 * @return status
 */
int      LCD_puts         (LCD lcd, char* stringToSend);
int      LCD_puts_buffer  (LCD lcd, char* stringToSend);

/**
 * Writes formated string to the screen and flushes it
 *
 * @param lcd - Handle to the LCD controller
 * @param format - Format string followed by parameters
 * @return  status
 */
int      LCD_printf       (LCD lcd, const char *format, ...);
int      LCD_printf_buffer(LCD lcd, const char *format, ...);

/**
 * Flushes buffer to the screen
 *
 * @param lcd - Handle to the LCD controller
 */
void		 LCD_flushBuffer	(LCD lcd);

#endif
