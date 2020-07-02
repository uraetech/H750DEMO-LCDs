/*
 * ST7796S RGB.h
 *
 *  Created on: 2020. 5. 28.
 *      Author: NSJ
 */
#include "main.h"
#include "stm32h7xx_hal.h"

#ifndef INC_ST7796S_RGB_H_
#define INC_ST7796S_RGB_H_

#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F

typedef struct
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;


void ST7796_Init_RGB(void);

uint16_t BSP_LCD_GetXSize(void);
uint16_t BSP_LCD_GetYSize(void);
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code);
uint32_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void BSP_LCD_Clear(uint32_t Color);
void BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color);
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color);
void BSP_LCD_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color);
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t Color);
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount, uint16_t Color);
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint16_t Color);
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color);
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t Color);
void BSP_LCD_FillTriangle(uint16_t X1, uint16_t X2, uint16_t X3, uint16_t Y1, uint16_t Y2, uint16_t Y3, uint16_t Color);
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount, uint16_t Color);
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint16_t Color);
void BSP_LCD_Direction(uint8_t dir);
uint8_t BSP_LCD_GetDirection(void);
void BSP_LCD_SetBackLight(uint16_t brightness);	// 0 <= brightness < 100

#endif /* INC_ST7796S_RGB_H_ */
