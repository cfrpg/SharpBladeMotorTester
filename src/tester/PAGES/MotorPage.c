#include "pages.h"
#include "pwm.h"

struct
{
	s32 rpm;
	s32 volt;
	s32 curr;
	s32 thro;
	u8 state;
	u8 lastarmed;
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
		motor.state=0;
		motor.lastarmed=0;
		return;
	}
	OledClear(0);
	PagesDrawHeader(MotorPage,"Motor");
	OledDispString(0,2,"DISARMED",0);
	OledDispString(0,3,"THRO:",0);
	OledDispString(0,4,"RPM :",0);
	OledDispString(0,5,"VOLT:",0);
	OledDispString(0,6,"CURR:",0);
	if(PWMIsArmed())
	{
		OledDispString(0,15,"    ",0);
		OledDispString(0,2,"ARMED   ",0);
	}
	else
	{
		OledDispString(0,15,"CALI",0);
		OledDispString(0,2,"DISARMED",0);
	}
}

void PageUpdate_Motor(void)
{
	u8 key=currKey&(currKey^lastKey);
	u8 armed=PWMIsArmed();
	switch(motor.state)
	{
		case 0://normal
			if(armed)
			{
				if(motor.lastarmed==0)
				{
					OledDispString(0,15,"    ",0);
					OledDispString(0,2,"ARMED   ",0);
				}
			}
			else
			{
				if(motor.lastarmed)
				{
					OledDispString(0,15,"CALI",0);
					OledDispString(0,2,"DISARMED",0);
				}
				if(key&KEY_A)
				{
					motor.state=1;
					PWMArm();
					sys.pwm[0]=1000;
					sys.pwm[1]=1000;
					sys.pwm[2]=1000;
					sys.pwm[3]=1000;
					PWMSet(1000,1000,1000,1000);
				}
			}
		break;
		case 1://cali esc
			OledDispString(0,2,"ARMED    ",0);
			OledDispString(0,15,"SET ",0);
			if(key&KEY_A)
			{
				motor.state=2;	
				sys.pwm[0]=0;		
				sys.pwm[1]=0;
				sys.pwm[2]=0;
				sys.pwm[3]=0;				
				PWMSet(0,0,0,0);
			}
		break;
		case 2://cali esc
			OledDispString(0,2,"ARMED    ",0);
			OledDispString(0,15,"OK  ",0);
			if(key&KEY_A)
			{
				motor.state=0;				
				PWMDisarm();
			}
		break;
	}
	motor.lastarmed=armed;
	OledDispFixed(5,3,sys.pwm[0],1,5,0);
	OledDispInt(5,4,sys.rpm,7,0);
	OledDispFixed(5,5,motor.volt,3,6,0);
	OledDispFixed(5,6,motor.curr,3,6,0);
	if(motor.state==0)
	{
		if(key&KEY_LEFT)
			PagesNext(-1);
		if(key&KEY_RIGHT)
			PagesNext(1);
	}

}
