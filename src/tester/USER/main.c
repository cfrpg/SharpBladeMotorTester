#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "timer.h"
#include "keyboard.h"

u16 tick[3]={0,0,0};

int main(void)
{
	s16 i;
	u8 key;
	delay_init(168);
	uart_init(115200);
	MainClockInit();
	LEDInit();
	OledInit();
	KeyInit();
	
	LEDSet(1);
	OledClear(0x00);
	OledDispString(8,4,"HELLO",0);
	OledDispString(6,7,"STM32F407",0);
	OledDispString(7,12,"FPS:219",0);
	OledDispString(0,15,"OK                YES",0);
	while(1)
	{	
		if(tick[0]>500)
		{
			tick[0]=0;
			LEDFlip();
		}
		if(tick[1]>500)
		{
			tick[1]=0;
			for(i=0;i<8;i++)
			{
				printf("%d",key&0x01);
				key>>=1;
			}
			printf("\r\n%d\r\n",WheelGetValue());
		}
		if(tick[2]>500)
		{
			tick[2]=0;
			key=GPGetData();
			
		}
	}
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
		u8 n=3;
		while(n--)
		{
			tick[n]++;
		}				
	}
}

