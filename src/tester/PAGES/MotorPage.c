#include "pages.h"

struct
{
	s32 rpm;
	s32 volt;
	s32 curr;
	s32 thro;
	u8 state;
} motor;

void PageInit_Motor(u8 f)
{
	if(f)
	{
		motor.rpm=0;
		motor.volt=0;
		motor.curr=0;
		motor.state=0;
		motor.thro=0;
		return;
	}
	OledClear(0);
	PagesDrawHeader(MotorPage,"Motor");
	OledDispString(0,1,"DISARMD",0);
	OledDispString(0,2,"THRO:",0);
	OledDispString(0,3,"RPM :",0);
	OledDispString(0,4,"VOLT:",0);
	OledDispString(0,5,"CURR:",0);
}

void PageUpdate_Motor(void)
{
	u8 key=currKey&(currKey^lastKey);
	
	OledDispFixed(5,2,motor.thro,1,5,0);
	OledDispInt(5,3,motor.rpm,7,0);
	OledDispFixed(5,4,motor.volt,3,6,0);
	OledDispFixed(5,5,motor.curr,3,6,0);
	if(key&KEY_D)
		PagesNext(1);
}