#include "parameter.h"

ParameterSet params;

u32 param_readWord(u32 addr)
{
	return *(vu32*)addr;
}


u8 ParamRead(void)
{
	u32 *data=(u32*)(&params);
	u16 i;
	for(i=0;i<sizeof(params)/4;i++)
	{
		data[i]=param_readWord(FLASH_BASE+TGT_ADDR+i*4);
	}
	return 0;
}


FLASH_Status ParamWrite(void)
{	
	u32 *data;
	u16 i;
	FLASH_Status status = FLASH_COMPLETE;
	FLASH_Unlock();
	FLASH_DataCacheCmd(DISABLE);
	u32 sectorAddr=FLASH_BASE+TGT_ADDR;
	status=FLASH_EraseSector(TGT_SECTOR,VoltageRange_3);
	if(status==FLASH_COMPLETE)
	{
		data=(u32*)(&params);
		for(i=0;i<sizeof(params)/4;i++)
		{
			status=FLASH_ProgramWord(sectorAddr+i*4,data[i]);
			if(status!=FLASH_COMPLETE)
				break;		
		}
	}
	FLASH_DataCacheCmd(ENABLE);
	FLASH_Lock();
	return status;
}

void ParamReset(void)
{
	params.headFlag=0xCFCFCFCF;
	params.version=0x00010000;
	params.tailFlag=0xFCFCFCFC;
}
