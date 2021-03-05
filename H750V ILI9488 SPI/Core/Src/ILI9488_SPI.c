/**
  * @brief  ILI9488 Registers
  */

/* Level 1 Commands */
#define LCD_SWRESET             0x01   /* Software Reset */
#define LCD_READ_DISPLAY_ID     0x04   /* Read display identification information */
#define LCD_RDDST               0x09   /* Read Display Status */
#define LCD_RDDPM               0x0A   /* Read Display Power Mode */
#define LCD_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define LCD_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define LCD_RDDIM               0x0D   /* Read Display Image Format */
#define LCD_RDDSM               0x0E   /* Read Display Signal Mode */
#define LCD_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define LCD_SPLIN               0x10   /* Enter Sleep Mode */
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_PTLON               0x12   /* Partial Mode ON */
#define LCD_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define LCD_DINVOFF             0x20   /* Display Inversion OFF */
#define LCD_DINVON              0x21   /* Display Inversion ON */
#define LCD_GAMMA               0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Colomn address register */
#define LCD_PAGE_ADDR           0x2B   /* Page address register */
#define LCD_GRAM                0x2C   /* GRAM register */
#define LCD_RGBSET              0x2D   /* Color SET */
#define LCD_RAMRD               0x2E   /* Memory Read */
#define LCD_PLTAR               0x30   /* Partial Area */
#define LCD_VSCRDEF             0x33   /* Vertical Scrolling Definition */
#define LCD_TEOFF               0x34   /* Tearing Effect Line OFF */
#define LCD_TEON                0x35   /* Tearing Effect Line ON */
#define LCD_MAC                 0x36   /* Memory Access Control register*/
#define LCD_VSCRSADD            0x37   /* Vertical Scrolling Start Address */
#define LCD_IDMOFF              0x38   /* Idle Mode OFF */
#define LCD_IDMON               0x39   /* Idle Mode ON */
#define LCD_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define LCD_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */
#define LCD_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */
#define LCD_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */
#define LCD_GET_SCANLINE        0x45   /* Get Scanline */
#define LCD_WDB                 0x51   /* Write Brightness Display register */
#define LCD_RDDISBV             0x52   /* Read Display Brightness */
#define LCD_WCD                 0x53   /* Write Control Display register*/
#define LCD_RDCTRLD             0x54   /* Read CTRL Display */
#define LCD_WRCABC              0x55   /* Write Content Adaptive Brightness Control */
#define LCD_RDCABC              0x56   /* Read Content Adaptive Brightness Control */
#define LCD_WRITE_CABC          0x5E   /* Write CABC Minimum Brightness */
#define LCD_READ_CABC           0x5F   /* Read CABC Minimum Brightness */
#define LCD_READ_ID1            0xDA   /* Read ID1 */
#define LCD_READ_ID2            0xDB   /* Read ID2 */
#define LCD_READ_ID3            0xDC   /* Read ID3 */

/* Level 2 Commands */
#define LCD_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define LCD_FRMCTR1             0xB1   /* Frame Rate Control (In Normal Mode) */
#define LCD_FRMCTR2             0xB2   /* Frame Rate Control (In Idle Mode) */
#define LCD_FRMCTR3             0xB3   /* Frame Rate Control (In Partial Mode) */
#define LCD_INVTR               0xB4   /* Display Inversion Control */
#define LCD_BPC                 0xB5   /* Blanking Porch Control register */
#define LCD_DFC                 0xB6   /* Display Function Control register */
#define LCD_ETMOD               0xB7   /* Entry Mode Set */
#define LCD_BACKLIGHT1          0xB8   /* Backlight Control 1 */
#define LCD_BACKLIGHT2          0xB9   /* Backlight Control 2 */
#define LCD_BACKLIGHT3          0xBA   /* Backlight Control 3 */
#define LCD_BACKLIGHT4          0xBB   /* Backlight Control 4 */
#define LCD_BACKLIGHT5          0xBC   /* Backlight Control 5 */
#define LCD_BACKLIGHT7          0xBE   /* Backlight Control 7 */
#define LCD_BACKLIGHT8          0xBF   /* Backlight Control 8 */
#define LCD_POWER1              0xC0   /* Power Control 1 register */
#define LCD_POWER2              0xC1   /* Power Control 2 register */
#define LCD_VCOM1               0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2               0xC7   /* VCOM Control 2 register */
#define LCD_NVMWR               0xD0   /* NV Memory Write */
#define LCD_NVMPKEY             0xD1   /* NV Memory Protection Key */
#define LCD_RDNVM               0xD2   /* NV Memory Status Read */
#define LCD_READ_ID4            0xD3   /* Read ID4 */
#define LCD_PGAMMA              0xE0   /* Positive Gamma Correction register */
#define LCD_NGAMMA              0xE1   /* Negative Gamma Correction register */
#define LCD_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define LCD_DGAMCTRL2           0xE3   /* Digital Gamma Control 2 */
#define LCD_INTERFACE           0xF6   /* Interface control register */

