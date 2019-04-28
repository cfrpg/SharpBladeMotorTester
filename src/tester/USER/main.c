#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "timer.h"
#include "keyboard.h"
#include "parameter.h"
#include "pages.h"
#include "ads1256.h"
#include "eo.h"

u16 tick[3]={0,0,0};
u16 cpucnt=0;
u8 lastKey=0;
u8 currKey=0;
s32 lastWheel=0;
s32 currWheel=0;

u8 currpage;
systemState sys;

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
	ADS1256Init();
	EOInit();
		
	OledClear(0x00);
	OledDispString(0,0,"== SB Motor Tester ==",0);
	OledDispString(5,4,"Loading...",0);
	printf("Read params.\r\n");
	ParamRead();
	printf("Read complete.\r\n");
	
	if(params.headFlag!=0xCFCFCFCF||params.tailFlag!=0xFCFCFCFC)
	{
		ParamReset();		
		FLASH_Status fs=ParamWrite();
		if(fs==FLASH_COMPLETE)
			printf("Reset parameters.\r\n");
		else
		{
			printf("Failed to write parameters:%d\r\n",fs);
			OledDispString(0,6,"Cannot setup params!",0);
			while(1);
		}
	}
	else
	{
		printf("Parameters Loaded.\r\n");
		printf("Version:%d.%d\r\n",params.version>>16,params.version&0x0000FFFF);
	}
	while(tick[0]<200);
	LEDSet(1);
	OledClear(0x00);	
	ADSStartUp(0,ADS1256_7500SPS);
	
	lastKey=0xFF;
	currpage=0;
	PagesInit();
	PagesChangeTo(MainPage);
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
		}
		if(tick[2]>50)
		{
			tick[2]=0;
			UpdateSensors();
			currKey=GPGetData();
			currWheel=WheelGetValue();
			PagesUpdate();			
			lastKey=currKey;
			lastWheel=currWheel;
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
		cpucnt++;
		if(cpucnt>10000)
			cpucnt=0;
	}
}


