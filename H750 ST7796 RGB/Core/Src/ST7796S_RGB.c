/*
 * ST7796S RGB.c
 *
 *  Created on: 2020. 5. 28.
 *      Author: NSJ
 */

#include "ST7796S_RGB.h"
#include "spi.h"
#include "ltdc.h"
#include "dma2d.h"
#include "main.h"
#include "tim.h"

volatile uint16_t LCD_HEIGHT = 480;
volatile uint16_t LCD_WIDTH	 = 320;

#define BACKLIGHT_TIMER			htim15

#define LtdcHandler		hltdc
#define ABS(X)  			((X) > 0 ? (X) : -(X))
#define POLY_X(Z)     ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)     ((int32_t)((Points + Z)->Y))


static uint32_t ActiveLayer = 0;
static uint8_t Direction;


/*Send data (char) to LCD*/
void ST7796_SPI_Send(unsigned char SPI_Data)
{
	HAL_SPI_Transmit(&hspi1, &SPI_Data, 1, 1);
}

/* Send command (char) to LCD */
void ST7796_Write_Command(uint8_t Command)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	ST7796_SPI_Send(Command);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

/* Send Data (char) to LCD */
void ST7796_Write_Data(uint8_t Data)
{
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ST7796_SPI_Send(Data);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void ST7796_Init_RGB(void)
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(120);

  ST7796_Write_Command(0x01);		//Software Reset
	HAL_Delay(120);

  ST7796_Write_Command(0x11);		// Sleep Out
	HAL_Delay(120);

  ST7796_Write_Command(0xF0);		//Command Set Control
  ST7796_Write_Data(0xC3);			//D[7:0] = C3h enable command 2 part I

  ST7796_Write_Command(0xF0);		//Command Set Control
  ST7796_Write_Data(0x96);			//D[7:0] = 96h enable command 2 part II

  ST7796_Write_Command(0x36);		//Memory Data Access Control
  ST7796_Write_Data(0x88);			//MY MX MV ML RGB MH

  ST7796_Write_Command(0x3A);		//Interface Pixel Format
  ST7796_Write_Data(0x55);			//¡®101¡¯ = 16bit/pixel

  ST7796_Write_Command(0xB0);		//Interface Mode Control
  ST7796_Write_Data(0x12);			//SPI_EN = 1, 3/4 wire serial interface

  ST7796_Write_Command(0xB5);
  ST7796_Write_Data(8);					//VFP: Front porch of vertical lines =4
  ST7796_Write_Data(4);					//VBP: Back porch of vertical lines =2
  ST7796_Write_Data(10);
  ST7796_Write_Data(10);				//HBP: Back porch of horizontal dotclk=10

  ST7796_Write_Command(0xB6);		//Display Function Control
  ST7796_Write_Data(0x20);	// A0		//D5=1:RGB,D5=0:System Interface D6=1:SYNC Mode   E0  20
  ST7796_Write_Data(0x02);	// 02
  ST7796_Write_Data(0x3B);

  ST7796_Write_Command(0xE8);		//Display Output Ctrl Adjust
  ST7796_Write_Data(0x40);
  ST7796_Write_Data(0x8A);	// 82
  ST7796_Write_Data(0x00);	// 07
  ST7796_Write_Data(0x00);	// 18
  ST7796_Write_Data(0x25);	// 27
  ST7796_Write_Data(0x0A);	// 0A
  ST7796_Write_Data(0x38);	// B6
  ST7796_Write_Data(0x33);	// 33

  ST7796_Write_Command(0xC2);		//Power Control 3
  ST7796_Write_Data(0x0A);	// A7

  ST7796_Write_Command(0xC5);		// VCOM Control
  ST7796_Write_Data(0x1C);	// 3E

  ST7796_Write_Command(0xC6);		//Vcom Offset Register
  ST7796_Write_Data(0x00);

	//POSITIVE GAMMA CORRECTION
	ST7796_Write_Command(0xE0);
	ST7796_Write_Data(0xF0);
	ST7796_Write_Data(0x00);
	ST7796_Write_Data(0x02);
	ST7796_Write_Data(0x0A);
	ST7796_Write_Data(0x0D);
	ST7796_Write_Data(0x1D);
	ST7796_Write_Data(0x35);
	ST7796_Write_Data(0x55);
	ST7796_Write_Data(0x45);
	ST7796_Write_Data(0x3C);
	ST7796_Write_Data(0x17);
	ST7796_Write_Data(0x17);
	ST7796_Write_Data(0x18);
	ST7796_Write_Data(0x1B);

	//NEGATIVE GAMMA CORRECTION
	ST7796_Write_Command(0xE1);
	ST7796_Write_Data(0xF0);
	ST7796_Write_Data(0x00);
	ST7796_Write_Data(0x02);
	ST7796_Write_Data(0x07);
	ST7796_Write_Data(0x06);
	ST7796_Write_Data(0x04);
	ST7796_Write_Data(0x2E);
	ST7796_Write_Data(0x44);
	ST7796_Write_Data(0x45);
	ST7796_Write_Data(0x0B);
	ST7796_Write_Data(0x17);
	ST7796_Write_Data(0x16);
	ST7796_Write_Data(0x18);
	ST7796_Write_Data(0x1B);

  ST7796_Write_Command(0xF0);		//Command Set Control
  ST7796_Write_Data(0x3C);			//D[7:0] = 3Ch disable command 2 part I

  ST7796_Write_Command(0xF0);		//Command Set Control
  ST7796_Write_Data(0x69);			//D[7:0] = 69h disable command 2 part II

  HAL_Delay(120);
  ST7796_Write_Command(0x29);		// Display ON
}

