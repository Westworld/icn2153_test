#include <stdio.h>
#include <string.h>
#include <arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_spi_flash.h"
#include "esp_spiram.h"
#include "dispcolor.h" //"display/dispcolor.h"
#include "esp_spi_flash.h"
#include "spiram.h" // "esp_spiram.h"


// �������� ����� ������
tRGBcolor RainbowColors[] =
{
	{255, 0, 0},    // �������
	{255, 165, 0},  // ���������
	{255, 255, 0},  // ������
	{0, 255, 0},    // ������
	{0, 255, 255},  // �������
	{0, 0, 255},    // �����
	{255, 0, 255}	// ����������
};
tRGBcolor RainbowString[384];

uint32_t Cnt = 0;
tRGBcolor bgcolor = {.r = 0, .g = 0, .b = 0};
tRGBcolor TestColors[] =
{
	{255, 255, 255},
	{255, 0, 0},
	{0, 255, 0},
	{0, 0, 255},
	{255, 255, 0},
	{255, 0, 255},
	{0, 255, 255},
	{150, 150, 150}
};


//==============================================================================
// ��������� � �������� ����� �������� �������� *source � �������� *desc
//==============================================================================
void StepChange(uint8_t *desc, uint8_t *source, uint8_t Step)
{
  if (*desc < *source)
  {
    if ((0xFF - *desc) < Step)
      *desc = 0xFF;
    else
      *desc += Step;
  }

  if (*desc > *source)
  {
    if (*desc > Step)
      *desc -= Step;
    else
      *desc = 0;
  }
}
//==============================================================================


//==============================================================================
// ��������� � �������� ����� ����������� ���� pLEDsource � pLEDdesc
//==============================================================================
void StepChangeColor(tRGBcolor *pLEDdesc, tRGBcolor *pLEDsource, uint8_t Step)
{
  StepChange(&(pLEDdesc->r), &(pLEDsource->r), Step);
  StepChange(&(pLEDdesc->g), &(pLEDsource->g), Step);
  StepChange(&(pLEDdesc->b), &(pLEDsource->b), Step);
}
//==============================================================================

uint8_t Step;

void RainbowTestInit()
{
	dispcolor_ClearScreen();

	tRGBcolor color;
	uint8_t Idx = 0;

	for (uint8_t i = 0; i < 7; i++)
	{
		color = RainbowColors[i];

		uint8_t newColor = i + 1;
		if (newColor == 7)
			newColor = 0;

		for (int step = 0; step < 19; step++)
		{
			// ������� ����� ����� � ����� �� ����� Step
			StepChangeColor(&color, &(RainbowColors[newColor]), 20);
			if (Idx < 128)
			{
				memcpy(&RainbowString[Idx], &color, 3);
				memcpy(&RainbowString[Idx + 128], &color, 3);
				memcpy(&RainbowString[Idx + 256], &color, 3);
				Idx++;
			}
		}
	}
	Step = 0;
}

void RainbowTest()
{
	int LineOffset = 0;
	for (int y = 0; y < 64; y++, LineOffset++)
	{
		for (int x = 0; x < 128; x++)
			dispcolor_DrawPixel(x, y, &RainbowString[x + LineOffset + Step]);
	}

	if (++Step == 128)
		Step = 0;

	dispcolor_Update();
	vTaskDelay(150 / portTICK_PERIOD_MS);
}

