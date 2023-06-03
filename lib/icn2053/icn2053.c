#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "rgbcolor.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "icn2053.h"

// original
/*
#define PIN_OE			3
#define PIN_CLK			4
#define PIN_LE 			5

#define PIN_A 			22
#define PIN_B 			23
#define PIN_C  			25
#define PIN_D   		26
#define PIN_E   		27

#define PIN_R1   		2
#define PIN_G1   		16
#define PIN_B1   		17
#define PIN_R2   		18
#define PIN_G2   		19
#define PIN_B2   		21
*/



    #define PIN_R1  25
    #define PIN_G1  26
    #define PIN_B1  27
    #define PIN_R2  18  
    #define PIN_G2  21 
    #define PIN_B2  2  

    #define PIN_A   23
    #define PIN_B   19
    #define PIN_C   5
    #define PIN_D   17
    #define PIN_E   22 
              
    #define PIN_LE 4
    #define PIN_OE  15
    #define PIN_CLK 16
	





#define SCAN_MODE		32
#define SCAN_MASK		(SCAN_MODE - 1)

#define USE_ESP32_RMT	0

static uint16_t _width;
static uint16_t _height;
static uint8_t scan = 0;
static uint8_t WrBuffIdx = 0;
static uint8_t *ScreenBuff[2];

#define setGPIO_fast(num, val) 	setGPIO32(num, val)
//#define setGPIO_fast(num, val) 	gpio_set_level(num, val)
#define setGPIO_slow(num, val) 	gpio_set_level(num, val)


void inline IRAM_ATTR setGPIO32(uint8_t gpio_num, uint32_t val)
{
	if (val)
		GPIO.out_w1ts = (1 << gpio_num);
	else
		GPIO.out_w1tc = (1 << gpio_num);
}

void inline IRAM_ATTR  icn2053_SetScan(uint8_t scan)
{
	scan &= SCAN_MASK;

	setGPIO_slow(PIN_A, scan & 0x1);
	scan >>= 1;
	setGPIO_slow(PIN_B, scan & 0x1);
	scan >>= 1;
	setGPIO_slow(PIN_C, scan & 0x1);
	// ab hier vorher abgeschaltet
	scan >>= 1;
	setGPIO_slow(PIN_D, scan & 0x1);
	scan >>= 1;
	setGPIO_slow(PIN_E, scan & 0x1);
	
}

void  IRAM_ATTR icn2053_SendLatch(uint8_t clocks)
{
	setGPIO_slow(PIN_LE, 1);

	while (clocks--)
	{
		setGPIO_fast(PIN_CLK, 1);
		setGPIO_fast(PIN_CLK, 0);
	}

	setGPIO_slow(PIN_LE, 0);
}

void  IRAM_ATTR icn2053_SendData(uint16_t R1, uint16_t G1, uint16_t B1_, uint16_t R2, uint16_t G2, uint16_t B2, uint8_t latches)
{
	uint8_t bit = 0;
	uint16_t mask = 0x8000;

	while (mask)
	{
		setGPIO_fast(PIN_CLK, 0);

		if (bit == (16 - latches))
			setGPIO_fast(PIN_LE, 1);

		setGPIO_slow(PIN_R1, R1 & mask);
		setGPIO_slow(PIN_G1, G1 & mask);
		setGPIO_slow(PIN_B1, B1_ & mask);
		setGPIO_slow(PIN_R2, R2 & mask);
		setGPIO_slow(PIN_G2, G2 & mask);
		setGPIO_slow(PIN_B2, B2 & mask);

		setGPIO_slow(PIN_CLK, 1);

		mask >>= 1;
		bit++;
	}

	setGPIO_slow(PIN_CLK, 0);
	setGPIO_slow(PIN_LE, 0);
}

void icn2053_SendDataToAllDrivers(uint16_t Data, uint16_t driversNum, uint8_t latches)
{
	while (driversNum)
	{
	    icn2053_SendData(Data, Data, Data, Data, Data, Data, driversNum == 1 ? latches : 0);
	    driversNum--;
	}
}

void icn2053_DrawPixel(int16_t x, int16_t y, tRGBcolor *pcolor)
{
	if ((x < _width) && (y < _height))
	{
		uint8_t *buff = ScreenBuff[WrBuffIdx];
		memcpy(&buff[((_width * y) + x) * 3], pcolor, 3);
	}
}

void icn2053_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, tRGBcolor *pcolor)
{
	if ((!w) || (!h))
		return;

	if (x + w > _width)
		w = _width - x;
	if (y + h > _height)
		h = _height - y;

	while (h--)
	{
		for (int16_t xIdx = x; xIdx < (x + w); xIdx++)
			icn2053_DrawPixel(xIdx, y, pcolor);

		y++;
	}
}

extern uint16_t ColorCoefs[];

void inline IRAM_ATTR icn2053_send_datapacket(int sect, int x, int y, uint8_t *buff)
{
	uint32_t idx1 = ((y * _width) + (sect << 4) + x) * 3;
	uint32_t idx2 = (((y + 32) * _width) + (sect << 4) + x) * 3;

/*
 	// ������ � ���������� �������
	uint16_t r1 = ColorCoefs[buff[idx1++]];
	uint16_t g1 = ColorCoefs[buff[idx1++]];
	uint16_t b1 = ColorCoefs[buff[idx1]];
	uint16_t r2 = ColorCoefs[buff[idx2++]];
	uint16_t g2 = ColorCoefs[buff[idx2++]];
	uint16_t b2 = ColorCoefs[buff[idx2]];
	icn2053_SendData(r1, g1, b1, r2, g2, b2, sect == 7 ? 1 : 0);
*/
	icn2053_SendData(buff[idx1] << 8, buff[idx1 + 1] << 8, buff[idx1 + 2] << 8, buff[idx2] << 8, buff[idx2 + 1] << 8, buff[idx2 + 2] << 8, sect == 7 ? 1 : 0);
}


