#include "pages.h"
#include "ads1256.h"
#include "ad7606.h"
#include "adc.h"
//u32 EOcnt;

s16 adcTmp[8];
void UpdateADC(void)
{
	u8 i;
	//ADSReadAllChannel(sys.ADCRawData);
	AD7606Read(adcTmp);
	for(i=0;i<8;i++)
	{		
		//sys.sensors.ADCData[i]=((float)sys.ADCRawData[i])/(1<<23)*5;
		sys.ADCRawData[i]=adcTmp[i];
		sys.sensors.ADCData[i]=((float)adcTmp[i])/(1<<15)*5;
	}
	sys.rpm=60*1000*1000/sys.EOtime;
	//ADCReadVol(&(sys.sensors.ADCData[8]));	
}

void SendSensorData(void)
{
	sys.sensors.data[0]=sys.rpm;
	sys.sensors.data[1]=sys.pwm[0];
	sys.sensors.data[2]=ADSCpu2;	
	sys.sensors.header.time=LinkPackTime();
	LinkSendData(&sys.sensors,sizeof(SensorDataPackage));
}

void UpdateSensors(void)
{
	//if(sys.adcEn)
		UpdateADC();
	SendSensorData();
}

