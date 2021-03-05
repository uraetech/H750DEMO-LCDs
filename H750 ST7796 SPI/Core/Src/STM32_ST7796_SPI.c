/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "STM32_ST7796_SPI.h"
#include "tim.h"

/* Global Variables ------------------------------------------------------------------*/
volatile uint16_t LCD_HEIGHT = 320;
volatile uint16_t LCD_WIDTH	 = 480;
uint8_t screen_rotation;

#define BACKLIGHT_TIMER			htim15

/*Send data (char) to LCD*/
void ST7796_SPI_Send(unsigned char SPI_Data)
{
	HAL_SPI_Transmit(HSPI_INSTANCE, &SPI_Data, 1, 1);
}

uint16_t ST7796_GetWidth(void)
{
	return LCD_WIDTH;
}

uint16_t ST7796_GetHeight(void)
{
	return LCD_HEIGHT;
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

/* Set Address - Location block - to draw into */
void ST7796_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	ST7796_Write_Command(0x2A);
	ST7796_Write_Data(X1>>8);
	ST7796_Write_Data(X1);
	ST7796_Write_Data(X2>>8);
	ST7796_Write_Data(X2);

	ST7796_Write_Command(0x2B);
	ST7796_Write_Data(Y1>>8);
	ST7796_Write_Data(Y1);
	ST7796_Write_Data(Y2>>8);
	ST7796_Write_Data(Y2);

	ST7796_Write_Command(0x2C);
}

/*HARDWARE RESET*/
void ST7796_Reset(void)
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
}

/*Ser rotation of the screen - changes x0 and y0*/
void ST7796_Set_Rotation(uint8_t Rotation)
{
	screen_rotation = Rotation;
	
	ST7796_Write_Command(0x36);
	HAL_Delay(1);

	switch(screen_rotation)
	{
		case SCREEN_VERTICAL_1:
			ST7796_Write_Data(0x40|0x08);
			LCD_WIDTH = LCD_HEIGHT;
			LCD_HEIGHT = LCD_WIDTH;
			break;
		case SCREEN_HORIZONTAL_1:
			ST7796_Write_Data(0x20|0x08);
			LCD_WIDTH  = LCD_WIDTH;
			LCD_HEIGHT = LCD_HEIGHT;
			break;
		case SCREEN_VERTICAL_2:
			ST7796_Write_Data(0x80|0x08);
			LCD_WIDTH  = LCD_HEIGHT;
			LCD_HEIGHT = LCD_WIDTH;
			break;
		case SCREEN_HORIZONTAL_2:
			ST7796_Write_Data(0x40|0x80|0x20|0x08);
			LCD_WIDTH  = LCD_WIDTH;
			LCD_HEIGHT = LCD_HEIGHT;
			break;
		default:
			//EXIT IF SCREEN ROTATION NOT VALID!
			break;
	}
}

/*Enable LCD display*/
void ST7796_Enable(void)
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
}

/*Initialize LCD display*/
void ST7796_Init(void)
{
	ST7796_Reset();

  ST7796_Write_Command(0x01);		//Software Reset
	HAL_Delay(120);

  ST7796_Write_Command(0x11);		// Sleep Out
	HAL_Delay(120);

  ST7796_Write_Command(0x36);		//Memory Data Access Control
  ST7796_Write_Data(0x48);			//MY MX MV ML RGB MH

  ST7796_Write_Command(0x3A);		//Interface Pixel Format
  ST7796_Write_Data(0x55);			//¡®101¡¯ = 16bit/pixel

  ST7796_Write_Command(0xB0);		//Interface Mode Control
  ST7796_Write_Data(0x80);			//SPI_EN = 1, 3/4 wire serial interface

  ST7796_Write_Command(0xB5);
  ST7796_Write_Data(8);					//VFP: Front porch of vertical lines =4
  ST7796_Write_Data(4);					//VBP: Back porch of vertical lines =2
  ST7796_Write_Data(0);
  ST7796_Write_Data(20);				//HBP: Back porch of horizontal dotclk=10

  ST7796_Write_Command(0xB6);		//Display Function Control
  ST7796_Write_Data(0x20);	// A0		//D5=1:RGB,D5=0:System Interface D6=1:SYNC Mode   E0
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

//	ST7796_Set_Rotation(SCREEN_VERTICAL_1);
 	ST7796_Set_Rotation(SCREEN_HORIZONTAL_1);
}

