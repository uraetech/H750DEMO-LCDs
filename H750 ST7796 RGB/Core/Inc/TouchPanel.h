#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define TP_CS(x)			HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, x ? GPIO_PIN_SET : GPIO_PIN_RESET)

#define TP_INT_IN()   HAL_GPIO_ReadPin(TOUCH_PRESS_GPIO_Port, TOUCH_PRESS_Pin)

/* Private typedef -----------------------------------------------------------*/
//typedef	struct POINT
//{
//   uint16_t x;
//   uint16_t y;
//}Point;


typedef struct Matrix
{
long double An,
            Bn,
            Cn,
            Dn,
            En,
            Fn,
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Point ScreenSample[3];
extern Point DisplaySample[3];
extern Matrix matrix ;
extern Point  display ;

/* Private define ------------------------------------------------------------*/

#define	CHX 	0x90
#define	CHY 	0xd0

/* Private function prototypes -----------------------------------------------*/
void TP_Init(void);
Point *Read_XPT_2046(void);
void TouchPanel_Calibrate(void);
void TP_GetXY(uint16_t *x, uint16_t *y);
int GetTouchStatus(Point * pos);

#endif		// _TOUCHPANEL_H_
