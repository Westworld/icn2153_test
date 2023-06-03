//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-�����
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "esp_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "rgbcolor.h"
#include "dispcolor.h"
#include "font.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



#if (DISPCOLOR_type == DISPTYPE_st7789)
  #include <st77xx.h>
  #include <st7789.h>
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  #include <st77xx.h>
  #include <st7735.h>
#elif (DISPCOLOR_type == DISPTYPE_ili9641)
  #include "..\ili9641\ili9641.h"
#elif (DISPCOLOR_type == DISPTYPE_icn2053)
  #include "icn2053.h"
#else
  #error ������ c ��������� ����� ������� �� ��������������, ���������� �������� ������ disp1color
#endif


static uint16_t _width, _height;


//==============================================================================
// ��������� ����� ������� 2 �������� int16_t
//==============================================================================
static void SwapInt16Values(int16_t *pValue1, int16_t *pValue2)
{
  int16_t TempValue = *pValue1;
  *pValue1 = *pValue2;
  *pValue2 = TempValue;
}
//==============================================================================


//==============================================================================
// ��������� �������������� ������� �������
//==============================================================================
void dispcolor_Init(uint16_t Width, uint16_t Height)
{
	_width = Width;
	_height = Height;

  // ������������� �������
#if (DISPCOLOR_type == DISPTYPE_st7789)
  // ������������� �������
  ST7789_Init(Width, Height);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  // ������������� �������
  ST7735_Init(Width, Height);
#elif (DISPCOLOR_type == DISPTYPE_ili9641)
  ili9341_init(Width, Height);
#elif (DISPCOLOR_type == DISPTYPE_icn2053)
  icn2053_init(Width, Height);
#endif
  // ������� �������
  //dispcolor_ClearScreen();
}
//==============================================================================


//==============================================================================
// ��������� ������������� ������� �������
//==============================================================================
void dispcolor_SetBrightness(uint8_t Value)
{
  if (Value > 100)
    Value = 100;

#if (DISPCOLOR_type == DISPTYPE_st7789)
  st77xx_SetBL(Value);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  st77xx_SetBL(Value);
#endif
}
//==============================================================================


//==============================================================================
// ��������� ���������� 1 ������� �������
//==============================================================================
void dispcolor_DrawPixel(int16_t x, int16_t y, tRGBcolor *pcolor)
{
#if (DISPCOLOR_type == DISPTYPE_icn2053)
  icn2053_DrawPixel(x, y, pcolor);
#endif
}
//==============================================================================


//==============================================================================
// ��������� ���������� �������������� ������ color
//==============================================================================
void dispcolor_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, tRGBcolor *pcolor)
{
#if (DISPCOLOR_type == DISPTYPE_icn2053)
  icn2053_FillRect(x, y, w, h, pcolor);
#endif
}
//==============================================================================


//==============================================================================
// ��������� ��������� ������� �� ������ �����
//==============================================================================
void dispcolor_Update(void)
{
#if (DISPCOLOR_type == DISPTYPE_ili9641)
#if (ILI9341_MODE == ILI9341_BUFFER_MODE)
  ili9341_update();
#endif
#elif (DISPCOLOR_type == DISPTYPE_icn2053)
  icn2053_update();
#endif
}
//==============================================================================

void dispcolor_UpdateAndCopy()
{
  icn2053_UpdateAndCopy();
}

//==============================================================================
// ��������� ����������� ����� ������ color
//==============================================================================
void dispcolor_FillScreen(tRGBcolor *pcolor)
{
  dispcolor_FillRect(0, 0, _width, _height, pcolor);
}
//==============================================================================


//==============================================================================
// ��������� ������� ����� (����������� ������)
//==============================================================================
void dispcolor_ClearScreen(void)
{
	tRGBcolor color = {0, 0, 0};
	dispcolor_FillScreen(&color);
}
//==============================================================================


