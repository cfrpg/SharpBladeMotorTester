#include "pages.h"


void PagesInit(void)
{	
	sys.adcEn=0;
	sys.pwm[0]=0;
	sys.pwm[1]=0;
	sys.pwm[2]=0;
	sys.pwm[3]=0;
	
	sys.sensors.header.stx=LINKSTX;
	sys.sensors.header.len=DataLen[SENSOR_DATA];
	sys.sensors.header.fun=SENSOR_DATA;	
	
	PageInit_main(1);
	PageInit_ADC(1);
	PageInit_Motor(1);
	currpage=-1;
}

void PagesChangeTo(u8 p)
{	
	if(p!=currpage)
	{
		currpage=p;
		switch(p)
		{
			case MainPage:
				PageInit_main(0);
			break;
			case ADCPage:
				PageInit_ADC(0);
			break;
			case MotorPage:
				PageInit_Motor(0);
			break;
			default:
				currpage=MainPage;
				PageInit_main(0);
			break;
		}
	}
}

void PagesNext(s8 d)
{	
	if(d<0)
	{
		if(currpage==0)
			PagesChangeTo(PageNum-1);
		else
			PagesChangeTo(currpage-1);
	}
	if(d>0)
		PagesChangeTo((currpage+1)%PageNum);
}

void PagesUpdate(void)
{	
	switch(currpage)
	{
		case MainPage:
			PageUpdate_main();
		break;
		case ADCPage:
			PageUpdate_ADC();
		break;
		case MotorPage:
			PageUpdate_Motor();
		break;
		default:
			currpage=MainPage;
			PageInit_main(0);
			PageUpdate_main();
		break;
	}
}

void PagesDrawHeader(u8 n,s8 *name)
{
	OledDispString(0,0,"=",0);
	OledDispString(19,0,"=",0);
	OledDispString(4,0,"/",0);
	OledDispInt(5,0,PageNum,0,0);
	if(n>8)
		OledDispInt(2,0,n+1,0,0);
	else
		OledDispInt(3,0,n+1,0,0);
	OledDispString(8,0,name,0);
}