void BSP_LCD_Direction(uint8_t dir)
{
	Direction = dir;

  ST7796_Write_Command(0x36);		//Memory Data Access Control

  switch(Direction)
	{
		case 0:
			LCD_WIDTH = 320;
			LCD_HEIGHT = 480;
		  ST7796_Write_Data((1<<7) | (1<<3));	//0 degree MY=1, MX=0, MV=0, ML=0, BGR=1, MH=0

		  ST7796_Write_Command(0x2A);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 319
			ST7796_Write_Data(0x3F);
		  ST7796_Write_Command(0x2B);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 479
			ST7796_Write_Data(0xDF);
			break;

		case 1:
			LCD_WIDTH = 480;
			LCD_HEIGHT = 320;
		  ST7796_Write_Data((1<<7)|(1<<6)|(1<<5)|(1<<3));//90 degree MY=1, MX=1, MV=1, ML=0, BGR=1, MH=0

		  ST7796_Write_Command(0x2A);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 479
			ST7796_Write_Data(0xDF);
		  ST7796_Write_Command(0x2B);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 319
			ST7796_Write_Data(0x3F);
			break;

		case 2:
			LCD_WIDTH = 320;
			LCD_HEIGHT = 480;
		  ST7796_Write_Data((1<<6)|(1<<3));//180 degree MY=0, MX=1, MV=0, ML=0, BGR=1, MH=0

		  ST7796_Write_Command(0x2A);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 319
			ST7796_Write_Data(0x3F);
		  ST7796_Write_Command(0x2B);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 479
			ST7796_Write_Data(0xDF);
			break;

		case 3:
			LCD_WIDTH = 480;
			LCD_HEIGHT = 320;
		  ST7796_Write_Data((1<<5)|(1<<3));//270 degree MY=0, MX=0, MV=1, ML=0, BGR=1, MH=0
		  ST7796_Write_Command(0x2A);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 479
			ST7796_Write_Data(0xDF);
		  ST7796_Write_Command(0x2B);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x00);
			ST7796_Write_Data(0x01);		// 319
			ST7796_Write_Data(0x3F);
			break;
	}
}

uint8_t BSP_LCD_GetDirection(void)
{
	return Direction;
}


/**
  * @brief  Fills buffer.
  * @param  LayerIndex: layer index
  * @param  pDst: output color
  * @param  xSize: buffer width
  * @param  ySize: buffer height
  * @param  OffLine: offset
  * @param  ColorIndex: color Index
  */
static void FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
  /* Register to memory mode with ARGB8888 as color Mode */
  hdma2d.Init.Mode         = DMA2D_R2M;
  if(LtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
  { /* RGB565 format */
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_RGB565;
		ColorIndex = ((ColorIndex & RED)<<8) | ((ColorIndex & GREEN )<<5) | ((ColorIndex & BLUE) << 3);
  }
  else
  { /* ARGB8888 format */
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_RGB888;
  }
  hdma2d.Init.OutputOffset = OffLine;

  hdma2d.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, LayerIndex) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d, 10);
      }
    }
  }
}

