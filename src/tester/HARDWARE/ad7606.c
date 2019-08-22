#include "ad7606.h"
#include "stdio.h"
#include "delay.h"
 
void AD7606Init(void)
{
	GPIO_InitTypeDef gi;
	SPI_InitTypeDef si;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);	
	
	//CS,RST
	gi.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	gi.GPIO_Mode=GPIO_Mode_OUT;
	gi.GPIO_OType=GPIO_OType_PP;
	gi.GPIO_Speed=GPIO_Speed_100MHz;
	gi.GPIO_PuPd=GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOD,&gi);
	
	//CV
	gi.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOC,&gi);
		
	//BUSY
	gi.GPIO_Pin=GPIO_Pin_0;
	gi.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOD,&gi);
	
	//SPI3 SCK->PC10,MISO->C11
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	gi.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	gi.GPIO_Mode=GPIO_Mode_AF;
	gi.GPIO_OType=GPIO_OType_PP;
	gi.GPIO_Speed=GPIO_Speed_100MHz;
	gi.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&gi);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3);
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);
	
	si.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	si.SPI_Mode=SPI_Mode_Master;
	si.SPI_DataSize=SPI_DataSize_8b;
	si.SPI_CPOL=SPI_CPOL_High;
	si.SPI_CPHA=SPI_CPHA_2Edge;
	si.SPI_NSS=SPI_NSS_Soft;
	si.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
	si.SPI_FirstBit=SPI_FirstBit_MSB;
	si.SPI_CRCPolynomial=7;
	SPI_Init(SPI3,&si);
	SPI_Cmd(SPI3,ENABLE);
	AD7606Reset();
	ADI_CV=1;
}

u8 ADRead(void)
{
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);	
	SPI_I2S_SendData(SPI3,0x0f);
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);	
	u8 b=SPI_I2S_ReceiveData(SPI3);
	
	return b;
}

void ADRead2(u8* ptr)
{
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);	
	SPI_I2S_SendData(SPI3,0x0f);
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);	
	*ptr=SPI_I2S_ReceiveData(SPI3);
}



void AD7606Reset(void)
{
	ADI_RST=0;
	ADI_RST=1;
	ADI_RST=1;
	ADI_RST=1;
	ADI_RST=1;
	ADI_RST=0;
}

void AD7606Start(void)
{
	ADI_CV=0;
	ADI_CV=0;
	ADI_CV=0;
	//delay_us(1);
	ADI_CV=1;
}

void AD7606Read(s16* data)
{
	u8 i;
	AD7606Start();	
	while(ADI_BUSY!=0);	
	ADI_CS=0;
	for(i=0;i<8;i++)
	{
		data[i]=ADRead();
		data[i]<<=8;
		data[i]|=ADRead();
	}
	ADI_CS=1;
	
}
