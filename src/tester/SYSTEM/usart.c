#include "sys.h"
#include "usart.h"	

#if 1
#pragma import(__use_no_semihosting)             
            
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
 
_sys_exit(int x) 
{ 
	x = x; 
} 
 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

void uart_init(u32 baud)
{   
	GPIO_InitTypeDef gi;
	USART_InitTypeDef ui;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	gi.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gi.GPIO_Mode = GPIO_Mode_AF;
	gi.GPIO_Speed = GPIO_Speed_50MHz;
	gi.GPIO_OType = GPIO_OType_PP;
	gi.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&gi);

	ui.USART_BaudRate = baud;
	ui.USART_WordLength = USART_WordLength_8b;
	ui.USART_StopBits = USART_StopBits_1;
	ui.USART_Parity = USART_Parity_No;
	ui.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	ui.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &ui);
	
	USART_Cmd(USART1, ENABLE);
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
		
}
