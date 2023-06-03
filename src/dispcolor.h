//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-�����
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _DISPCOLOR_H
#define _DISPCOLOR_H

#include "font.h" //"fonts/font.h"
#include "rgbcolor.h"


// ������ �������������� �������� (������������ ��������)
#define DISPTYPE_st7789         0
#define DISPTYPE_st7735         1
#define DISPTYPE_ili9641        2
#define DISPTYPE_icn2053        3

// � ����� ����� ������� ����� �������� ������ disp1color
#define DISPCOLOR_type         DISPTYPE_icn2053


#define RGB565(r, g, b)         (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// ������� �����
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

#ifdef __cplusplus
extern "C" {
#endif

// ��������� �������������� ������� �������
void dispcolor_Init(uint16_t Width, uint16_t Height);
// ��������� ������� ����� (����������� ������)
void dispcolor_ClearScreen(void);
// ��������� ������������� ������� �������
void dispcolor_SetBrightness(uint8_t Value);
// ��������� ���������� �������������� ������ color
void dispcolor_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, tRGBcolor *pcolor);
// ��������� ���������� 1 ������� �������
void dispcolor_DrawPixel(int16_t X, int16_t Y, tRGBcolor *pcolor);
// ��������� ����������� ����� ������ color
void dispcolor_FillScreen(tRGBcolor *pcolor);
// ��������� ��������� ������� �� ������ �����
void dispcolor_Update(void);
void dispcolor_UpdateAndCopy(void);
// ��������� ������ ������ ����� �� �������
void dispcolor_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pcolor);
// ��������� ������ ������������� �� �������
void dispcolor_DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pcolor);
// ��������� ������ ����������� ������������� �� �������
void dispcolor_DrawRectangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2, tRGBcolor *pfillcolor);
// ��������� ������ ���������� �� �������. x0 � y0 - ���������� ������ ����������
void dispcolor_DrawCircle(int16_t x0, int16_t y0, int16_t radius, tRGBcolor *pcolor);
// ��������� ������ ����������� ���������� �� �������. x0 � y0 - ���������� ������ ����������
void dispcolor_DrawCircleFilled(int16_t x0, int16_t y0, int16_t radius, tRGBcolor *pfillcolor);
// ������� ������ ������ �� ������ Str �� �������. ���������� ������ ������ � ��������
int16_t dispcolor_DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, tRGBcolor *pTextColor);
// ������� ������ ������ �� ������ Str �� �������. ���������� ������ ������ � ��������
int16_t dispcolor_DrawString_Bg(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, tRGBcolor *pTextColor, tRGBcolor *pBgColor);
// �x�������� ������� �� ������� ��������������� ������. ���������� ������ ������ � ��������
int16_t dispcolor_printf(int16_t X, int16_t Y, uint8_t FontID, tRGBcolor *pTextColor, const char *args, ...);
// ��������� ������� �� ������� ��������������� ������. ���������� ������ ������ � ��������
int16_t dispcolor_printf_Bg(int16_t X, int16_t Y, uint8_t FontID, tRGBcolor *pTextColor, tRGBcolor *pBgColor, const char *args, ...);
// ������� ���������� ������ ������ � ��������
int16_t dispcolor_getStrWidth(uint8_t FontID, char *Str);
// ������� ���������� ������ ��������������� ������ � ��������
int16_t dispcolor_getFormatStrWidth(uint8_t FontID, const char *args, ...);

#ifdef __cplusplus
}
#endif

#endif
