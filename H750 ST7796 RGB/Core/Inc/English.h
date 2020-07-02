/*
 * English.h
 *
 *  Created on: Jun 29, 2020
 *      Author: NSJ
 */

#ifndef INC_ENGLISH_H_
#define INC_ENGLISH_H_


/* Define TFT String Mode */
#define TFT_STRING_MODE_NO_BACKGROUND		0x01
#define TFT_STRING_MODE_BACKGROUND			0x00




void LCD_SetTextColor(uint16_t color);
void LCD_SetBackgroundColor(uint16_t color);

void LCD_Putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
void LCD_Putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void LCD_Putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void LCD_Putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);

void LCD_Puts8x16(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE);
void LCD_Puts14x24(uint16_t x, uint16_t y, uint8_t *string,uint8_t TFT_STRING_MODE);
void LCD_Puts18x32(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE);
void LCD_Puts26x48(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE);


#endif /* INC_ENGLISH_H_ */
