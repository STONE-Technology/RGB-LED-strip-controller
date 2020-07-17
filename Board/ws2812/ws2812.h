#ifndef __WS2812_H
#define __WS2812_H

#include "stm32f10x.h"

//#define PIXEL_NUM 120
extern uint8_t PIXEL_NUM;
#define WS_HIGH 0XF8
#define WS_LOW  0XE0


#define RED_COLOR   	 0x07
#define GREEN_COLOR  	 0x08
#define BLUE_COLOR 		 0x09
#define WHITE_COLOR 	 0x06
#define LED_ALL_ONOFF  0x01

#define BLINK1         0x0A        
#define BLINK2         0x0B  
#define BLINK3         0x0C  
#define BLINK4         0x0D  
#define LightOn      0x00
#define LightOff     0x01
void RGB_LED_Reset(void);
void RGB_LED_Init(void);
void RGB_LED_Reset(void);
void RGB_LED_Write_24Bits(uint8_t red,uint8_t green,uint8_t blue);
void RGB_LED_Write_24Bits_effect(uint8_t red,uint8_t green,uint8_t blue);
uint32_t ws281x_wheel(uint8_t wheelPos);
void RGB_LED_Write_24Bits_Efect(uint8_t green,uint8_t red,uint8_t blue);
#endif /* __WS2812_H */