__inline uint16_t BSP_LCD_GetXSize(void)
{
	return LCD_WIDTH;
}

__inline uint16_t BSP_LCD_GetYSize(void)
{
	return LCD_HEIGHT;
}

/**
  * @brief  Writes Pixel.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  RGB_Code: the pixel color in ARGB mode (8-8-8-8)
  */
void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
	uint16_t Width = BSP_LCD_GetXSize();
	uint16_t Height = BSP_LCD_GetYSize();

	if((Xpos >= Width) || (Ypos >= Height)) return;

  /* Write data value to all SDRAM memory */
  //*(__IO uint32_t*) (LtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*BSP_LCD_GetXSize() + Xpos))) = RGB_Code;		// 32bit color
	*(__IO uint16_t*) (LtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*Width + Xpos))) = RGB_Code;			// 16bit color
}

/**
  * @brief  Reads Pixel.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @retval RGB pixel color
  */
uint32_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  uint32_t ret = 0;

  if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    /* Read data value from SDRAM memory */
    ret = *(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*BSP_LCD_GetXSize() + Xpos)));
  }
  else if(hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    /* Read data value from SDRAM memory */
    ret = (*(__IO uint32_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*BSP_LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
  }
  else if((hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
          (hltdc.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))
  {
    /* Read data value from SDRAM memory */
    ret = *(__IO uint16_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*BSP_LCD_GetXSize() + Xpos)));
  }
  else
  {
    /* Read data value from SDRAM memory */
    ret = *(__IO uint8_t*) (hltdc.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*BSP_LCD_GetXSize() + Xpos)));
  }

  return ret;
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background
  */
void BSP_LCD_Clear(uint32_t Color)
{
  /* Clear the LCD */
  FillBuffer(ActiveLayer, (uint32_t *)(hltdc.LayerCfg[ActiveLayer].FBStartAdress), BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), 0, Color);
}

/**
  * @brief  Sets the Display window.
  * @param  LayerIndex: layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  */
void BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* reconfigure the layer size */
  HAL_LTDC_SetWindowSize(&LtdcHandler, Width, Height, LayerIndex);

  /* reconfigure the layer position */
  HAL_LTDC_SetWindowPosition(&LtdcHandler, Xpos, Ypos, LayerIndex);
}

/**
  * @brief  Displays an horizontal line.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Length: line length
  */