//==============================================================================
// ��������� ������ ������ ����� �� ������� ��������� ������� (�����������)
//==============================================================================
static void dispcolor_DrawLine_Slow(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pcolor)
{
  const int16_t deltaX = abs(x2 - x1);
  const int16_t deltaY = abs(y2 - y1);
  const int16_t signX = x1 < x2 ? 1 : -1;
  const int16_t signY = y1 < y2 ? 1 : -1;

  int16_t error = deltaX - deltaY;

  dispcolor_DrawPixel(x2, y2, pcolor);

  while (x1 != x2 || y1 != y2)
  {
	dispcolor_DrawPixel(x1, y1, pcolor);
    const int16_t error2 = error * 2;

    if (error2 > -deltaY)
    {
      error -= deltaY;
      x1 += signX;
    }
    if (error2 < deltaX)
    {
      error += deltaX;
      y1 += signY;
    }
  }
}
//==============================================================================


//==============================================================================
// ��������� ������ ������ ����� �� �������
//==============================================================================
void dispcolor_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pcolor)
{
  // ������������ �����
  if (x1 == x2)
  {
    // ������������ ����� ������� �������
    if (y1 > y2)
    	dispcolor_FillRect(x1, y2, 1, y1 - y2 + 1, pcolor);
    else
    	dispcolor_FillRect(x1, y1, 1, y2 - y1 + 1, pcolor);
    return;
  }

  // �������������� �����
  if (y1 == y2)
  {
    // ������������ ����� ������� �������
    if (x1 > x2)
    	dispcolor_FillRect(x2, y1, x1 - x2 + 1, 1, pcolor);
    else
    	dispcolor_FillRect(x1, y1, x2 - x1 + 1, 1, pcolor);
    return;
  }

  // ������������ ����� �����������
  dispcolor_DrawLine_Slow(x1, y1, x2, y2, pcolor);
}
//==============================================================================


//==============================================================================
// ��������� ������ ������������� �� �������
//==============================================================================
void dispcolor_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pcolor)
{
  dispcolor_DrawLine(x1, y1, x1, y2, pcolor);
  dispcolor_DrawLine(x2, y1, x2, y2, pcolor);
  dispcolor_DrawLine(x1, y1, x2, y1, pcolor);
  dispcolor_DrawLine(x1, y2, x2, y2, pcolor);
}
//==============================================================================


//==============================================================================
// ��������� ������ ����������� ������������� �� �������
//==============================================================================
void dispcolor_DrawRectangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pfillcolor)
{
  if (x1 > x2)
    SwapInt16Values(&x1, &x2);
  if (y1 > y2)
    SwapInt16Values(&y1, &y2);

  dispcolor_FillRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, pfillcolor);
}
//==============================================================================


//==============================================================================
// ��������� ������ ���������� �� �������. x0 � y0 - ���������� ������ ����������
//==============================================================================
void dispcolor_DrawCircle(int16_t x0, int16_t y0, int16_t radius, tRGBcolor *pcolor)
{
  int x = 0;
  int y = radius;
  int delta = 1 - 2 * radius;
  int error = 0;

  while (y >= 0)
  {
	dispcolor_DrawPixel(x0 + x, y0 + y, pcolor);
	dispcolor_DrawPixel(x0 + x, y0 - y, pcolor);
	dispcolor_DrawPixel(x0 - x, y0 + y, pcolor);
	dispcolor_DrawPixel(x0 - x, y0 - y, pcolor);
    error = 2 * (delta + y) - 1;

    if (delta < 0 && error <= 0)
    {
      ++x;
      delta += 2 * x + 1;
      continue;
    }

    error = 2 * (delta - x) - 1;

    if (delta > 0 && error > 0)
    {
      --y;
      delta += 1 - 2 * y;
      continue;
    }

    ++x;
    delta += 2 * (x - y);
    --y;
  }
}
//==============================================================================


