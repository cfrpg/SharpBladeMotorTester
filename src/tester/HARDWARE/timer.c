#include "timer.h"

void MainClockInit(void)
{				
	TIM_TimeBaseInitTypeDef  ti;
	NVIC_InitTypeDef ni;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	ti.TIM_Period = 9; //1ms
	ti.TIM_Prescaler =8400-1;
	ti.TIM_ClockDivision = TIM_CKD_DIV1; 
	ti.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &ti);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 
	TIM_Cmd(TIM3, ENABLE);
	ni.NVIC_IRQChannel = TIM3_IRQn;
	ni.NVIC_IRQChannelPreemptionPriority = 1; 
	ni.NVIC_IRQChannelSubPriority = 2;
	ni.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&ni);	
}