void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color)
{
  uint32_t xaddress = 0;

  if(Length == 0) return;

  /* Get the line address */
  xaddress = (LtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2 * (BSP_LCD_GetXSize()*Ypos + Xpos);

  /* Write line */
  FillBuffer(ActiveLayer, (uint32_t *)xaddress, Length, 1, 0, Color);
}

/**
  * @brief  Displays a vertical line.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Length: line length
  */
void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t Color)
{
  uint32_t xaddress = 0;

  if(Length == 0) return;

  /* Get the line address */
  xaddress = (LtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2 * (BSP_LCD_GetXSize()*Ypos + Xpos);

  /* Write line */
  FillBuffer(ActiveLayer, (uint32_t *)xaddress, 1, Length, (BSP_LCD_GetXSize() - 1), Color);
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  X1: the point 1 X position
  * @param  Y1: the point 1 Y position
  * @param  X2: the point 2 X position
  * @param  Y2: the point 2 Y position
  */
void BSP_LCD_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = ABS(X2 - X1);        /* The difference between the x's */
  deltay = ABS(Y2 - Y1);        /* The difference between the y's */
  x = X1;                       /* Start x off at the first pixel */
  y = Y1;                       /* Start y off at the first pixel */

  if (X2 >= X1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (Y2 >= Y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawPixel(x, y, Color);   /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Height: display rectangle height
  * @param  Width: display rectangle width
  */
void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color)
{
  if((Width == 0) || (Height == 0)) return;

  /* Draw horizontal lines */
  BSP_LCD_DrawHLine(Xpos, Ypos, Width,Color);
  BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width, Color);

  /* Draw vertical lines */
  BSP_LCD_DrawVLine(Xpos, Ypos, Height, Color);
  BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height, Color);
}

/**
  * @brief  Displays a circle.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Radius: the circle radius
  */
#if 0
void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t Color)
{
  int32_t  d;/* Decision Variable */
  uint32_t  curx;/* Current X Value */
  uint32_t  cury;/* Current Y Value */

	if(Radius == 0) return;

  d = 3 - (Radius << 1);
  curx = 0;
  cury = Radius;

  while (curx <= cury)
  {
    BSP_LCD_DrawPixel((Xpos + curx), (Ypos - cury), Color);
    BSP_LCD_DrawPixel((Xpos - curx), (Ypos - cury), Color);
    BSP_LCD_DrawPixel((Xpos + cury), (Ypos - curx), Color);
    BSP_LCD_DrawPixel((Xpos - cury), (Ypos - curx), Color);
    BSP_LCD_DrawPixel((Xpos + curx), (Ypos + cury), Color);
    BSP_LCD_DrawPixel((Xpos - curx), (Ypos + cury), Color);
    BSP_LCD_DrawPixel((Xpos + cury), (Ypos + curx), Color);
    BSP_LCD_DrawPixel((Xpos - cury), (Ypos + curx), Color);

    if (d < 0)
    {
      d += (curx << 2) + 6;
    }
    else
    {
      d += ((curx - cury) << 2) + 10;
      cury--;
    }
    curx++;
  }
}
#else
void BSP_LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radian, uint16_t color)
{
  int a, b;
  int di;

  a = 0;
  b = radian;
  di = 3 - (radian << 1);

  while(a <= b)
  {
  	BSP_LCD_DrawPixel(x-b,y-a,color); //3
  	BSP_LCD_DrawPixel(x+b,y-a,color); //0
  	BSP_LCD_DrawPixel(x-a,y+b,color); //1
  	BSP_LCD_DrawPixel(x-b,y-a,color); //7
  	BSP_LCD_DrawPixel(x-a,y-b,color); //2
  	BSP_LCD_DrawPixel(x+b,y+a,color); //4
  	BSP_LCD_DrawPixel(x+a,y-b,color); //5
  	BSP_LCD_DrawPixel(x+a,y+b,color); //6
  	BSP_LCD_DrawPixel(x-b,y+a,color);
    a++;
    //Bresenham
    if(di<0)
    {
      di +=4*a+6;
    }
    else
    {
      di+=10+4*(a-b);
      b--;
    }
    BSP_LCD_DrawPixel(x+a,y+b,color);
  }
}
#endif

/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array
  * @param  PointCount: Number of points
  */
void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount, uint16_t Color)
{
  int16_t x = 0, y = 0;

  if(PointCount < 2)
  {
    return;
  }

  BSP_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y, Color);

  while(--PointCount)
  {
    x = Points->X;
    y = Points->Y;
    Points++;
    BSP_LCD_DrawLine(x, y, Points->X, Points->Y, Color);
  }
}

/**
  * @brief  Displays an Ellipse.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  XRadius: the X radius of ellipse
  * @param  YRadius: the Y radius of ellipse
  */
void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint16_t Color)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float k = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;

  k = (float)(rad2/rad1);

  do {
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/k)), (Ypos+y), Color);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/k)), (Ypos+y), Color);
    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/k)), (Ypos-y), Color);
    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/k)), (Ypos-y), Color);

    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Height: rectangle height
  * @param  Width: rectangle width
  */