void GradientTest()
{
	dispcolor_ClearScreen();

	tRGBcolor color;
	color.r = color.g = color.b = 0;

	for (int i = 0; i < 128; i++)
	{
		color.r = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	color.r = color.g = color.b = 0;
	for (int i = 0; i < 128; i++)
	{
		color.g = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	color.r = color.g = color.b = 0;
	for (int i = 0; i < 128; i++)
	{
		color.b = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	color.r = color.g = color.b = 0;
	for (int i = 0; i < 128; i++)
	{
		color.r = color.g = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	color.r = color.g = color.b = 0;
	for (int i = 0; i < 128; i++)
	{
		color.r = color.b = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	color.r = color.g = color.b = 0;
	for (int i = 0; i < 128; i++)
	{
		color.g = color.b = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	color.r = color.g = color.b = 0;
	for (int i = 0; i < 128; i++)
	{
		color.r = color.g = color.b = i * 2;
		dispcolor_DrawLine(i, 0, i, 64, &color);
	}
    dispcolor_Update();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

static uint8_t Day = 1;
static uint8_t Month = 4;
static uint16_t Year = 2019;
static uint8_t Hour = 12;
static uint8_t Min = 32;
static uint8_t Sec = 55;
static uint16_t Msec = 0;

void TimeTick()
{
	Msec += 100;
	if (Msec == 1000)
	{
		Msec = 0;
		if (++Sec > 59)
		{
			Sec = 0;
			if (++Min > 59)
			{
				Min = 0;
				Hour++;
			}
		}
	}
}

void RefreshTest()
{
	dispcolor_ClearScreen();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	for (uint8_t y = 0; y < 32; y++)
	{
		for (uint8_t x = 0; x < 16; x++)
		{
			for (uint8_t sect = 0; sect < 8; sect++)
			{
				dispcolor_DrawPixel((sect * 16) + x, y, &TestColors[sect]);
				dispcolor_DrawPixel((sect * 16) + x, y + 32, &TestColors[sect]);
			}

			dispcolor_UpdateAndCopy();
		    //vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void ScanTest()
{
	dispcolor_ClearScreen();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	for (uint8_t y = 0; y < 32; y++)
	{
		for (uint8_t x = 0; x < 16; x++)
		{
			for (uint8_t sect = 0; sect < 8; sect++)
			{
				dispcolor_DrawPixel((sect * 16) + x, y, &TestColors[sect]);
				dispcolor_DrawPixel((sect * 16) + x, y + 32, &TestColors[sect]);
			}
		}

		if (y)
		{
			dispcolor_DrawLine(0, y - 1, 127, y - 1, &bgcolor);
			dispcolor_DrawLine(0, y + 31, 127, y + 31, &bgcolor);
		}

		//dispcolor_UpdateAndCopy();
		dispcolor_Update();
	    vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void PixelTest()
{
	int newx=0, newy=0;
	tRGBcolor black = {0, 0, 0};
	tRGBcolor red = {0xFF, 0, 0};
	tRGBcolor green = {0, 0x40, 0};
	int counter = 0;
	long timeold = 0;

	while(true) {
		timeold = millis();
		int loopcounter = 0;
		for (int y = 0; y < 64; y++)
		{
			for (int x = 0; x < 128; x++) {
				if (loopcounter++<counter)
					dispcolor_DrawPixel(x, y, &green);
				else
					dispcolor_DrawPixel(x, y, &black);
			}	
		}
		counter++;
		dispcolor_DrawPixel(newx, newy, &red);
		if (++newx>127) {
			newx=0;
			if (++newy>63)
			{	newy=0; counter = 0; }
		}
		//dispcolor_UpdateAndCopy();
		dispcolor_Update();
		long wait = 33-(millis()-timeold);
		//if (wait > 0)
		//	delay(wait);
		//delay(1);
		vTaskDelay(10);
	}


}

void TextTest1()
{
	dispcolor_ClearScreen();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    dispcolor_printf_Bg(15, 3, FONTID_16F, &TestColors[4], &bgcolor, "�����������");
    dispcolor_printf_Bg(16, 19, FONTID_16F, &TestColors[4], &bgcolor, "� ���������");
    dispcolor_printf_Bg(1, 40, FONTID_6X8M, &TestColors[5], &bgcolor, "128x64 RGBmatrix DEMO");
    dispcolor_printf_Bg(18, 52, FONTID_6X8M, &TestColors[6], &bgcolor, "ChipOne icn2053");

    dispcolor_Update();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void TextTest2()
{
	uint8_t font = FONTID_6X8M;

	// ����� � �����������
	// �������� - ��������� �����
	// �������� - �������� �����
	dispcolor_ClearScreen();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "0123456789@#$%^&(){}[\r\n]_!�|%;:.,?*/+-=");
	dispcolor_printf_Bg(0, 20, font, &TestColors[0], &bgcolor, "ABCDEFGHIJKLMNOPQRSTU\r\nVWXYZ");
	dispcolor_printf_Bg(0, 40, font, &TestColors[0], &bgcolor, "abcdefghijklmnopqrstu\r\nvwxyz");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	// ��������� - ��������� �����
	// ��������� - �������� �����
	dispcolor_ClearScreen();
	dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "�����Ũ��������������\r\n������������");
	dispcolor_printf_Bg(0, 20, font, &TestColors[0], &bgcolor, "��������������������\r\n������������");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);

	font = FONTID_16F;

	// ����� � �����������
	dispcolor_ClearScreen();
	dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "0123456789 \r\n@#$%^&(){}[]_\r\n!�|%;:.,?*/+-=");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	// �������� - ��������� �����
	dispcolor_ClearScreen();
	dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "ABCDEFGHIJKLM\r\nNOPQRSTUVWX\r\nYZ");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	// �������� - �������� �����
	dispcolor_ClearScreen();
	dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "abcdefghijklmnop\r\nqrstuvwxyz");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	// ��������� - ��������� �����
	dispcolor_ClearScreen();
	dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "�����Ũ����\r\n�����������\r\n����������\r\n�");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);
	// ��������� - �������� �����
	dispcolor_ClearScreen();
	dispcolor_printf_Bg(0, 0, font, &TestColors[0], &bgcolor, "������������\r\n�������������\r\n�������");
	dispcolor_Update();
	vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void TextTest3()
{
	dispcolor_ClearScreen();

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    dispcolor_printf_Bg(0, 0, FONTID_6X8M, &TestColors[0], &bgcolor, "ESP32 rev. %d", chip_info.revision);
    dispcolor_printf_Bg(0, 10, FONTID_6X8M, &TestColors[0], &bgcolor, "%d CPU, WiFi%s%s", chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "", (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    dispcolor_printf_Bg(0, 20, FONTID_6X8M, &TestColors[0], &bgcolor, "%d MB SPI FLASH\n", spi_flash_get_chip_size() / (1024 * 1024));

    dispcolor_printf_Bg(0, 32, FONTID_6X8M, &TestColors[4], &bgcolor, "128x64 RGB matrix");
    dispcolor_printf_Bg(0, 42, FONTID_6X8M, &TestColors[5], &bgcolor, "ChipOne icn2053");
    dispcolor_printf_Bg(0, 55, FONTID_6X8M, &TestColors[6], &bgcolor, "TIMER: %02d:%02d:%02d.%d", Hour, Min, Sec, (Msec / 100) % 10);
    Cnt++;

    dispcolor_Update();

    TimeTick();
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void TextTest4()
{
	dispcolor_ClearScreen();

	dispcolor_DrawRectangle(0, 0, 127, 63, &TestColors[7]);
	dispcolor_DrawLine(0, 48, 128, 48, &TestColors[7]);

    dispcolor_printf_Bg(40, 16, FONTID_32F, &TestColors[0], &bgcolor, "%d : %d", 2, 3);

    //dispcolor_printf_Bg(8, 53, FONTID_6X8M, &TestColors[7], &bgcolor, "%02d.%02d.%04d %02d:%02d:%02d", Day, Month, Year, Hour, Min, Sec);
    dispcolor_DrawRectangleFilled(0, 48, 127, 63, &TestColors[0]);
    dispcolor_printf_Bg(8, 53, FONTID_6X8M, &bgcolor, &TestColors[0], "%02d.%02d.%04d %02d:%02d:%02d", Day, Month, Year, Hour, Min, Sec);

    dispcolor_printf_Bg(3, 4, FONTID_6X8M, &TestColors[1], &bgcolor, "������� 1");
    dispcolor_printf_Bg(72, 4, FONTID_6X8M, &TestColors[2], &bgcolor, "������� 2");

    dispcolor_Update();

    TimeTick();
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
