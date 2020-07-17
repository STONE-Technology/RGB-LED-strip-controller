#include "../BOARD/ws2812/ws2812.h"
#include "usart.h"
#include "delay.h"

 uint8_t PIXEL_NUM=60; //����Ĭ�ϵƵĸ���

//����WS2812������
#define 	RGB_LED 	GPIO_Pin_7
#define		RGB_LED_HIGH	(GPIO_SetBits(GPIOA,RGB_LED))
#define 	RGB_LED_LOW		(GPIO_ResetBits(GPIOA,RGB_LED))

//����WS2812�����ų�ʼ��
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
void RGB_LED_Write0(void)//д0
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

void RGB_LED_Write1(void)//д1
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void RGB_LED_Reset(void) //��λWS2812
{
	RGB_LED_LOW;
	delay_us(80);
}

void RGB_LED_Write_Byte(uint8_t byte)//дһ���ֽ�
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

void RGB_LED_Write_24Bits(uint8_t red,uint8_t green,uint8_t blue)//д�����ֽ�
{
	uint16_t i=0;
	for( i=0;i<PIXEL_NUM;i++)
	{
		RGB_LED_Write_Byte(green);
		RGB_LED_Write_Byte(red);
		RGB_LED_Write_Byte(blue);
	}

}

void RGB_LED_Write_24Bits_Efect(uint8_t red,uint8_t green,uint8_t blue)//��˸ģʽ��ʹ��
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}


void RGB_LED_Red(void)//���ú������
{
	 uint8_t i;
		for(i=0;i<PIXEL_NUM;i++)
			{
				RGB_LED_Write_24Bits(0, 0xff, 0);
		}
}

void RGB_LED_Green(void)//�����̵�����
{
	uint8_t i;

	for(i=0;i<PIXEL_NUM;i++)
		{
			RGB_LED_Write_24Bits(0xff, 0, 0);
	}
}

void RGB_LED_Blue(void)//������������
{
	uint8_t i;

	for(i=0;i<PIXEL_NUM;i++)
		{
			RGB_LED_Write_24Bits(0x40, 0x50, 0);
	}
}
