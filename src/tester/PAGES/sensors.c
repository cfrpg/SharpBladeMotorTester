#include "pages.h"
#include "ads1256.h"

u32 EOcnt;

void UpdateADC(void)
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		sys.ADCRawData[i]=0;
	}
	for(j=0;j<8;j++)
	{		
		for(i=0;i<8;i++)
		{
			sys.ADCRawData[i]+=ADSReadChannel(i);			
		}
	}
	for(i=0;i<8;i++)
	{
		sys.ADCRawData[i]>>=3;
		sys.ADCData[i]=((float)sys.ADCRawData[i])/(1<<23)*5;
	}
	
}

void UpdateSensors(void)
{
	if(sys.adcEn)
		UpdateADC();
}

void TIM8_BRK_TIM12_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)
	{		
		TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
		sys.rpm=EOcnt*60/4;
		//printf("EO %d\r\n",EOcnt);
		EOcnt=0;
		
	}
}

void EXTI4_IRQHandler(void)
{
	//EXTI_ClearITPendingBit(EXTI_Line4);
	delay_us(20);
	if(PDin(4)==0)
	{
		delay_us(20);
		if(PDin(4)==0)
		{
			EOcnt++;
			//printf("EO %d\r\n",EOcnt);	
		}
		
	}
	
	EXTI_ClearITPendingBit(EXTI_Line4);
}

