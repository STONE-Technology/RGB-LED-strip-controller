#include "../BOARD/ws2812/ws2812.h"
#include "usart.h"
#include "delay.h"

 uint8_t PIXEL_NUM=60; //设置默认灯的个数

//驱动WS2812的引脚
#define 	RGB_LED 	GPIO_Pin_7
#define		RGB_LED_HIGH	(GPIO_SetBits(GPIOA,RGB_LED))
#define 	RGB_LED_LOW		(GPIO_ResetBits(GPIOA,RGB_LED))

//驱动WS2812的引脚初始化
void RGB_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOA,GPIO_Pin_7);	
}

/********************************************************/
//
/********************************************************/
void RGB_LED_Write0(void)//写0
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

/********************************************************/
//
/********************************************************/

void RGB_LED_Write1(void)//写1
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void RGB_LED_Reset(void) //复位WS2812
{
	RGB_LED_LOW;
	delay_us(80);
}

void RGB_LED_Write_Byte(uint8_t byte)//写一个字节
{
	uint8_t i;

	for(i=0;i<8;i++)
		{
			if(byte&0x80)
				{
					RGB_LED_Write1();
			}
			else
				{
					RGB_LED_Write0();
			}
		byte <<= 1;
	}
}

void RGB_LED_Write_24Bits(uint8_t red,uint8_t green,uint8_t blue)//写三个字节
{
	uint16_t i=0;
	for( i=0;i<PIXEL_NUM;i++)
	{
		RGB_LED_Write_Byte(green);
		RGB_LED_Write_Byte(red);
		RGB_LED_Write_Byte(blue);
	}

}

void RGB_LED_Write_24Bits_Efect(uint8_t red,uint8_t green,uint8_t blue)//闪烁模式下使用
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}


void RGB_LED_Red(void)//设置红灯亮度
{
	 uint8_t i;
		for(i=0;i<PIXEL_NUM;i++)
			{
				RGB_LED_Write_24Bits(0, 0xff, 0);
		}
}

void RGB_LED_Green(void)//设置绿灯亮度
{
	uint8_t i;

	for(i=0;i<PIXEL_NUM;i++)
		{
			RGB_LED_Write_24Bits(0xff, 0, 0);
	}
}

void RGB_LED_Blue(void)//设置蓝灯亮度
{
	uint8_t i;

	for(i=0;i<PIXEL_NUM;i++)
		{
			RGB_LED_Write_24Bits(0x40, 0x50, 0);
	}
}
