#ifndef ICN2053_H_
#define ICN2053_H_

#include "rgbcolor.h"

#ifdef __cplusplus
extern "C" {
#endif



void icn2053_init(uint16_t width, uint16_t height);
void icn2053_start();
void icn2053_vsync();
// ��������� ���������� ����� ������� �����
void icn2053_update();
// ��������� ���������� ����� ������� ����� � ������������ �������� ������ � �������
void icn2053_UpdateAndCopy();
void icn2053_refresh();
void icn2053_send_oe_clock(uint8_t clocks);
void icn2053_SetScan(uint8_t scan);

void IRAM_ATTR icn2053_scan_next();
void IRAM_ATTR icn2053_send_datapacket(int sect, int x, int y, uint8_t *buff);

void icn2053_SendDataToAllDrivers(uint16_t Data, uint16_t driversNum, uint8_t latches);
void icn2053_DrawPixel(int16_t x, int16_t y, tRGBcolor *pcolor);
void icn2053_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, tRGBcolor *pcolor);

void icn2053_SendDataStart(uint16_t R1, uint16_t G1, uint16_t B1_, uint16_t R2, uint16_t G2, uint16_t B2, uint8_t latches);
void icn2053_SendDataLatches(uint16_t R1, uint16_t G1, uint16_t B1_, uint16_t R2, uint16_t G2, uint16_t B2, uint8_t latches);

#ifdef __cplusplus
}
#endif

#endif /* ICN2053_H_ */
