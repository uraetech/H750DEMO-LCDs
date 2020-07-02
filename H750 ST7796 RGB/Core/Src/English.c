/*
 * English.c
 *
 *  Created on: Jun 29, 2020
 *      Author: NSJ
 */

#include "main.h"
#include "ST7796S_RGB.h"
#include "LCD_font.h"
#include "English.h"

#define ABS(x)   ((x) > 0 ? (x) : -(x))

uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;


void LCD_SetTextColor(uint16_t color)
{
	POINT_COLOR = color;
}

void LCD_SetBackgroundColor(uint16_t color)
{
	BACK_COLOR = color;
}

void LCD_Putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  uint16_t colortemp = POINT_COLOR;

  if(x > (BSP_LCD_GetXSize() - 8) || y > BSP_LCD_GetYSize() - 16) return;
  num=num-' ';

  if(!mode)
  {
    for(pos = 0; pos < 16; pos++)
    {
      temp = asc2_1608[(uint16_t)num*16 + pos];
      for(t = 0; t < 8; t++)
      {
        if(temp&0x01)BSP_LCD_DrawPixel(x+t,y+pos,POINT_COLOR);
        else BSP_LCD_DrawPixel(x+t,y+pos,BACK_COLOR);
        temp>>=1;
      }
    }
  }
  else
  {
    for(pos=0;pos<16;pos++)
    {
      temp=asc2_1608[(uint16_t)num*16+pos];
      for(t=0;t<8;t++)
      {
        if(temp&0x01)BSP_LCD_DrawPixel(x+t,y+pos,POINT_COLOR);
        temp>>=1;
      }
    }
  }

  POINT_COLOR=colortemp;
}

void LCD_Putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>BSP_LCD_GetXSize()-14)||(y>BSP_LCD_GetYSize()-24)) return;
  uint8_t i,j,k,temp;

  for(i=0;i<24/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<14;k++)
      {
        temp=Consolas14x24[(data-' ')*(24/8)*14+k*(24/8)+i];
        if(mode==TFT_STRING_MODE_BACKGROUND)
        {
          if(temp&(0x01<<j))
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

void LCD_Putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>BSP_LCD_GetXSize()-18)||(y>BSP_LCD_GetYSize()-32)) return;
  uint8_t i,j,k,temp;

  for(i=0;i<32/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<18;k++)
      {
        temp=Consolas18x32[(data-' ')*(32/8)*18+k*(32/8)+i];
        if(mode==TFT_STRING_MODE_BACKGROUND)
        {
          if(temp&(0x01<<j))
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

void LCD_Putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>BSP_LCD_GetXSize()-26)||(y>BSP_LCD_GetYSize()-48)) return;
  uint8_t i,j,k,temp;

  for(i=0;i<48/8;i++)
  {
    for(j=0;j<8;j++)
    {
      for(k=0;k<26;k++)
      {
        temp=Consolas26x48[(data-' ')*(48/8)*26+k*(48/8)+i];
        if(mode==TFT_STRING_MODE_BACKGROUND)
        {
          if(temp&(0x01<<j))
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
          	BSP_LCD_DrawPixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}


void LCD_Puts8x16(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  uint8_t font_w=8;
  uint8_t font_h=16;

  while(*(string+i)!='\0')
  {

    if(*(string+i)==0)
    {
      return;
    }

    if(*(string+i)=='\n')
    {
      y += font_h;
      x = 0;
      string++;
    }

    if(x > BSP_LCD_GetXSize() - font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > BSP_LCD_GetYSize() - font_h)
    {
      x = y = 0;
    }

    LCD_Putchar(x, y,*(string+i),TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}

void LCD_Puts14x24(uint16_t x, uint16_t y, uint8_t *string,uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  uint8_t font_w=14;
  uint8_t font_h=24;

  while(*(string+i)!='\0')
  {

    if(*(string+i)==0)
    {
      return;
    }

    if(*(string+i)=='\n')
    {
      y += font_h;
      x = 0;
      string++;
    }

    if(x > BSP_LCD_GetXSize()-font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > BSP_LCD_GetYSize()-font_h)
    {
      x = y = 0;
    }

    LCD_Putchar14x24(x, y, *(string+i), TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}

void LCD_Puts18x32(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i=0;
  uint8_t font_w=18;
  uint8_t font_h=32;

  while(*(string+i)!='\0')
  {

    if(*(string+i)==0)
    {
      return;
    }

    if(*(string+i)=='\n')
    {
      x += font_h;
      y = 0;
      string++;
    }

    if(x > BSP_LCD_GetXSize()-font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > BSP_LCD_GetYSize()-font_h)
    {
      x = y = 0;
    }

    LCD_Putchar18x32(x, y, *(string+i),TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}

void LCD_Puts26x48(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE)
{
  uint8_t i = 0;
  uint8_t font_w = 26;
  uint8_t font_h = 48;

  while(*(string + i) != '\0')
  {
    if(*(string + i) == 0)
    {
      return;
    }

    if(*(string + i) == '\n')
    {
      y += font_h;
      x = 0;
      string++;
    }

    if(x > BSP_LCD_GetXSize() - font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > BSP_LCD_GetYSize() - font_h)
    {
      x = y = 0;
    }

    LCD_Putchar26x48(x, y, *(string+i), TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}
