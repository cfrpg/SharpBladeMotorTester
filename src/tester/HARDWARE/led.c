#include "led.h"

u8 _led_state;
void LEDInit(void)
{
	GPIO_InitTypeDef gi;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	gi.GPIO_Pin=GPIO_Pin_0;
	gi.GPIO_Mode=GPIO_Mode_OUT;
	gi.GPIO_OType=GPIO_OType_PP;
	gi.GPIO_Speed=GPIO_Speed_100MHz;
	gi.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&gi);
	_led_state=0;
}

void LEDSet(u8 v)
{
	if(v)
	{
		_led_state=0;		
	}
	else
	{
		_led_state=1;		
	}
	PBout(0)=_led_state;
}

void LEDFlip(void)
{
	_led_state^=1;
	PBout(0)=_led_state;
}