//==============================================================================
// ��������� ������ ����������� ���������� �� �������. x0 � y0 - ���������� ������ ����������
//==============================================================================
void dispcolor_DrawCircleFilled(int16_t x0, int16_t y0, int16_t radius, tRGBcolor *pfillcolor)
{
  int x = 0;
  int y = radius;
  int delta = 1 - 2 * radius;
  int error = 0;

  while (y >= 0)
  {
    dispcolor_DrawLine(x0 + x, y0 - y, x0 + x, y0 + y, pfillcolor);
    dispcolor_DrawLine(x0 - x, y0 - y, x0 - x, y0 + y, pfillcolor);
    error = 2 * (delta + y) - 1;

    if (delta < 0 && error <= 0)
    {
      ++x;
      delta += 2 * x + 1;
      continue;
    }

    error = 2 * (delta - x) - 1;

    if (delta > 0 && error > 0)
    {
      --y;
      delta += 1 - 2 * y;
      continue;
    }

    ++x;
    delta += 2 * (x - y);
    --y;
  }
}
//==============================================================================


//==============================================================================
// ������� ������ ������� Char �� �������. ���������� ������ ����������� �������
//==============================================================================
static uint8_t dispcolor_DrawChar_General(int16_t X, int16_t Y, uint8_t FontID,
                                          uint8_t Char, tRGBcolor *pTextColor,
										  tRGBcolor *pBgColor, uint8_t TransparentBg)
{
  // ��������� �� ����������� ����������� ������� ������
  uint8_t *pCharTable = font_GetFontStruct(FontID, Char);
  uint8_t CharWidth = font_GetCharWidth(pCharTable);    // ������ �������
  uint8_t CharHeight = font_GetCharHeight(pCharTable);  // ������ �������
  pCharTable += 2;

  if (FontID == FONTID_6X8M)
  {
    for (uint8_t row = 0; row < CharHeight; row++)
    {
      for (uint8_t col = 0; col < CharWidth; col++)
      {
        if (pCharTable[row] & (1 << (7 - col)))
          dispcolor_DrawPixel(X + col, Y + row, pTextColor);
        else if (!TransparentBg)
          dispcolor_DrawPixel(X + col, Y + row, pBgColor);
      }
    }
  }
  else
  {
    for (uint8_t row = 0; row < CharHeight; row++)
    {
      for (uint8_t col = 0; col < CharWidth; col++)
      {
        if (col < 8)
        {
          if (pCharTable[row * 2] & (1 << (7 - col)))
            dispcolor_DrawPixel(X + col, Y + row, pTextColor);
          else if (!TransparentBg)
            dispcolor_DrawPixel(X + col, Y + row, pBgColor);
        }
        else
        {
          if (pCharTable[(row * 2) + 1] & (1 << (15 - col)))
            dispcolor_DrawPixel(X + col, Y + row, pTextColor);
          else if (!TransparentBg)
            dispcolor_DrawPixel(X + col, Y + row, pBgColor);
        }
      }
    }
  }

  return CharWidth;
}
//==============================================================================


//==============================================================================
// ������� ������ ������� Char �� �������. ���������� ������ ����������� �������
//==============================================================================
uint8_t dispcolor_DrawChar(int16_t X, int16_t Y, uint8_t FontID, uint8_t Char,
						   tRGBcolor *pTextColor)
{
  return dispcolor_DrawChar_General(X, Y, FontID, Char, pTextColor, 0, 1);
}
//==============================================================================


//==============================================================================
// ������� ������ ������� Char �� �������. ���������� ������ ����������� �������
//==============================================================================
uint8_t dispcolor_DrawChar_Bg(int16_t X, int16_t Y, uint8_t FontID, uint8_t Char,
							  tRGBcolor *pTextColor, tRGBcolor *pBgColor)
{
  return dispcolor_DrawChar_General(X, Y, FontID, Char, pTextColor, pBgColor, 0);
}
//==============================================================================


