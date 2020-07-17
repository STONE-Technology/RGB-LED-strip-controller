#include "stm32f10x.h"

#include "usart.h"
#include "delay.h"
#include "../BOARD/ws2812/ws2812.h"


/*
Define a structure
C_RED£ºRecord the value of RED LED
C_GREEN£ºRecord the value of GREEN LED
C_BLUE£ºRecord the value of BLUE LED
C_WHITE£ºRecord the value of WHITE LED
C_RED_FLAG£ºSolve the bug of the brightness issue while re-power on the LED
*/
struct RGB_COLOR
{
	u8 C_RED;
	u8 C_GREEN;
	u8 C_BLUE;
	u8 C_WHITE;
	
	u8 C_RED_FLAG;
	u8 C_GREEN_FLAG;
	u8 C_BLUE_FLAG;
};

#define ICON_WHITE_ADDR  0x02//Variable memory address of WHITE LED
#define ICON_RED_ADDR	   0x03//Variable memory address of RED LED
#define ICON_GREEN_ADDR  0x04//Variable memory address of GREEN LED
#define ICON_BLUE_ADDR   0x05//Variable memory address of BLUE LED

#define TEXT_RED_ADDR         0x07//Variable memory address of RED LED brightness value
#define TEXT_GREEN_ADDR       0x08//Variable memory address of GREEN LED brightness value
#define TEXT_BLUE_ADDR        0x09//Variable memory address of BLUE LED brightness value
#define TEXT_WHITE_ADDR       0x06//Variable memory address of WHITE LED brightness value

#define SWITCH_ONOFF_ADDR     0x01//Variable memory address of control the ON/OFF

#define ICON_ON          0x01//Variable memroy address of ON icon
#define ICON_OFF         0x00//Variable memroy address of OFF icon
u8 data_send[8]=  {0xA5, 0x5A, 0x05, 0x82, 0x00, 0x00, 0x00,0x00};//The data format sent by MCU

/*
Function sent by UART
send_array[]£ºDATA
num£ºDATA length
*/
void UART1_Send_Array(u8 send_array[],unsigned char num)
{
        u8 i=0;  
        while(i<num)
        {
                USART_SendData(USART1,send_array[i]);  
                while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
                i++;  
        }
}

