#include "pages.h"
#include "ads1256.h"
struct
{	
	u8 state;	
} adcp;

void PageInit_ADC(u8 f)
{
	u8 i;
	if(f)
	{
		adcp.state=0;		
		return;
	}
	OledClear(0);
	PagesDrawHeader(ADCPage,"ADC");
	for(i=0;i<8;i++)
	{
		OledDispString(0,i+2,"Ch",0);
		OledDispInt(2,i+2,i,1,0);
	}
	OledDispString(0,13,"CPU:",0);
	OledDispString(0,15,"RAW",0);
	OledDispString(15,15,"SCALED",0);
	
}

void PageUpdate_ADC(void)
{
	u8 i,j;
	u8 key=currKey&(currKey^lastKey);
	u16 cpu=cpucnt;
//	for(i=0;i<8;i++)
//	{
//		sys.rawData[i]=0;
//	}
//	for(j=0;j<8;j++)
//	{		
//		for(i=0;i<8;i++)
//		{
//			sys.rawData[i]+=ADSReadChannel(i);			
//		}
//	}
//	for(i=0;i<8;i++)
//	{
//		sys.rawData[i]>>=3;
//		sys.data[i]=((float)sys.rawData[i])/(1<<23)*5;
//	}
//	
	if(key&KEY_A)
		adcp.state=0;
	if(key&KEY_C)
		adcp.state=1;
	for(i=0;i<8;i++)
	{
		if(adcp.state==0)
		{
			OledDispInt(4,i+2,sys.ADCRawData[i],11,0);
		}
		else
		{
			OledDispFixed(4,i+2,(s32)(sys.ADCData[i]*1e6),6,11,0);
		}
	}
	cpu=(cpucnt+10000-cpu)%10000;
	OledDispInt(4,13,cpu,4,0);
	if(key&KEY_LEFT)
		PagesNext(-1);
	if(key&KEY_RIGHT)
		PagesNext(1);
}