//==============================================================================
// ������� ������ ������ �� ������ Str �� �������
//==============================================================================
static int16_t dispcolor_DrawString_General(int16_t X, int16_t Y, uint8_t FontID,
                                         uint8_t *Str, tRGBcolor *pTextColor,
										 tRGBcolor *pBgColor, uint8_t TransparentBg)
{
  uint8_t done = 0;             // ���� ��������� ������
  int16_t Xstart = X;           // ���������� ���� ����� ���������� ������� ��� �������� �� ����� ������
  uint8_t StrHeight = 8;        // ������ �������� � �������� ��� �������� �� ��������� ������

  // ����� ������
  while (!done)
  {
    switch (*Str)
    {
    case '\0':  // ����� ������
      done = 1;
      break;
    case '\n':  // ������� �� ��������� ������
      Y += StrHeight;
      break;
    case '\r':  // ������� � ������ ������
      X = Xstart;
      break;
    default:    // ������������ ������
      if (TransparentBg)
        X += dispcolor_DrawChar(X, Y, FontID, *Str, pTextColor);
      else
        X += dispcolor_DrawChar_Bg(X, Y, FontID, *Str, pTextColor, pBgColor);

      StrHeight = font_GetCharHeight(font_GetFontStruct(FontID, *Str));
      break;
    }
    Str++;
  }
  return X;
}
//==============================================================================


//==============================================================================
// ������� ������ ������ �� ������ Str �� �������
//==============================================================================
int16_t dispcolor_DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, tRGBcolor *pTextColor)
{
  return dispcolor_DrawString_General(X, Y, FontID,  Str, pTextColor, 0, 1);
}
//==============================================================================


//==============================================================================
// ������� ������ ������ �� ������ Str �� �������
//==============================================================================
int16_t dispcolor_DrawString_Bg(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, tRGBcolor *pTextColor, tRGBcolor *pBgColor)
{
  return dispcolor_DrawString_General(X, Y, FontID,  Str, pTextColor, pBgColor, 0);
}
//==============================================================================


//==============================================================================
// ������� ������� �� ������� ��������������� ������
//==============================================================================
int16_t dispcolor_printf(int16_t X, int16_t Y, uint8_t FontID, tRGBcolor *pTextColor, const char *args, ...)
{
  char StrBuff[256];

  va_list ap;
  va_start(ap, args);
  vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
  va_end(ap);

  return dispcolor_DrawString(X, Y, FontID, (uint8_t *)StrBuff, pTextColor);
}
//==============================================================================


//==============================================================================
// ������� ������� �� ������� ��������������� ������
//==============================================================================
int16_t dispcolor_printf_Bg(int16_t X, int16_t Y, uint8_t FontID, tRGBcolor *pTextColor, tRGBcolor *pBgColor, const char *args, ...)
{
  char StrBuff[256];

  va_list ap;
  va_start(ap, args);
  vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
  va_end(ap);

  return dispcolor_DrawString_Bg(X, Y, FontID, (uint8_t *)StrBuff, pTextColor, pBgColor);
}
//==============================================================================


//==============================================================================
// ������� ���������� ������ ������ � ��������
//==============================================================================
int16_t dispcolor_getStrWidth(uint8_t FontID, char *Str)
{
	uint8_t done = 0;       // ���� ��������� ������
	int16_t StrWidth = 0;  	// ������ ������ � ��������

	// ����� ������
	while (!done)
	{
		switch (*Str)
	    {
	    case '\0':  // ����� ������
	    	done = 1;
	      	break;
	    case '\n':  // ������� �� ��������� ������
	    case '\r':  // ������� � ������ ������
	    	break;
	    default:    // ������������ ������
	    	StrWidth += font_GetCharWidth(font_GetFontStruct(FontID, *Str));
	    	break;
	    }
		Str++;
	}

	return StrWidth;
}
//==============================================================================


//==============================================================================
// ������� ���������� ������ ��������������� ������ � ��������
//==============================================================================
int16_t dispcolor_getFormatStrWidth(uint8_t FontID, const char *args, ...)
{
  char StrBuff[256];

  va_list ap;
  va_start(ap, args);
  vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
  va_end(ap);

  return dispcolor_getStrWidth(FontID, StrBuff);
}
//==============================================================================