//INTERNAL FUNCTION OF LIBRARY, USAGE NOT RECOMENDED, USE Draw_Pixel INSTEAD
/*Sends single pixel colour information to LCD*/
void ST7796_Draw_Colour(uint16_t Colour)
{
//SENDS COLOUR
	unsigned char TempBuffer[2] = {Colour>>8, Colour};
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(HSPI_INSTANCE, TempBuffer, 2, 1);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

//INTERNAL FUNCTION OF LIBRARY
/*Sends block colour information to LCD*/
void ST7796_Draw_Colour_Burst(uint16_t Colour, uint32_t Size)
{
//SENDS COLOUR
	uint32_t Buffer_Size = 0;
	if((Size*2) < BURST_MAX_SIZE)
	{
		Buffer_Size = Size;
	}
	else
	{
		Buffer_Size = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

	unsigned char chifted = 	Colour>>8;;
	unsigned char burst_buffer[Buffer_Size];
	for(uint32_t j = 0; j < Buffer_Size; j+=2)
		{
			burst_buffer[j] = 	chifted;
			burst_buffer[j+1] = Colour;
		}
	
	uint32_t Sending_Size = Size*2;
	uint32_t Sending_in_Block = Sending_Size/Buffer_Size;
	uint32_t Remainder_from_block = Sending_Size%Buffer_Size;

	if(Sending_in_Block != 0)
	{
		for(uint32_t j = 0; j < (Sending_in_Block); j++)
		{
			HAL_SPI_Transmit(HSPI_INSTANCE, (unsigned char *)burst_buffer, Buffer_Size, 10);
		}
	}

	//REMAINDER!
	HAL_SPI_Transmit(HSPI_INSTANCE, (unsigned char *)burst_buffer, Remainder_from_block, 10);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

//FILL THE ENTIRE SCREEN WITH SELECTED COLOUR (either #define-d ones or custom 16bit)
/*Sets address (entire screen) and Sends Height*Width ammount of colour information to LCD*/
void ST7796_Fill_Screen(uint16_t Colour)
{
	ST7796_Set_Address(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
	ST7796_Draw_Colour_Burst(Colour, LCD_WIDTH*LCD_HEIGHT);
}

//DRAW PIXEL AT XY POSITION WITH SELECTED COLOUR
//
//Location is dependant on screen orientation. x0 and y0 locations change with orientations.
//Using pixels to draw big simple structures is not recommended as it is really slow
//Try using either rectangles or lines if possible
//
void ST7796_Draw_Pixel(uint16_t X,uint16_t Y,uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;	//OUT OF BOUNDS!

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ST7796_SPI_Send(0x2A);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//XDATA
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer[4] = {X>>8,X, (X+1)>>8, (X+1)};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer, 4, 1);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ST7796_SPI_Send(0x2B);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//YDATA
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer1[4] = {Y>>8,Y, (Y+1)>>8, (Y+1)};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer1, 4, 1);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
	
	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ST7796_SPI_Send(0x2C);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
	
	//COLOUR
	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer2[2] = {Colour>>8, Colour};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer2, 2, 1);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

//DRAW RECTANGLE OF SET SIZE AND HEIGTH AT X and Y POSITION WITH CUSTOM COLOUR
//
//Rectangle is hollow. X and Y positions mark the upper left corner of rectangle
//As with all other draw calls x0 and y0 locations dependant on screen orientation
//

void ST7796_Draw_FillRectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
	if(Width == 0 || Height == 0) return;

	if((X+Width-1)>=LCD_WIDTH)
	{
			Width=LCD_WIDTH-X;
	}
	if((Y+Height-1)>=LCD_HEIGHT)
	{
			Height=LCD_HEIGHT-Y;
	}
	ST7796_Set_Address(X, Y, X+Width-1, Y+Height-1);
	ST7796_Draw_Colour_Burst(Colour, Height*Width);
}

//DRAW LINE FROM X,Y LOCATION to X+Width,Y LOCATION
void ST7796_Draw_Horizontal_Line(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
	if(Width == 0) return;

	if((X+Width-1)>=LCD_WIDTH)
	{
			Width=LCD_WIDTH-X;
	}
	ST7796_Set_Address(X, Y, X+Width-1, Y);
	ST7796_Draw_Colour_Burst(Colour, Width);
}

//DRAW LINE FROM X,Y LOCATION to X,Y+Height LOCATION
void ST7796_Draw_Vertical_Line(uint16_t X, uint16_t Y, uint16_t Height, uint16_t Colour)
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;
	if(Height == 0) return;

	if((Y+Height-1)>=LCD_HEIGHT)
	{
			Height=LCD_HEIGHT-Y;
	}
	ST7796_Set_Address(X, Y, X, Y+Height-1);
	ST7796_Draw_Colour_Burst(Colour, Height);
}

