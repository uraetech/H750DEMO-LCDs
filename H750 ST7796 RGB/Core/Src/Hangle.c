//Font Name : Font_018
//---------------------------------------------------------
#include "main.h"
#include "ST7796S_RGB.h"
#include "EFont.h"
#include "HFont.h"

#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

byte HANFontImage[32] = {0, };
uint16_t fcolor = WHITE, bcolor = BLACK;
uint16_t Transparent = 0;

void SetForColor(uint16_t c)
{
	fcolor = c;
}

void SetBackColor(uint16_t c)
{
	bcolor = c;
}

void SetTransparent(uint16_t t)
{
	Transparent = t;
}
/*=============================================================================
      한글 font 처리부분
  =============================================================================*/

byte *getHAN_font(byte HAN1, byte HAN2, byte HAN3)
{
  uint16_t utf16;
  uint8_t first, mid, last;
  uint8_t i;
  byte firstType, midType, lastType = 0;
  byte *pB, *pF;

  const byte cho[]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0 };
  const byte cho2[] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5 };
  const byte jong[] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 };

  /*------------------------------
    UTF-8 을 UTF-16으로 변환한다.

    UTF-8 1110xxxx 10xxxxxx 10xxxxxx
   ------------------------------ */

  utf16 = ((HAN1 & 0x0f) << 12L) | ((HAN2 & 0x3f) << 6L) | (HAN3 & 0x3f);

  /*------------------------------
    초,중,종성 코드를 분리해 낸다.

    unicode = {[(초성 * 21) + 중성] * 28}+ 종성 + 0xAC00

          0   1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
    초성 ㄱ   ㄲ ㄴ ㄷ ㄸ ㄹ ㅁ ㅂ ㅃ ㅅ ㅆ ㅇ ㅈ ㅉ ㅊ ㅋ ㅌ ㅍ ㅎ
    중성 ㅏ   ㅐ ㅑ ㅒ ㅓ ㅔ ㅕ ㅖ ㅗ ㅘ ㅙ ㅚ ㅛ ㅜ ㅝ ㅞ ㅟ ㅠ ㅡ ㅢ ㅣ
    종성 없음 ㄱ ㄲ ㄳ ㄴ ㄵ ㄶ ㄷ ㄹ ㄺ ㄻ ㄼ ㄽ ㄾ ㄿ ㅀ ㅁ ㅂ ㅄ ㅅ ㅆ ㅇ ㅈ ㅊ ㅋ ㅌ ㅍ ㅎ
  ------------------------------*/
  if((utf16 >= 0x3131) && (utf16 <= 0x318E))	// 단자음, 단모음
  {
  	if(utf16 >= 0x314F)		// 단모음
  	{
  		mid = utf16 - 0x314F + 1;
  	  pB = HANFontImage;
  	  pF = (byte*)HFont + (8 * 20 + mid) * 32;
  	  i = 32;
  	  while(i--) *pB++ = *pF++;
  	  return HANFontImage;
  	}
  	else	// 단자음
  	{
  		first = utf16 - 0x3131 + 1;
  	  pB = HANFontImage;
  	  pF = (byte*)HFont + (20 + first) * 32;
  	  i = 32;
  	  while(i--) *pB++ = *pF++;
  	  return HANFontImage;
  	}
  }


  utf16 -= 0xac00;

  first = (utf16 / 21) / 28;
  mid   = (utf16 % (21 * 28) / 28);
  last  = utf16 % 28;

  first++;
  mid++;

  /*------------------------------
    초,중,종성 해당 폰트 타입(벌)을 결정한다.
  ------------------------------*/

  /*
   초성 19자:ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ
   중성 21자:ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ
   종성 27자:ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅆㅇㅈㅊㅋㅌㅍㅎ

   초성
      초성 1벌 : 받침없는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 2벌 : 받침없는 'ㅗㅛㅡ'
      초성 3벌 : 받침없는 'ㅜㅠ'
      초성 4벌 : 받침없는 'ㅘㅙㅚㅢ'
      초성 5벌 : 받침없는 'ㅝㅞㅟ'
      초성 6벌 : 받침있는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 7벌 : 받침있는 'ㅗㅛㅜㅠㅡ'
      초성 8벌 : 받침있는 'ㅘㅙㅚㅢㅝㅞㅟ'

   중성
      중성 1벌 : 받침없는 'ㄱㅋ' 와 결합
      중성 2벌 : 받침없는 'ㄱㅋ' 이외의 자음
      중성 3벌 : 받침있는 'ㄱㅋ' 와 결합
      중성 4벌 : 받침있는 'ㄱㅋ' 이외의 자음

   종성
      종성 1벌 : 중성 'ㅏㅑㅘ' 와 결합
      종성 2벌 : 중성 'ㅓㅕㅚㅝㅟㅢㅣ'
      종성 3벌 : 중성 'ㅐㅒㅔㅖㅙㅞ'
      종성 4벌 : 중성 'ㅗㅛㅜㅠㅡ'

  */
  if(!last)  //받침 없는 경우
  {
    firstType = cho[mid];
    if(first == 1 || first == 24) midType = 0;
    else midType = 1;
  }
  else       //받침 있는 경우
  {
    firstType = cho2[mid];
    if(first == 1 || first == 24) midType = 2;
    else midType = 3;
    lastType = jong[mid];
  }

  //초성
  pB = HANFontImage;
  pF = (byte*)HFont + (firstType * 20 + first) * 32;
  i = 32;
  while(i--) *pB++ = *pF++;

  //중성
  pB = HANFontImage;
  pF = (byte*)HFont + (8 * 20 + midType * 22 + mid) * 32;
  i = 32;
  while(i--) *pB++ |= *pF++;

  //종성
  if(last)
  {
    pB = HANFontImage;
    pF = (byte *)HFont + (8 * 20 + 4 * 22 + lastType * 28 + last) * 32;
    i = 32;
    while(i--) *pB++ |= *pF++;
  }

  return HANFontImage;
}

