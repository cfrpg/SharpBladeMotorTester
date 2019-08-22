#include "eo.h"
#include "pages.h"
#include "delay.h"
u32 lastCCR;
u32 EOsum;
u32 EOcnt;

void EOInit(void)
{
	TIM_TimeBaseInitTypeDef  ti;
	TIM_ICInitTypeDef tc;
	NVIC_InitTypeDef ni;
	GPIO_InitTypeDef gi;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	gi.GPIO_Pin=GPIO_Pin_10;
	gi.GPIO_Mode=GPIO_Mode_AF;
	gi.GPIO_Speed=GPIO_Speed_100MHz;
	gi.GPIO_PuPd=GPIO_PuPd_UP;	
	GPIO_Init(GPIOB,&gi);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	
	ti.TIM_Period=0xFFFFFFFF;
	ti.TIM_Prescaler=84-1;
	ti.TIM_CounterMode=TIM_CounterMode_Up;
	ti.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&ti);
	
	tc.TIM_Channel=TIM_Channel_3;
	tc.TIM_ICPolarity=TIM_ICPolarity_Falling;
	tc.TIM_ICSelection=TIM_ICSelection_DirectTI;
	tc.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	tc.TIM_ICFilter=0x0F;
	TIM_ICInit(TIM2,&tc);
	
	ni.NVIC_IRQChannel=TIM2_IRQn;
	ni.NVIC_IRQChannelPreemptionPriority=1;
	ni.NVIC_IRQChannelSubPriority=2;
	ni.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&ni);
	
	TIM_ITConfig(TIM2,TIM_IT_CC3,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
	lastCCR=0;
	EOcnt=0;
	TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);

}

void TIM2_IRQHandler(void)
{
	u32 ccr,val;
	if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET)
	{
		//printf("TIM2CC\r\n");
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);
		ccr=TIM_GetCapture3(TIM2);	
		if(ccr>=lastCCR)
		{
			val=ccr-lastCCR;
		}
		else
		{
			val=(0xFFFFFFFF-ccr)+lastCCR;
		}		
		lastCCR=ccr;
		EOsum+=val;
		EOcnt++;
		if(EOcnt>=4)
		{
			sys.EOtime=EOsum;
			EOsum=0;
			EOcnt=0;			
		}
		//TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);
	}
	
	
}