void IRAM_ATTR icn2053_scan_next()
{
	// old
//	icn2053_SetScan(scan++);
//	scan &= 31;
	

    if(scan == 0) {
        icn2053_SetScan(4);
        icn2053_SetScan(5);
        icn2053_SetScan(4);
        icn2053_SetScan(0);

    } else {


		icn2053_SetScan(1);
        icn2053_SetScan(0);

    }
    scan = (scan+1)&31;
	
}

void IRAM_ATTR icn2053_refresh()
{
	uint8_t RdBuffIdx = (WrBuffIdx + 1) & 1;
	uint8_t *buff = ScreenBuff[RdBuffIdx];

	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	portENTER_CRITICAL(&mux);
	icn2053_vsync();

	for (uint8_t y = 0; y < 32; y++)  
	{
		for (uint8_t x = 0; x < 16; x++)
		{
			icn2053_scan_next();
			icn2053_send_oe_clock(10);

			for (uint8_t sect = 0; sect < 8; sect++)
			{
				icn2053_send_oe_clock(16);
				icn2053_send_datapacket(sect, x, y, buff);
			}
		}
	}
	portEXIT_CRITICAL(&mux);
}

// ��������� ���������� ����� ������� �����
void icn2053_update()
{
	WrBuffIdx = (WrBuffIdx + 1) & 1;
}

// ��������� ���������� ����� ������� ����� � ������������ �������� ������ � �������
void icn2053_UpdateAndCopy()
{
	WrBuffIdx = (WrBuffIdx + 1) & 1;
	memcpy(ScreenBuff[WrBuffIdx], ScreenBuff[(WrBuffIdx + 1) & 1], _height * _width * 3);
}

void inline IRAM_ATTR icn2053_send_oe_clock(uint8_t clocks)
{
    while (clocks--)
    {
    	setGPIO_fast(PIN_OE, 1);
    	setGPIO_fast(PIN_OE, 0);
    }
}

void icn2053_vsync()
{
    icn2053_SendLatch(3);
}

void icn2053_start()
{
	// Pre-active command
    icn2053_SendLatch(14);
    // Enable all output channels
    icn2053_SendLatch(12);
    // Vertical sync. signal
    icn2053_vsync();
    // Pre-active command
    icn2053_SendLatch(14);

    // Write config. register 1
    //icn2053_SendDataToAllDrivers(0x00FF, 8, 4);	// 0xff
    icn2053_SendDataToAllDrivers(0x1F70, 8, 4);
    //icn2053_SendDataToAllDrivers(0x0770, 8, 4);

    // Pre-active command
    icn2053_SendLatch(14);

    // Write config. register 2
    icn2053_SendDataToAllDrivers(0xffff, 8, 6);		// �������� ������ �� ������� �������
    //icn2053_SendDataToAllDrivers(0xF3FF, 8, 6);	// 62463
    //icn2053_SendDataToAllDrivers(0x7C80, 8, 6);

    // Pre-active command
    icn2053_SendLatch(14);

    // Write config. register 3
    icn2053_SendDataToAllDrivers(0x40F3, 8, 8);
    //icn2053_SendDataToAllDrivers(0x40F7, 8, 8);

    // Pre-active command
    icn2053_SendLatch(14);

    // Write config. register 4
    icn2053_SendDataToAllDrivers(0x0000, 8, 10);
    //icn2053_SendDataToAllDrivers(0x8080, 8, 10);

    // Pre-active command
    icn2053_SendLatch(14);

    // Write debug register
    icn2053_SendDataToAllDrivers(0x0000, 8, 2);
    //icn2053_SendDataToAllDrivers(0x0008, 8, 2);
}

void icn2053_init(uint16_t width, uint16_t height)
{
	_width = width;
	_height = height;

    // ������������� ��� GPIO
    gpio_set_direction(PIN_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_LE, GPIO_MODE_OUTPUT);

    gpio_set_direction(PIN_A, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_B, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_C, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_D, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_E, GPIO_MODE_OUTPUT);

    gpio_set_direction(PIN_R1, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_G1, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_B1, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_R2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_G2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_B2, GPIO_MODE_OUTPUT);


    setGPIO_slow(PIN_CLK, 0);
    setGPIO_slow(PIN_LE, 0);

    setGPIO_slow(PIN_R1, 0);
    setGPIO_slow(PIN_G1, 0);
    setGPIO_slow(PIN_B1, 0);
    setGPIO_slow(PIN_R2, 0);
    setGPIO_slow(PIN_G2, 0);
    setGPIO_slow(PIN_B2, 0);

    icn2053_SetScan(0);

    icn2053_start();

    ScreenBuff[0] = heap_caps_malloc(_height * _width * 3, MALLOC_CAP_8BIT);
    ScreenBuff[1] = heap_caps_malloc(_height * _width * 3, MALLOC_CAP_8BIT);
    memset(ScreenBuff[0], 0, _height * _width * 3);
    memset(ScreenBuff[1], 0, _height * _width * 3);

	gpio_iomux_out(PIN_OE, FUNC_U0RXD_GPIO3, 0);
    gpio_set_direction(PIN_OE, GPIO_MODE_OUTPUT);
    setGPIO_slow(PIN_OE, 0);
}