/* Extend register commands */
#define LCD_POWERA               0xCB   /* Power control A register */
#define LCD_POWERB               0xCF   /* Power control B register */
#define LCD_DTCA                 0xE8   /* Driver timing control A */
#define LCD_DTCB                 0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ            0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN            0xF2   /* 3 Gamma enable register */
#define LCD_PRC                  0xF7   /* Pump ratio control register */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "ILI9488_SPI.h"
#include "LCD_font.h"

#define ABS(x)   ((x) > 0 ? (x) : -(x))


/* Global Variables ------------------------------------------------------------------*/
volatile uint16_t LCD_HEIGHT = ILI9488_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = ILI9488_SCREEN_WIDTH;
uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
static uint8_t Direction;

uint8_t ILI9488_GetDirection(void)
{
	return Direction;
}

/*Send data (char) to LCD*/
void ILI9488_SPI_Send(unsigned char SPI_Data)
{
	HAL_SPI_Transmit(HSPI_INSTANCE, &SPI_Data, 1, 1);
}

/* Send command (char) to LCD */
void ILI9488_Write_Command(uint8_t Command)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	ILI9488_SPI_Send(Command);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

/* Send Data (char) to LCD */
void ILI9488_Write_Data(uint8_t Data)
{
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ILI9488_SPI_Send(Data);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

/* Set Address - Location block - to draw into */
void ILI9488_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	ILI9488_Write_Command(0x2A);
	ILI9488_Write_Data(X1 >> 8);
	ILI9488_Write_Data(X1);
	ILI9488_Write_Data(X2 >> 8);
	ILI9488_Write_Data(X2);

	ILI9488_Write_Command(0x2B);
	ILI9488_Write_Data(Y1 >> 8);
	ILI9488_Write_Data(Y1);
	ILI9488_Write_Data(Y2 >> 8);
	ILI9488_Write_Data(Y2);

	ILI9488_Write_Command(0x2C);
}

