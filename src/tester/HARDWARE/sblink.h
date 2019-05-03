#ifndef __SBLINK_H
#define __SBLINK_H
#include "sys.h"

#define SENDBUFFSIZE 256
#define LINKSTX 0xFC

#define HEARTBEAT 0x00
#define SENSOR_DATA 0x01

extern const u8 DataLen[];


typedef struct
{
	u8 stx;
	u8 len;
	u8 seq;
	u8 fun;
	u32 time;
} PackageHeader;

typedef struct
{
	PackageHeader header;
	float ADCData[8];
	s32 data[4];
} SensorDataPackage;

extern u8 sendBuff[SENDBUFFSIZE];

void LinkInit(void);
void LinkSendData(void* buff,u8 len);

#endif
