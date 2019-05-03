#ifndef __PAGES_H
#define __PAGES_H
#include "sys.h"
#include "oled.h"
#include "keyboard.h"
#include "sblink.h"
#include "stdio.h"

#define MainPage 0
#define ADCPage 1
#define MotorPage 2

#define PageNum 3

typedef struct
{
	s32 ADCRawData[8];
	SensorDataPackage sensors;
	s32 rpm;
	u16 pwm[4];
	u8 adcEn;
	
} systemState;

extern systemState sys;
extern u8 lastKey;
extern u8 currKey;
extern s32 lastWheel;
extern s32 currWheel;

extern u8 currpage;
extern u16 cpucnt;

void PagesInit(void);
void PagesChangeTo(u8 p);
void PagesNext(s8 d);
void PagesUpdate(void);
void PagesDrawHeader(u8 n,s8 *name);

void UpdateSensors(void);

void PageInit_main(u8 f);
void PageUpdate_main(void);

void PageInit_ADC(u8 f);
void PageUpdate_ADC(void);

void PageInit_Motor(u8 f);
void PageUpdate_Motor(void);

#endif
