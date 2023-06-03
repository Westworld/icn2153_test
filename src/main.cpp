#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_spi_flash.h"
#include "spiram.h" //"esp_spiram.h"
#include "tests.h"
#include "dispcolor.h" //"display/dispcolor.h"
#include "icn2053.h"  // "icn2053/icn2053.h"
//#include "rmt/rmt.h"

#define SW_VERSION_MAJOR	1
#define SW_VERSION_MINOR	0


// –‡ÁÂ¯ÂÌËÂ ˝Í‡Ì‡
#define dispWidth 				128
#define dispHeight				64

// max 6 FPS, der refresh job benötigt 1800 ms für 10 Aufrufe


void ledmatrix_task(void* arg)
{
  
  Serial.begin(460800);
  long timetotal=0, timestart=0;
  int timecounter=0;


	while (1)
    {
     // timestart = millis();
    	icn2053_refresh();
    /*
      timetotal += (millis()-timestart);
      if (timecounter++>10) {
        Serial.println(timetotal);
        timecounter=0;
        timetotal=0;

      }
      */
      yield();
    }
}

void main_task(void* arg)
{
	while (1)
    {
      
   //   PixelTest();


		ScanTest();
    delay(1000);
		RefreshTest();
    delay(1000);
    
		TextTest1();
    delay(1000);    
	
  TextTest2();
    delay(1000);    
		TextTest3();
    delay(1000);    
		TextTest4();
    delay(1000);    

		GradientTest();
    delay(1000);
		RainbowTestInit();

			RainbowTest();
    delay(1000);
    }
}

void setup() {
  	dispcolor_Init(dispWidth, dispHeight);
/*
    xTaskCreatePinnedToCore(main_task, "Main", 2048, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(ledmatrix_task, "LedMatrix", 2048, NULL, 0, NULL, 1);
*/
/*
	while (1)
    {
	    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    */


       xTaskCreatePinnedToCore(main_task, "Main", 2048, NULL, 9, NULL, 0);
    xTaskCreatePinnedToCore(ledmatrix_task, "LedMatrix", 2048, NULL, 8, NULL, 1);

	while (1)
    {
	    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}