/*HARDWARE RESET*/
void ILI9488_Reset(void)
{
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

/*Ser rotation of the screen - changes x0 and y0*/
void ILI9488_Set_Rotation(uint8_t Rotation)
{
	Direction = Rotation;

	ILI9488_Write_Command(0x36);
	HAL_Delay(1);

	switch(Direction)
	{
		case SCREEN_VERTICAL_1:
			ILI9488_Write_Data((1<<3));
			LCD_WIDTH = 320;
			LCD_HEIGHT = 480;
			break;
		case SCREEN_HORIZONTAL_1:
			ILI9488_Write_Data((1<<6)|(1<<5)|(1<<4)|(1<<3));
			LCD_WIDTH  = 480;
			LCD_HEIGHT = 320;
			break;
		case SCREEN_VERTICAL_2:
			ILI9488_Write_Data((1<<7)|(1<<6)|(1<<3));
			LCD_WIDTH  = 320;
			LCD_HEIGHT = 480;
			break;
		case SCREEN_HORIZONTAL_2:
			ILI9488_Write_Data((1<<7)|(1<<5)|(1<<3));
			LCD_WIDTH  = 480;
			LCD_HEIGHT = 320;
			break;
		default:
			//EXIT IF SCREEN ROTATION NOT VALID!
			break;
	}
}


uint16_t ILI9488_GetWidth(void)
{
	return LCD_WIDTH;
}

uint16_t ILI9488_GetHeight(void)
{
	return LCD_HEIGHT;
}


/*Initialize LCD display*/
void ILI9488_Init(void)
{
	ILI9488_Reset();

	//SOFTWARE RESET
	ILI9488_Write_Command(0x01);
	HAL_Delay(100);

	ILI9488_Write_Command(0xF7);
	ILI9488_Write_Data(0xA9);
	ILI9488_Write_Data(0x51);
	ILI9488_Write_Data(0x2C);
	ILI9488_Write_Data(0x82);

	ILI9488_Write_Command(0xC0);
	ILI9488_Write_Data(0x11);
	ILI9488_Write_Data(0x09);

	ILI9488_Write_Command(0xC1);
	ILI9488_Write_Data(0x41);

	ILI9488_Write_Command(0xC5);
	ILI9488_Write_Data(0x00);
	ILI9488_Write_Data(0x0A);
	ILI9488_Write_Data(0x80);

	ILI9488_Write_Command(0xB1);
	ILI9488_Write_Data(0xB0);
	ILI9488_Write_Data(0x11);

	ILI9488_Write_Command(0xB4);
	ILI9488_Write_Data(0x02);

	ILI9488_Write_Command(0xB6);
	ILI9488_Write_Data(0x02);
	ILI9488_Write_Data(0x42);

	ILI9488_Write_Command(0xB7);
	ILI9488_Write_Data(0xC6);

	ILI9488_Write_Command(0xBE);
	ILI9488_Write_Data(0x00);
	ILI9488_Write_Data(0x04);

	ILI9488_Write_Command(0xE9);
	ILI9488_Write_Data(0x00);

	ILI9488_Write_Command(0x36);
	ILI9488_Write_Data((1<<3)|(0<<7)|(1<<6)|(1<<5));

	ILI9488_Write_Command(0x3A);
	ILI9488_Write_Data(0x66);

	//POSITIVE GAMMA CORRECTION
	ILI9488_Write_Command(0xE0);
	ILI9488_Write_Data(0x00);
	ILI9488_Write_Data(0x07);
	ILI9488_Write_Data(0x0f);
	ILI9488_Write_Data(0x0D);
	ILI9488_Write_Data(0x1B);
	ILI9488_Write_Data(0x0A);
	ILI9488_Write_Data(0x3c);
	ILI9488_Write_Data(0x78);
	ILI9488_Write_Data(0x4A);
	ILI9488_Write_Data(0x07);
	ILI9488_Write_Data(0x0E);
	ILI9488_Write_Data(0x09);
	ILI9488_Write_Data(0x1B);
	ILI9488_Write_Data(0x1e);
	ILI9488_Write_Data(0x0f);

	//NEGATIVE GAMMA CORRECTION
	ILI9488_Write_Command(0xE1);
	ILI9488_Write_Data(0x00);
	ILI9488_Write_Data(0x22);
	ILI9488_Write_Data(0x24);
	ILI9488_Write_Data(0x06);
	ILI9488_Write_Data(0x12);
	ILI9488_Write_Data(0x07);
	ILI9488_Write_Data(0x36);
	ILI9488_Write_Data(0x47);
	ILI9488_Write_Data(0x47);
	ILI9488_Write_Data(0x06);
	ILI9488_Write_Data(0x0a);
	ILI9488_Write_Data(0x07);
	ILI9488_Write_Data(0x30);
	ILI9488_Write_Data(0x37);
	ILI9488_Write_Data(0x0f);

	//EXIT SLEEP
	ILI9488_Write_Command(0x11);
	HAL_Delay(120);

	//TURN ON DISPLAY
	ILI9488_Write_Command(0x29);

	//STARTING ROTATION
	ILI9488_Set_Rotation(SCREEN_HORIZONTAL_1);
	//ILI9488_Set_Rotation(SCREEN_VERTICAL_1);
}

//INTERNAL FUNCTION OF LIBRARY, USAGE NOT RECOMENDED, USE Draw_Pixel INSTEAD
/*Sends single pixel colour information to LCD*/
void ILI9488_Draw_Colour(uint16_t Colour)
{
//SENDS COLOUR
	unsigned char TempBuffer[3] = {(Colour>>8)&0xF8, (Colour>>3)&0xFC, Colour<<3};

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(HSPI_INSTANCE, TempBuffer, 3, 1);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

//INTERNAL FUNCTION OF LIBRARY
/*Sends block colour information to LCD*/
void ILI9488_Draw_Colour_Burst(uint16_t Colour, uint32_t Size)
{
//SENDS COLOUR
	uint32_t Buffer_Size = 0;

	if((Size * 3) < BURST_MAX_SIZE)
	{
		Buffer_Size = Size;
	}
	else
	{
		Buffer_Size = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

	unsigned char Byte1 = (Colour>>8)&0xF8;	//RED
	unsigned char Byte2 = (Colour>>3)&0xFC;	//GREEN
	unsigned char Byte3 = (Colour<<3);			//BLUE

	unsigned char burst_buffer[Buffer_Size];

	for(uint32_t j = 0; j < Buffer_Size; j+=3)
		{
			burst_buffer[j] = 	Byte1;
			burst_buffer[j+1] = Byte2;
			burst_buffer[j+2] = Byte3;
		}
	
	uint32_t Sending_Size = Size * 3;
	uint32_t Sending_in_Block = Sending_Size / Buffer_Size;
	uint32_t Remainder_from_block = Sending_Size % Buffer_Size;

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
void ILI9488_Fill_Screen(uint16_t Colour)
{
	ILI9488_Set_Address(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
	ILI9488_Draw_Colour_Burst(Colour, LCD_WIDTH * LCD_HEIGHT);
}

//DRAW PIXEL AT XY POSITION WITH SELECTED COLOUR
//
//Location is dependant on screen orientation. x0 and y0 locations change with orientations.
//Using pixels to draw big simple structures is not recommended as it is really slow
//Try using either rectangles or lines if possible
//
void ILI9488_Draw_Pixel(uint16_t X,uint16_t Y,uint16_t Colour) 
{
	if((X >=LCD_WIDTH) || (Y >=LCD_HEIGHT)) return;	//OUT OF BOUNDS!

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ILI9488_SPI_Send(0x2A);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//XDATA
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer[4] = {X>>8,X, (X+1)>>8, (X+1)};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer, 4, 1);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ILI9488_SPI_Send(0x2B);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//YDATA
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer1[4] = {Y>>8,Y, (Y+1)>>8, (Y+1)};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer1, 4, 1);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
	
	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	ILI9488_SPI_Send(0x2C);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
	
	//COLOUR
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	unsigned char Temp_Buffer2[3] = {(Colour>>8)&0xF8, (Colour>>3)&0xFC, Colour<<3};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer2, 3, 1);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

//DRAW RECTANGLE OF SET SIZE AND HEIGTH AT X and Y POSITION WITH CUSTOM COLOUR
//
//Rectangle is hollow. X and Y positions mark the upper left corner of rectangle
//As with all other draw calls x0 and y0 locations dependant on screen orientation
//

void ILI9488_Draw_Fill_Rectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
	if((X >= LCD_WIDTH) || (Y >= LCD_HEIGHT)) return;

	if((X + Width - 1) >= LCD_WIDTH)
	{
			Width = LCD_WIDTH - X;
	}
	if((Y + Height - 1) >= LCD_HEIGHT)
	{
			Height = LCD_HEIGHT - Y;
	}

	ILI9488_Set_Address(X, Y, X + Width - 1, Y + Height - 1);
	ILI9488_Draw_Colour_Burst(Colour, Height * Width);
}

//DRAW LINE FROM X,Y LOCATION to X+Width,Y LOCATION
void ILI9488_Draw_Horizontal_Line(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Colour)
{
	if((X >= LCD_WIDTH) || (Y >= LCD_HEIGHT)) return;

	if((X+Width-1)>=LCD_WIDTH)
	{
			Width=LCD_WIDTH-X;
	}
	ILI9488_Set_Address(X, Y, X+Width-1, Y);
	ILI9488_Draw_Colour_Burst(Colour, Width);
}

//DRAW LINE FROM X,Y LOCATION to X,Y+Height LOCATION
void ILI9488_Draw_Vertical_Line(uint16_t X, uint16_t Y, uint16_t Height, uint16_t Colour)
{
	if((X >= LCD_WIDTH) || (Y >= LCD_HEIGHT)) return;
	if((Y+Height-1)>=LCD_HEIGHT)
	{
			Height=LCD_HEIGHT-Y;
	}
	ILI9488_Set_Address(X, Y, X, Y+Height-1);
	ILI9488_Draw_Colour_Burst(Colour, Height);
}

/* ==========================================================================================================================
 *
 */
void ILI9488_Draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
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
  	ILI9488_Draw_Pixel(uRow, uCol, color);
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

void ILI9488_Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  ILI9488_Draw_line(x1,y1,x1,y2,color);
  ILI9488_Draw_line(x1,y2,x2,y2,color);
  ILI9488_Draw_line(x2,y2,x2,y1,color);
  ILI9488_Draw_line(x2,y1,x1,y1,color);
}

void ILI9488_Draw_Circle(uint16_t x, uint16_t y, uint16_t radian, uint16_t color)
{
  int a, b;
  int di;

  a = 0;
  b = radian;
  di = 3 - (radian << 1);

  while(a <= b)
  {
  	ILI9488_Draw_Pixel(x-b,y-a,color); //3
  	ILI9488_Draw_Pixel(x+b,y-a,color); //0
  	ILI9488_Draw_Pixel(x-a,y+b,color); //1
  	ILI9488_Draw_Pixel(x-b,y-a,color); //7
  	ILI9488_Draw_Pixel(x-a,y-b,color); //2
  	ILI9488_Draw_Pixel(x+b,y+a,color); //4
  	ILI9488_Draw_Pixel(x+a,y-b,color); //5
  	ILI9488_Draw_Pixel(x+a,y+b,color); //6
  	ILI9488_Draw_Pixel(x-b,y+a,color);
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
    ILI9488_Draw_Pixel(x+a,y+b,color);
  }
}

void ILI9488_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	/* Draw lines */
	ILI9488_Draw_line(x1, y1, x2, y2, color);
	ILI9488_Draw_line(x2, y2, x3, y3, color);
	ILI9488_Draw_line(x3, y3, x1, y1, color);
}