void DisplayHangle(uint16_t XPOS, uint16_t YPOS, char *pChar)
{
//  byte rg = 3;   //<b1> red, <b0> green
  byte *pFs;
//  byte i, b;
  byte c, c2, c3;
	uint16_t i, j, k, bit;

  while(*pChar)
  {
    c = *(byte*)pChar++;

    //---------- 한글 ---------
    if(c & 0x80)
    {
      c2 = *(byte*)pChar++;
      c3 = *(byte*)pChar++;
      //Serial.print(c,DEC);
      //Serial.print(c2,DEC);
      //Serial.print(c3,DEC);
      pFs = getHAN_font(c, c2, c3);

			for(i = 0; i < 16; i++)
			{
				for(bit = 0; bit < 8; bit++)
				{
					if((pFs[i * 2] << bit) & 0x80) BSP_LCD_DrawPixel(XPOS + bit,  YPOS + i, fcolor);
					else if(!Transparent) BSP_LCD_DrawPixel(XPOS + bit,  YPOS + i, bcolor);
				}

				for(bit = 0; bit < 8; bit++)
				{
					if((pFs[i * 2 + 1] << bit) & 0x80) BSP_LCD_DrawPixel(XPOS + 8 + bit,  YPOS + i, fcolor);
					else if(!Transparent) BSP_LCD_DrawPixel(XPOS + 8 + bit,  YPOS + i, bcolor);
				}
			}

      XPOS = XPOS + 16;
      if(XPOS > BSP_LCD_GetXSize())
      {
        XPOS = 0;
        YPOS = YPOS + 16;
      }
    }
    //---------- ASCII ---------
    else
    {
			for(k = 0; k < 2; k++)
			{
				for(i = 0; i < 8; i++)
				{
					for(j = 0; j < 8; j++)
					{
						if(E_FONT[c][i + k * 8] & (0x01 << j)) BSP_LCD_DrawPixel(XPOS + i, YPOS + k * 8 + j, fcolor);
						else if(!Transparent) BSP_LCD_DrawPixel(XPOS + i, YPOS + k * 8 + j, bcolor);
					}
				}
			}

			XPOS = XPOS + 8;
      if(XPOS > BSP_LCD_GetXSize())
      {
        XPOS = 0;
        YPOS = YPOS + 16;
      }
    }
  }
}