void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color)
{
	uint16_t W = BSP_LCD_GetXSize();
	uint16_t H = BSP_LCD_GetYSize();
  uint32_t xaddress = 0;

  if((Width == 0) || (Height == 0)) return;

  if(Xpos + Width >= W) Width = W - Xpos;
  if(Ypos + Height >= H) Height = H - Ypos;

  /* Get the rectangle start address */
  xaddress = (LtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2 * (W * Ypos + Xpos);

  /* Fill the rectangle */
  FillBuffer(ActiveLayer, (uint32_t *)xaddress, Width, Height, (W - Width), Color);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Radius: the circle radius
  */
void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t Color)
{
  int32_t  d;    /* Decision Variable */
  uint32_t  curx;/* Current X Value */
  uint32_t  cury;/* Current Y Value */

  d = 3 - (Radius << 1);

  curx = 0;
  cury = Radius;

  while (curx <= cury)
  {
    if(cury > 0)
    {
      BSP_LCD_DrawHLine(Xpos - cury, Ypos + curx, 2*cury, Color);
      BSP_LCD_DrawHLine(Xpos - cury, Ypos - curx, 2*cury, Color);
    }

    if(curx > 0)
    {
      BSP_LCD_DrawHLine(Xpos - curx, Ypos - cury, 2*curx, Color);
      BSP_LCD_DrawHLine(Xpos - curx, Ypos + cury, 2*curx, Color);
    }
    if (d < 0)
    {
      d += (curx << 2) + 6;
    }
    else
    {
      d += ((curx - cury) << 2) + 10;
      cury--;
    }
    curx++;
  }

  BSP_LCD_DrawCircle(Xpos, Ypos, Radius, Color);
}

/**
  * @brief  Fill triangle.
  * @param  X1: the point 1 x position
  * @param  Y1: the point 1 y position
  * @param  X2: the point 2 x position
  * @param  Y2: the point 2 y position
  * @param  X3: the point 3 x position
  * @param  Y3: the point 3 y position
  */
void BSP_LCD_FillTriangle(uint16_t X1, uint16_t X2, uint16_t X3, uint16_t Y1, uint16_t Y2, uint16_t Y3, uint16_t Color)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;

  deltax = ABS(X2 - X1);        /* The difference between the x's */
  deltay = ABS(Y2 - Y1);        /* The difference between the y's */
  x = X1;                       /* Start x off at the first pixel */
  y = Y1;                       /* Start y off at the first pixel */

  if (X2 >= X1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (Y2 >= Y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    BSP_LCD_DrawLine(x, y, X3, Y3, Color);

    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays a full poly-line (between many points).
  * @param  Points: pointer to the points array
  * @param  PointCount: Number of points
  */
void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount, uint16_t Color)
{

  int16_t x = 0, y = 0, x2 = 0, y2 = 0, xcenter = 0, ycenter = 0, xfirst = 0, yfirst = 0, pixelx = 0, pixely = 0, counter = 0;
  uint16_t  imageleft = 0, imageright = 0, imagetop = 0, imagebottom = 0;

  imageleft = imageright = Points->X;
  imagetop= imagebottom = Points->Y;

  for(counter = 1; counter < PointCount; counter++)
  {
    pixelx = POLY_X(counter);
    if(pixelx < imageleft)
    {
      imageleft = pixelx;
    }
    if(pixelx > imageright)
    {
      imageright = pixelx;
    }

    pixely = POLY_Y(counter);
    if(pixely < imagetop)
    {
      imagetop = pixely;
    }
    if(pixely > imagebottom)
    {
      imagebottom = pixely;
    }
  }

  if(PointCount < 2)
  {
    return;
  }

  xcenter = (imageleft + imageright)/2;
  ycenter = (imagebottom + imagetop)/2;

  xfirst = Points->X;
  yfirst = Points->Y;

  while(--PointCount)
  {
    x = Points->X;
    y = Points->Y;
    Points++;
    x2 = Points->X;
    y2 = Points->Y;

    BSP_LCD_FillTriangle(x, x2, xcenter, y, y2, ycenter, Color);
    BSP_LCD_FillTriangle(x, xcenter, x2, y, ycenter, y2, Color);
    BSP_LCD_FillTriangle(xcenter, x2, x, ycenter, y2, y, Color);
  }

  BSP_LCD_FillTriangle(xfirst, x2, xcenter, yfirst, y2, ycenter, Color);
  BSP_LCD_FillTriangle(xfirst, xcenter, x2, yfirst, ycenter, y2, Color);
  BSP_LCD_FillTriangle(xcenter, x2, xfirst, ycenter, y2, yfirst, Color);
}

/**
  * @brief  Draw a full ellipse.
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  XRadius: X radius of ellipse
  * @param  YRadius: Y radius of ellipse.
  */
void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius, uint16_t Color)
{
  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
  float K = 0, rad1 = 0, rad2 = 0;

  rad1 = XRadius;
  rad2 = YRadius;
  K = (float)(rad2/rad1);

  do
  {
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), Color);
    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), Color);

    e2 = err;
    if (e2 <= x)
    {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;
  }
  while (y <= 0);
}


void BSP_LCD_SetBackLight(uint16_t brightness)	// 0 <= brightness < 100
{
	BACKLIGHT_TIMER.Instance->CCR1 = brightness;
}