void ILI9488_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		ILI9488_Draw_line(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void ILI9488_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	ILI9488_Draw_Pixel(x0, y0 + r, color);
	ILI9488_Draw_Pixel(x0, y0 - r, color);
	ILI9488_Draw_Pixel(x0 + r, y0, color);
	ILI9488_Draw_Pixel(x0 - r, y0, color);
	ILI9488_Draw_Horizontal_Line(x0 - r, y0, x0 + r, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9488_Draw_Horizontal_Line(x0 - x, y0 + y, x0 + x, color);
		ILI9488_Draw_Horizontal_Line(x0 + x, y0 - y, x0 - x, color);

		ILI9488_Draw_Horizontal_Line(x0 + y, y0 + x, x0 - y, color);
		ILI9488_Draw_Horizontal_Line(x0 + y, y0 - x, x0 - y, color);
	}
}

void ILI9488_SetTextColor(uint16_t color)
{
	POINT_COLOR = color;
}

void ILI9488_SetBackgroundColor(uint16_t color)
{
	BACK_COLOR = color;
}

void ILI9488_Putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
  uint16_t colortemp = POINT_COLOR;

  if(x > (LCD_WIDTH - 8) || y > LCD_HEIGHT - 16) return;
  num=num-' ';

  if(!mode)
  {
    for(pos = 0; pos < 16; pos++)
    {
      temp = asc2_1608[(uint16_t)num*16 + pos];
      for(t = 0; t < 8; t++)
      {
        if(temp&0x01)ILI9488_Draw_Pixel(x+t,y+pos,POINT_COLOR);
        else ILI9488_Draw_Pixel(x+t,y+pos,BACK_COLOR);
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
        if(temp&0x01)ILI9488_Draw_Pixel(x+t,y+pos,POINT_COLOR);
        temp>>=1;
      }
    }
  }

  POINT_COLOR=colortemp;
}

