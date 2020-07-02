/* Includes ------------------------------------------------------------------*/
#include "ST7796S_RGB.h"
#include "TouchPanel.h"
#include "spi.h"


/* Private variables ---------------------------------------------------------*/
Matrix 		matrix ;
Point  display;


Point ScreenSample[3];

Point DisplaySample[3] = {
                                 	{  30,  45 },
                                 	{ 380,  45 },
                                 	{ 200, 210 }
	                          };

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 		2
#define TP_hspi 		hspi2

/*******************************************************************************
* Function Name  : TP_Init
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_Init(void)
{
}

/*******************************************************************************
* Function Name  : DelayUS
* Description    :
* Input          : - cnt:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DelayUS(uint32_t cnt)
{
	volatile uint32_t i;
	i = cnt * 10;
	while(i--);
}


/*******************************************************************************
* Function Name  : WR_CMD
* Description    :
* Input          : - cmd:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void WR_CMD (uint8_t cmd)
{
	HAL_SPI_Transmit(&TP_hspi, &cmd, 1, 1000);
}


/*******************************************************************************
* Function Name  : RD_AD
* Description    :
* Input          : None
* Output         : None
* Return         :
* Attention		 : None
*******************************************************************************/
static uint16_t RD_AD(void)
{
	uint8_t buf[2] = {0, 0 };
	int value;

	HAL_SPI_Receive(&TP_hspi, buf, 2, 1000);
	value = (((uint16_t)buf[0] * 256 +  buf[1]) >> 3) & 0x0FFF;

	return value;
}


/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC X
* Input          : None
* Output         : None
* Return         :
* Attention		 : None
*******************************************************************************/
uint16_t Read_X(void)
{
  uint16_t i;

  TP_CS(0);
  DelayUS(1);
  WR_CMD(CHX);
  DelayUS(1);
  i = RD_AD();
  TP_CS(1);

  return i;
}

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC Y
* Input          : None
* Output         : None
* Return         :
* Attention		 : None
*******************************************************************************/
uint16_t Read_Y(void)
{
	uint16_t i;

  TP_CS(0);
  DelayUS(1);
  WR_CMD(CHY);
  DelayUS(1);
  i = RD_AD();
  TP_CS(1);

  return i;
}


/*******************************************************************************
* Function Name  : TP_GetXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         :
* Attention		 : None
*******************************************************************************/
void TP_GetXY(uint16_t *x, uint16_t *y)
{
	uint16_t tpx, tpy;

  tpx = Read_X();
  DelayUS(1);
  tpy = Read_Y();
  *x = tpx;
  *y = tpy;
}

/*******************************************************************************
* Function Name  : Read_XPT2046
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : Point *
* Attention		 : None
*******************************************************************************/
Point *Read_XPT2046(void)
{
  static Point screen;
  uint16_t m0, m1, m2, TP_X[1], TP_Y[1], temp[3];
  uint8_t count = 0;
  int buffer[2][9] = {{0}, {0}};

  do
  {
    TP_GetXY(TP_X, TP_Y);
    buffer[0][count] = TP_X[0];
    buffer[1][count] = TP_Y[0];
    count++;
  }
  while((GPIO_PIN_RESET == TP_INT_IN()) && (count < 9));  /* TP_INT_IN()  */

	if(count == 9)   /* Average X Y  */
  {
		/* Average X  */
		temp[0] = (buffer[0][0] + buffer[0][1] + buffer[0][2]) / 3;
		temp[1] = (buffer[0][3] + buffer[0][4] + buffer[0][5]) / 3;
		temp[2] = (buffer[0][6] + buffer[0][7] + buffer[0][8]) / 3;

		m0=temp[0] - temp[1];
		m1=temp[1] - temp[2];
		m2=temp[2] - temp[0];

		m0 = m0 > 0 ? m0 : (-m0);
		m1 = m1 > 0 ? m1 : (-m1);
		m2 = m2 > 0 ? m2 : (-m2);

		if( m0 > THRESHOLD  &&  m1 > THRESHOLD  &&  m2 > THRESHOLD ) return 0;

		if(m0 < m1)
		{
			if(m2 < m0)
				screen.X = (temp[0] + temp[2]) / 2;
			else
				screen.X = (temp[0] + temp[1]) / 2;
		}
		else if(m2 < m1)
			screen.X = (temp[0] + temp[2]) / 2;
		else
			screen.X = (temp[1] + temp[2]) / 2;

		/* Average Y  */
		temp[0] = (buffer[1][0] + buffer[1][1] + buffer[1][2]) / 3;
		temp[1] = (buffer[1][3] + buffer[1][4] + buffer[1][5]) / 3;
		temp[2] = (buffer[1][6] + buffer[1][7] + buffer[1][8]) / 3;
		m0 = temp[0] - temp[1];
		m1 = temp[1] - temp[2];
		m2 = temp[2] - temp[0];
		m0 = m0 > 0 ? m0 : (-m0);
		m1 = m1 > 0 ? m1 : (-m1);
		m2 = m2 > 0 ? m2 : (-m2);
		if((m0 > THRESHOLD) && (m1 > THRESHOLD) && (m2>THRESHOLD)) return 0;

		if(m0 < m1)
		{
			if(m2 < m0)
				screen.Y = (temp[0] + temp[2]) / 2;
			else
				screen.Y = (temp[0] + temp[1]) / 2;
			}
		else if(m2 < m1)
			 screen.Y = (temp[0] + temp[2]) / 2;
		else
			 screen.Y = (temp[1] + temp[2]) / 2;

		return &screen;
  }

  return 0;
}


// 좌표값은 그리 정밀하지 않습니다.
// 아래 수치는 터치 스크린의 가장자리에서 읽은 그대로의 값으로 제품마다 편차가 있습니다.
// 더 정밀한 좌표가 필요하면 캘리브레이션 과정이 들어가야 합니다.
#define MIN_X		260
#define MAX_X		3950
#define MIN_Y		220
#define MAX_Y		3870

int GetTouchStatus(Point * pos)
{
	uint16_t x, y, tmpx, tmpy;

	if(GPIO_PIN_SET == TP_INT_IN()) return 0;

	TP_GetXY(&x, &y);

	if((x > MAX_X) || (x < MIN_X)) return 0;
	if((y > MAX_Y) || (y < MIN_Y)) return 0;

//	pos->x = (x - 180) / ((3860 - 190) / 480);
//	pos->y = (y - 190) / ((3850 - 190) / 480);
	tmpx = (x - MIN_X) * 480 / (MAX_X - MIN_X);
	tmpy = (y - MIN_Y) * 320 / (MAX_Y - MIN_Y);

	switch(BSP_LCD_GetDirection())
	{
		case 0:
			pos->X = tmpy;
			pos->Y = tmpx;
			break;

		case 1:
			pos->X = tmpx;
			pos->Y = 319 - tmpy;
			break;

		case 2:
			pos->X = 319 - tmpy;
			pos->Y = 479 - tmpx;
			break;

		case 3:
			pos->X = 479 - tmpx;
			pos->Y = tmpy;
			break;
	}

	return 1;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