void ST7796_Draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;

  delta_x = x2 - x1;
  delta_y = y2 - y1;
  uRow = x1;
  uCol = y1;

  if(delta_x > 0) incx = 1;
  else if(delta_x == 0) incx = 0;
  else
  {
    incx = -1;
    delta_x = -delta_x;
  }

  if(delta_y > 0) incy = 1;
  else if(delta_y == 0) incy=0;
  else
  {
    incy = -1; delta_y = -delta_y;
  }

  if( delta_x > delta_y) distance = delta_x;
  else distance = delta_y;

  for(t = 0; t <= distance + 1; t++ )
  {
  	ST7796_Draw_Pixel(uRow, uCol, color);
    xerr += delta_x ;
    yerr += delta_y ;
    if(xerr > distance)
    {
      xerr -= distance;
      uRow += incx;
    }
    if(yerr > distance)
    {
      yerr -= distance;
      uCol += incy;
    }
  }
}

void ST7796_Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	ST7796_Draw_line(x1,y1,x1,y2,color);
	ST7796_Draw_line(x1,y2,x2,y2,color);
	ST7796_Draw_line(x2,y2,x2,y1,color);
	ST7796_Draw_line(x2,y1,x1,y1,color);
}

void ST7796_Draw_Circle(uint16_t x, uint16_t y, uint16_t radian, uint16_t color)
{
  int a, b;
  int di;

  a = 0;
  b = radian;
  di = 3 - (radian << 1);

  while(a <= b)
  {
    ST7796_Draw_Pixel(x-b,y-a,color); //3
    ST7796_Draw_Pixel(x+b,y-a,color); //0
    ST7796_Draw_Pixel(x-a,y+b,color); //1
    ST7796_Draw_Pixel(x-b,y-a,color); //7
    ST7796_Draw_Pixel(x-a,y-b,color); //2
    ST7796_Draw_Pixel(x+b,y+a,color); //4
    ST7796_Draw_Pixel(x+a,y-b,color); //5
    ST7796_Draw_Pixel(x+a,y+b,color); //6
    ST7796_Draw_Pixel(x-b,y+a,color);
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
    ST7796_Draw_Pixel(x+a,y+b,color);
  }
}

void ST7796_Scroll(uint16_t position)
{
	ST7796_Write_Command(0x37);
	ST7796_Write_Data(position >> 8);
	ST7796_Write_Data(position & 0xFF);
}

void BSP_LCD_SetBackLight(uint16_t brightness)	// 0 <= brightness < 100
{
	BACKLIGHT_TIMER.Instance->CCR1 = brightness;
}

uint8_t ST7796_GetDirection(void)
{
	return screen_rotation;
}
