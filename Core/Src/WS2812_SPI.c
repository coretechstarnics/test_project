/*
 * WS2812_SPI.c
 *
 *  Created on: Sep 4, 2023
 *      Author: controllerstech
 */


#include "main.h"
#include "WS2812_SPI.h"
#include "led_conf.h"


extern rgb_color       rLed_Data[];

extern SPI_HandleTypeDef hspi1;

#define USE_BRIGHTNESS 1
extern int brightness;


void ws2812_spi (int GREEN, int RED, int BLUE)
{
#if USE_BRIGHTNESS
	if (brightness>100)brightness = 100;
	GREEN = GREEN*brightness/100;
	RED = RED*brightness/100;
	BLUE = BLUE*brightness/100;
#endif
	uint32_t color = GREEN<<16 | RED<<8 | BLUE;
	uint8_t sendData[24];
	int indx = 0;

	uint8_t *ucpSendData;					// variable pointer
	ucpSendData = &sendData[0];				// point to 1st variable of sendData


	for (int i=23; i>=0; i--)
	{
		if (((color>>i)&0x01) == 1) 
		{
			sendData[indx++] = 0x1E;  	// store 1
		}
		else 
		{
			sendData[indx++] = 0x0C;  						// store 0
		}
	}
	HAL_SPI_Transmit(&hspi1, ucpSendData, 24, 1000);
}



void WS2812_Send  (void)
{
	for (int i=0; i<MAX_NUMB; i++)
	{
		ws2812_spi(rLed_Data[i].green, rLed_Data[i].red, rLed_Data[i].blue);
	}
	HAL_Delay (1);
}

