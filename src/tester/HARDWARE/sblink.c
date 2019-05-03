#include "sblink.h"
#include "string.h"
#include "stdio.h"

u8 sendBuff[SENDBUFFSIZE];
u8 linkSeq;

void LinkInit(void)
{
	GPIO_InitTypeDef gi;
	USART_InitTypeDef ui;	
	DMA_InitTypeDef di;
	NVIC_InitTypeDef ni;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART1);
	
	gi.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	gi.GPIO_Mode = GPIO_Mode_AF;
	gi.GPIO_Speed = GPIO_Speed_50MHz;
	gi.GPIO_OType = GPIO_OType_PP;
	gi.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&gi);

	ui.USART_BaudRate = 115200;
	ui.USART_WordLength = USART_WordLength_8b;
	ui.USART_StopBits = USART_StopBits_1;
	ui.USART_Parity = USART_Parity_No;
	ui.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	ui.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &ui);
	
	USART_Cmd(USART2, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Stream6);
	
	while(DMA_GetCmdStatus(DMA1_Stream6)!=DISABLE);
	
	di.DMA_Channel = DMA_Channel_4;
	di.DMA_PeripheralBaseAddr = (u32)&USART2->DR;
	di.DMA_Memory0BaseAddr = (u32)sendBuff;
	di.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	di.DMA_BufferSize = 0;
	di.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	di.DMA_MemoryInc = DMA_MemoryInc_Enable;
	di.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	di.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	di.DMA_Mode = DMA_Mode_Normal;
	di.DMA_Priority = DMA_Priority_Medium;
	di.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	di.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	di.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	di.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &di);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
	ni.NVIC_IRQChannel=DMA1_Stream6_IRQn;
	ni.NVIC_IRQChannelPreemptionPriority=1;
	ni.NVIC_IRQChannelSubPriority=3;
	ni.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&ni);
	
	linkSeq=0;
}

void LinkSendData(void* buff,u8 len)
{
	u8 i,sum=0;
	if(buff)
		memcpy(sendBuff,buff,len);
	sendBuff[2]=linkSeq;
	for(i=0;i<len;i++)
	{
		sum^=sendBuff[i];
	}
	sendBuff[len]=sum;
	
	linkSeq++;	
	//printf("%d\r\n",len);
	while(DMA_GetCurrDataCounter(DMA1_Stream6));		
	DMA_Cmd(DMA1_Stream6,DISABLE);	
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);	
	DMA_SetCurrDataCounter(DMA1_Stream6,len+1);
	DMA_Cmd(DMA1_Stream6,ENABLE);
	
}

void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_IT_TCIF6)==SET)
	{
		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_DMEIF6)!=RESET)
		{
			DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_DMEIF6);
		}
		DMA_ClearFlag(DMA1_Stream6,DMA_IT_TCIF6);
	}
		
}