void ILI9488_Putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>LCD_WIDTH-14)||(y>LCD_HEIGHT-24)) return;
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
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

void ILI9488_Putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>LCD_WIDTH-18)||(y>LCD_HEIGHT-32)) return;
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
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}

void ILI9488_Putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
  if((x>LCD_WIDTH-26)||(y>LCD_HEIGHT-48)) return;
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
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),POINT_COLOR);
          }
          else
          {
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),BACK_COLOR);
          }
        }
        else
        {
          if(temp&(0x01<<j))
          {
          	ILI9488_Draw_Pixel(x+k,y+(8*i+j),POINT_COLOR);
          }
        }
      }
    }
  }
}


void ILI9488_Puts8x16(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE)
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

    if(x > LCD_WIDTH - font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > LCD_HEIGHT - font_h)
    {
      x = y = 0;
    }

    ILI9488_Putchar(x, y,*(string+i),TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}

void ILI9488_Puts14x24(uint16_t x, uint16_t y, uint8_t *string,uint8_t TFT_STRING_MODE)
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

    if(x > LCD_WIDTH-font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > LCD_HEIGHT-font_h)
    {
      x = y = 0;
    }

    ILI9488_Putchar14x24(x, y, *(string+i), TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}

void ILI9488_Puts18x32(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE)
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

    if(x > LCD_WIDTH-font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > LCD_HEIGHT-font_h)
    {
      x = y = 0;
    }

    ILI9488_Putchar18x32(x, y, *(string+i),TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}

void ILI9488_Puts26x48(uint16_t x, uint16_t y, uint8_t *string, uint8_t TFT_STRING_MODE)
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

    if(x > LCD_WIDTH - font_w)
    {
      x = 0;
      y += font_h;
    }

    if(y > LCD_HEIGHT - font_h)
    {
      x = y = 0;
    }

    ILI9488_Putchar26x48(x, y, *(string+i), TFT_STRING_MODE);
    x += font_w;
    i++;
  }
}