/*
Main function
*/
int main(void)
{
  uart_init(115200); //Initiallize the baudrate as 115200
  delay_init(); //Delay the initiallization of function
	struct RGB_COLOR USER_RGB_COLOR;//Initiallize the RGB brightness value structure
	USER_RGB_COLOR.C_BLUE=0; //Initial brightness 0
	USER_RGB_COLOR.C_GREEN=0;
	USER_RGB_COLOR.C_RED=0;
	USER_RGB_COLOR.C_RED_FLAG=1;
	USER_RGB_COLOR.C_GREEN_FLAG=1;
	USER_RGB_COLOR.C_BLUE_FLAG=1;
	u16 k,q;//Count flag
	u8 BLINK_2=0;//Blink count
	u8 USER_R=0,USER_G=0,USER_B=0,COLOR_TYPE=0,COLOR_DIR=0;//Four blinking flags and brightness value
	u8 blink_type=0;//Record the single blink type
	u16 times=0; //Main function delay flag
	RGB_LED_Init(); //Initiallize the WS2812 RGB LED strip driver
	
 	while(1) //main loop
	{
		if(USART_RX_END)//Judging that serial port reception is complete
		{					   
			switch (USART_RX_BUF[5]) //Judging the data received is from which button
			{
				case 0x33: //LED number adjusting
					PIXEL_NUM=USART_RX_BUF[8];
					break;
				case LED_ALL_ONOFF: //LED ON/OFF
					blink_type=0;
					if(USART_RX_BUF[8]==0)//LED OFF
					{
						/*Clear the brightness and the icon of RED LED*/
						data_send[5]=ICON_RED_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_RED_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						
						/*Clear the brightness and the icon of GREEN LED*/
						data_send[5]=ICON_GREEN_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_GREEN_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						/*Clear the brightness and the icon of BLUE LED*/
						data_send[5]=ICON_BLUE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_BLUE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						
						USER_RGB_COLOR.C_BLUE=0;
						USER_RGB_COLOR.C_GREEN=0;
						USER_RGB_COLOR.C_RED=0;
						
						/*Clear the brightness and the icon of WHITE LED*/
						data_send[5]=ICON_WHITE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_WHITE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						USER_RGB_COLOR.C_WHITE=0;
					}
					else  //LED ON
					{
						/*Set the initial RGB value*/
						USER_RGB_COLOR.C_BLUE=0x32;
						USER_RGB_COLOR.C_GREEN=0x10;
						USER_RGB_COLOR.C_RED=0x24;
						USER_RGB_COLOR.C_RED_FLAG=0;
						USER_RGB_COLOR.C_GREEN_FLAG=0;
						USER_RGB_COLOR.C_BLUE_FLAG=0;
						
						/*MCU sending data to display, show the related icon*/
						data_send[5]=ICON_RED_ADDR;
						data_send[7]=ICON_ON;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_RED_ADDR;
						data_send[7]=0x24;
						UART1_Send_Array(data_send,8);
						data_send[5]=ICON_GREEN_ADDR;
						data_send[7]=ICON_ON;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_GREEN_ADDR;
						data_send[7]=0x10;
						UART1_Send_Array(data_send,8);
						data_send[5]=ICON_BLUE_ADDR;
						data_send[7]=ICON_ON;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_BLUE_ADDR;
						data_send[7]=0x32;
						UART1_Send_Array(data_send,8);
					}
					//Set LED brightness
					RGB_LED_Write_24Bits(USER_RGB_COLOR.C_RED, USER_RGB_COLOR.C_GREEN, USER_RGB_COLOR.C_BLUE);
					break;
				case RED_COLOR://Set RED LED brightness
					blink_type=0;
					if(USER_RGB_COLOR.C_RED_FLAG==1)
					{
						if(USART_RX_BUF[8]==0)
							break;
					}
					//Clear the WHITE LED icon
					data_send[5]=ICON_WHITE_ADDR;
					data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					
					//Clear the WHITE LED brightness value
					data_send[5]=TEXT_WHITE_ADDR;
					data_send[7]=0x00;
					UART1_Send_Array(data_send,8);
					USER_RGB_COLOR.C_WHITE=0;
					
					//Turn on ON/OFF icon
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
					
					//Turn on RED LED icon
					data_send[5]=ICON_RED_ADDR;
					if(USART_RX_BUF[8]>0)data_send[7]=ICON_ON;
					else data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					
					USER_RGB_COLOR.C_RED=USART_RX_BUF[8];
					USER_RGB_COLOR.C_RED_FLAG=0;
					if(USER_RGB_COLOR.C_RED==0)USER_RGB_COLOR.C_RED_FLAG=1;
					
					//If the value sent from display are all 0, turn off the ON/OFF icon
					if((USER_RGB_COLOR.C_RED==0x00)&&(USER_RGB_COLOR.C_GREEN==0x00)&&(USER_RGB_COLOR.C_BLUE==0x00)&&(USER_RGB_COLOR.C_WHITE==0x00))
					{
						data_send[5]=SWITCH_ONOFF_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
					}
					RGB_LED_Write_24Bits(USER_RGB_COLOR.C_RED, USER_RGB_COLOR.C_GREEN, USER_RGB_COLOR.C_BLUE); // Red
					
					break;
				case GREEN_COLOR://Set GREEN LED brightness
					blink_type=0;
					if(USER_RGB_COLOR.C_GREEN_FLAG==1)
					{
						if(USART_RX_BUF[8]==0)
							break;
					}
					//Turn on the GREEN LED icon
					data_send[5]=ICON_GREEN_ADDR;
					if(USART_RX_BUF[8]>0)data_send[7]=ICON_ON;
					else data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					
					//Turn off the WHITE LED icon
					data_send[5]=ICON_WHITE_ADDR;
					data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					data_send[5]=TEXT_WHITE_ADDR;
					data_send[7]=0x00;
					UART1_Send_Array(data_send,8);
					USER_RGB_COLOR.C_WHITE=0;
					
					//Turn on the ON icon
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
					
					USER_RGB_COLOR.C_GREEN=USART_RX_BUF[8];
					USER_RGB_COLOR.C_GREEN_FLAG=0;
					if(USER_RGB_COLOR.C_GREEN==0)USER_RGB_COLOR.C_GREEN_FLAG=1;
					
					if((USER_RGB_COLOR.C_RED==0x00)&&(USER_RGB_COLOR.C_GREEN==0x00)&&(USER_RGB_COLOR.C_BLUE==0x00)&&(USER_RGB_COLOR.C_WHITE==0x00))
					{
						data_send[5]=SWITCH_ONOFF_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
					}
					RGB_LED_Write_24Bits(USER_RGB_COLOR.C_RED, USER_RGB_COLOR.C_GREEN, USER_RGB_COLOR.C_BLUE);  // Green
					break;
				case BLUE_COLOR://Set the BLUE LED brightness
					blink_type=0;
					if(USER_RGB_COLOR.C_BLUE_FLAG==1)
					{
						if(USART_RX_BUF[8]==0)
							break;
					}
					data_send[5]=ICON_BLUE_ADDR;
					if(USART_RX_BUF[8]>0)data_send[7]=ICON_ON;
					else data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					
					data_send[5]=ICON_WHITE_ADDR;
					data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					data_send[5]=TEXT_WHITE_ADDR;
					data_send[7]=0x00;
					UART1_Send_Array(data_send,8);
					USER_RGB_COLOR.C_WHITE=0;
					
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
					
					USER_RGB_COLOR.C_BLUE=USART_RX_BUF[8];
					USER_RGB_COLOR.C_BLUE_FLAG=0;
					if(USER_RGB_COLOR.C_BLUE==0)USER_RGB_COLOR.C_BLUE_FLAG=1;
					
					if((USER_RGB_COLOR.C_RED==0x00)&&(USER_RGB_COLOR.C_GREEN==0x00)&&(USER_RGB_COLOR.C_BLUE==0x00)&&(USER_RGB_COLOR.C_WHITE==0x00))
					{
						data_send[5]=SWITCH_ONOFF_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
					}
					RGB_LED_Write_24Bits(USER_RGB_COLOR.C_RED, USER_RGB_COLOR.C_GREEN, USER_RGB_COLOR.C_BLUE); // Blue
					break;
				case WHITE_COLOR:	//Set WHITE LED brightness
					blink_type=0;
					data_send[5]=ICON_WHITE_ADDR;
					if(USART_RX_BUF[8]>0)data_send[7]=ICON_ON;
					else data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
				
					data_send[5]=ICON_RED_ADDR;
					data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
				
					data_send[5]=TEXT_RED_ADDR;
					data_send[7]=0x00;
					UART1_Send_Array(data_send,8);
				
					data_send[5]=ICON_GREEN_ADDR;
					data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					data_send[5]=TEXT_GREEN_ADDR;
					data_send[7]=0x00;
					UART1_Send_Array(data_send,8);
				
					data_send[5]=ICON_BLUE_ADDR;
					data_send[7]=ICON_OFF;
					UART1_Send_Array(data_send,8);
					data_send[5]=TEXT_BLUE_ADDR;
					data_send[7]=0x00;
					UART1_Send_Array(data_send,8);
					
					USER_RGB_COLOR.C_BLUE=0;
					USER_RGB_COLOR.C_GREEN=0;
					USER_RGB_COLOR.C_RED=0;
					USER_RGB_COLOR.C_RED_FLAG=1;
					USER_RGB_COLOR.C_GREEN_FLAG=1;
					USER_RGB_COLOR.C_BLUE_FLAG=1;
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
					USER_RGB_COLOR.C_WHITE=USART_RX_BUF[8];
					
					if((USER_RGB_COLOR.C_RED==0x00)&&(USER_RGB_COLOR.C_GREEN==0x00)&&(USER_RGB_COLOR.C_BLUE==0x00)&&(USER_RGB_COLOR.C_WHITE==0x00))
					{
						data_send[5]=SWITCH_ONOFF_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
					} 
					RGB_LED_Write_24Bits(USER_RGB_COLOR.C_WHITE, USER_RGB_COLOR.C_WHITE, USER_RGB_COLOR.C_WHITE);
					break;
				case BLINK1://Blink 1
					blink_type=1;//Recording the blink type, which will be used in the loop after
				
						//Turn off all the color icon, only ON/OFF icon remains
						data_send[5]=ICON_RED_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_RED_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_GREEN_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_GREEN_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_BLUE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_BLUE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						
						USER_RGB_COLOR.C_BLUE=0;
						USER_RGB_COLOR.C_GREEN=0;
						USER_RGB_COLOR.C_RED=0;

						data_send[5]=ICON_WHITE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_WHITE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						USER_RGB_COLOR.C_WHITE=0;
						
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
						
					break;
				case BLINK2://Blink 2
					blink_type=2;//Recording the blink type, which will be used in the loop after
				//Turn off all the color icon, only ON/OFF icon remains
						data_send[5]=ICON_RED_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_RED_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_GREEN_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_GREEN_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_BLUE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_BLUE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						
						USER_RGB_COLOR.C_BLUE=0;
						USER_RGB_COLOR.C_GREEN=0;
						USER_RGB_COLOR.C_RED=0;
						
						data_send[5]=ICON_WHITE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_WHITE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						USER_RGB_COLOR.C_WHITE=0;
						
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
						
					break;
					
				case BLINK3://Blink 3
					blink_type=3;//Recording the blink type, which will be used in the loop after
				//Turn off all the color icon, only ON/OFF icon remains
						data_send[5]=ICON_RED_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_RED_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_GREEN_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_GREEN_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_BLUE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_BLUE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						
						USER_RGB_COLOR.C_BLUE=0;
						USER_RGB_COLOR.C_GREEN=0;
						USER_RGB_COLOR.C_RED=0;

						data_send[5]=ICON_WHITE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_WHITE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						USER_RGB_COLOR.C_WHITE=0;
						
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
					break;
				case BLINK4://Blink 4
					blink_type=4;//Recording the blink type, which will be used in the loop after
				//Turn off all the color icon, only ON/OFF icon remains
						data_send[5]=ICON_RED_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_RED_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_GREEN_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_GREEN_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
					
						data_send[5]=ICON_BLUE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_BLUE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						
						USER_RGB_COLOR.C_BLUE=0;
						USER_RGB_COLOR.C_GREEN=0;
						USER_RGB_COLOR.C_RED=0;

						data_send[5]=ICON_WHITE_ADDR;
						data_send[7]=ICON_OFF;
						UART1_Send_Array(data_send,8);
						data_send[5]=TEXT_WHITE_ADDR;
						data_send[7]=0x00;
						UART1_Send_Array(data_send,8);
						USER_RGB_COLOR.C_WHITE=0;
						
					data_send[5]=SWITCH_ONOFF_ADDR;
					data_send[7]=ICON_ON;
					UART1_Send_Array(data_send,8);
					break;
				default:
					USART_RX_END=0;//Clear UART reception completion flag
					USART_RX_STA=0;//Clear the data received by UART
					break;
			}
			USART_RX_STA=0;//Clear UART reception completion flag
			USART_RX_END=0;//Clear the data received by UART
		}
		else //Enter blink mode
		{
			if(blink_type==1)//Function of blink type 1
			{
				times++;
				if(times>=14)
				{
					times=0;
					if(COLOR_DIR==0)
					{
						if(COLOR_TYPE==0)
						{
							USER_R++;
							USER_G=0;
							USER_B=0;
						}	
						else if(COLOR_TYPE==1)
						{
							USER_R=0;
							USER_G++;
							USER_B=0;
						}
						else if(COLOR_TYPE==2)
						{
							USER_R=0;
							USER_G=0;
							USER_B++;
						}
						else if(COLOR_TYPE==3)
						{
							USER_R++;
							USER_G++;
							USER_B=0;
						}
						else if(COLOR_TYPE==4)
						{
							USER_R=0;
							USER_G++;
							USER_B++;
						}
						else if(COLOR_TYPE==5)
						{
							USER_R++;
							USER_G=0;
							USER_B++;
						}
						if((USER_R>=250)||(USER_G>=250)||(USER_B>=250))
						{
							COLOR_DIR=1;
//							COLOR_TYPE++;
//							if(COLOR_TYPE>6)
//								COLOR_TYPE=0;
						}
					}
					else
					{
						if(COLOR_TYPE==0)
						{
							USER_R--;
							USER_G=0;
							USER_B=0;
						}	
						else if(COLOR_TYPE==1)
						{
							USER_R=0;
							USER_G--;
							USER_B=0;
						}
						else if(COLOR_TYPE==2)
						{
							USER_R=0;
							USER_G=0;
							USER_B--;
						}
						else if(COLOR_TYPE==3)
						{
							USER_R--;
							USER_G--;
							USER_B=0;
						}
						else if(COLOR_TYPE==4)
						{
							USER_R=0;
							USER_G--;
							USER_B--;
						}
						else if(COLOR_TYPE==5)
						{
							USER_R--;
							USER_G=0;
							USER_B--;
						}
						if((USER_R==0x02)||(USER_G==0x02)||(USER_B==0x02))
						{
							COLOR_DIR=0;
							COLOR_TYPE++;
							if(COLOR_TYPE>5)
								COLOR_TYPE=0;
						}
					}
					RGB_LED_Write_24Bits(USER_R,USER_G,USER_B);
				}
				delay_ms(1);
			}
			else if(blink_type==2)//Blink type 2 function
			{
				k++;
				if(k>=150)
				{
					k=0;
					q=200;
					{
						BLINK_2++;
						if(BLINK_2>8)BLINK_2=0;
					}			
						if(BLINK_2==0)
							RGB_LED_Write_24Bits(q,0,0);
						else if(BLINK_2==1)
							RGB_LED_Write_24Bits(0,q,0);
						else if(BLINK_2==2)
							RGB_LED_Write_24Bits(0,0,q);
						else if(BLINK_2==3)
							RGB_LED_Write_24Bits(q,q,0);
						else if(BLINK_2==4)
							RGB_LED_Write_24Bits(0,q,q);
						else if(BLINK_2==5)
							RGB_LED_Write_24Bits(q,0,q); 
						else if(BLINK_2==6)
							RGB_LED_Write_24Bits(q-100,q,0);
						else if(BLINK_2==7)
							RGB_LED_Write_24Bits(0,q-80,q);
						else if(BLINK_2==8)
							RGB_LED_Write_24Bits(q,0,q-120); 
						else if(BLINK_2==9)
							RGB_LED_Write_24Bits(40,q-100,q-70);
						else if(BLINK_2==10)
							RGB_LED_Write_24Bits(q,100,q-80); 
					
				}
				delay_ms(1);
			}
			else if(blink_type==3)//Blink type 3 function
			{
				k++;
				if(k>=1000)
				{
					k=0;
					{
						BLINK_2++;
						if(BLINK_2>5)BLINK_2=0;
					}			
					{
						if(BLINK_2==0)
							RGB_LED_Write_24Bits(q,0,0);
						else if(BLINK_2==1)
							RGB_LED_Write_24Bits(0,q,0);
						else if(BLINK_2==2)
							RGB_LED_Write_24Bits(0,0,q);
						else if(BLINK_2==3)
							RGB_LED_Write_24Bits(q,q,0);
						else if(BLINK_2==4)
							RGB_LED_Write_24Bits(0,q,q);
						else if(BLINK_2==5)
							RGB_LED_Write_24Bits(q,0,q); 
					}
				}
				delay_ms(1);
			}
			else if(blink_type==4)//Blink type 4 function
			{
				k++;
				if(k>=500)
				{
						k=0;
						q=0;
						BLINK_2++;
						if(BLINK_2>5)BLINK_2=0;
				}		
				q++;
				if(q>=250)q=0;
						if(BLINK_2==0)
							RGB_LED_Write_24Bits(q,0,0);
						else if(BLINK_2==1)
							RGB_LED_Write_24Bits(0,q,0);
						else if(BLINK_2==2)
							RGB_LED_Write_24Bits(0,0,q);
						else if(BLINK_2==3)
							RGB_LED_Write_24Bits(q,q,0);
						else if(BLINK_2==4)
							RGB_LED_Write_24Bits(0,q,q);
						else if(BLINK_2==5)
							RGB_LED_Write_24Bits(q,0,q); 
				delay_ms(1);
			}
			else //Non blink mode, means the system is not in the blink mode, no execution
			{
			//You may enter the related judging or execution here for the UART receiving failure
					
			}
		}
	}	 
}
