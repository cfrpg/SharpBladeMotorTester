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
//	ADSReadAllChannel(sys.ADCRawData);
//	for(i=0;i<8;i++)
//	{
//		sys.ADCRawData[i]>>=0;
//		sys.sensors.ADCData[i]=((float)sys.ADCRawData[i])/(1<<23)*5;
//	}
	
	
	
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
			OledDispFixed(4,i+2,(s32)(sys.sensors.ADCData[i]*1e6),6,11,0);
		}
	}
	cpu=(cpucnt+10000-cpu)%10000;
	OledDispInt(4,13,ADSCpu2,4,0);
	if(key&KEY_LEFT)
		PagesNext(-1);
	if(key&KEY_RIGHT)
		PagesNext(1);
}
