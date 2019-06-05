#include "pages.h"
#include "ads1256.h"

#define CURR_F		15.3413686
#define VOL_F		3.0
#define TORQUE_F	0.1

#define CURR_CH		1
#define	VOL_CH		3
#define	TORQUE_CH	5


struct
{
	u8 state;
	u8 disp;
} mainp;
void PageInit_main(u8 f)
{
	if(f)
	{
		mainp.state=0;
		mainp.disp=0;
		return;
	}
	OledClear(0);
	PagesDrawHeader(MainPage,"Main");
	OledDispString(0,2,"THRO:",0);
	OledDispString(0,3,"RPM :",0);
	OledDispString(0,4,"VOLT:",0);
	OledDispString(0,5,"CURR:",0);
	OledDispString(0,6,"TORQ:",0);
	OledDispString(0,8,"POWER",0);
	OledDispString(0,9,"IN :",0);
	OledDispString(0,10,"OUT:",0);
	OledDispString(0,11,"EFF:",0);
	OledDispString(0,13,"ADC CYCLE:",0);

	OledDispString(17,15,"DISP",0);
	OledDispString(9,15,"RST",0);
}

void PageUpdate_main(void)
{
	u8 key=currKey&(currKey^lastKey);
	float in,out;
	OledDispFixed(5,2,sys.pwm[0],1,5,0);
	OledDispInt(5,3,sys.rpm,7,0);
	OledDispInt(10,13,ADSCpu2,7,0);
	if(mainp.disp)
	{
		OledDispFixed(5,4,(s32)(sys.sensors.ADCData[VOL_CH]*1e6),6,11,0);
		OledDispFixed(5,5,(s32)(sys.sensors.ADCData[CURR_CH]*1e6),6,11,0);
		OledDispFixed(5,6,(s32)(sys.sensors.ADCData[TORQUE_CH]*1e6),6,11,0);
	}
	else
	{
		OledDispFixed(5,4,(s32)(sys.sensors.ADCData[VOL_CH]*1e6*VOL_F),6,11,0);
		OledDispFixed(5,5,(s32)(sys.sensors.ADCData[CURR_CH]*1e6*CURR_F),6,11,0);
		OledDispFixed(5,6,(s32)(sys.sensors.ADCData[TORQUE_CH]*1e6*TORQUE_F),6,11,0);
	}
	in=sys.sensors.ADCData[VOL_CH]*VOL_F*sys.sensors.ADCData[CURR_CH]*CURR_F;
	out=sys.sensors.ADCData[TORQUE_CH]*TORQUE_F*sys.rpm/9.549;
	OledDispFixed(4,9,(s32)(in*1e6),6,11,0);
	OledDispFixed(4,10,(s32)(out*1e6),6,11,0);
	OledDispFixed(4,11,(s32)(out/in*1e6),6,11,0);
	if(key&KEY_B)
		ADSStartUp(0,ADS_Primary_Rate);
	if(key&KEY_C)
		mainp.disp^=1;
	
	if(key&KEY_LEFT)
		PagesNext(-1);
	if(key&KEY_RIGHT)
		PagesNext(1);
}
