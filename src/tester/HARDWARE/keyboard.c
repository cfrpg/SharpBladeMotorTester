#include "keyboard.h"

s32 value;

void KeyInit(void)
{
	GPIO_InitTypeDef gi;
	EXTI_InitTypeDef ei;
	NVIC_InitTypeDef ni;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	gi.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	gi.GPIO_Mode=GPIO_Mode_OUT;
	gi.GPIO_OType=GPIO_OType_PP;
	gi.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gi.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&gi);
	
	gi.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_12;
	gi.GPIO_Mode=GPIO_Mode_IN;
	gi.GPIO_Speed=GPIO_Speed_100MHz;
	gi.GPIO_PuPd=GPIO_PuPd_UP;	
	GPIO_Init(GPIOD,&gi);
	
	gi.GPIO_Pin=GPIO_Pin_13;
	gi.GPIO_PuPd=GPIO_PuPd_UP;	
	GPIO_Init(GPIOD,&gi);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,EXTI_PinSource13);
	
	ei.EXTI_Line=EXTI_Line13;
	ei.EXTI_Mode=EXTI_Mode_Interrupt;
	ei.EXTI_Trigger=EXTI_Trigger_Falling;
	ei.EXTI_LineCmd=ENABLE;
	EXTI_Init(&ei);
	
	ni.NVIC_IRQChannel=EXTI15_10_IRQn;
	ni.NVIC_IRQChannelCmd=ENABLE;
	ni.NVIC_IRQChannelPreemptionPriority=2;
	ni.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&ni);
	
	GPSHIFT=0;
	value=0;
}

u8 GPGetData()
{
	u8 res,i;	
	GPCLK=0;
	res=0;	
	GPSHIFT=0;
	res=0;
	GPSHIFT=1;//锁存	
	//delay_us(1);
	for(i=0;i<8;i++)
	{	
		//delay_us(1);	
		__nop();
		res<<=1;
		GPCLK=0;
		//delay_us(1);
		res|=GPQ;
		//delay_us(1);
		GPCLK=1;//上升沿移位
		
	}
	//delay_us(1);
	GPSHIFT=0;
	GPCLK=0;
	printf("key %d\r\n",res);
	return res;
}

void EXTI15_10_IRQHandler(void)
{
	if(WHEEL)
	{
		value++;
	}
	else
	{
		value--;
	}
	EXTI_ClearITPendingBit(EXTI_Line13);
}

s32 WheelGetValue(void)
{
	return value;
}
