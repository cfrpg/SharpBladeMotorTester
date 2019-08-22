#ifndef __AD7606_H
#define __AD7606_H
#include "sys.h"

#define ADI_CS		PDout(1)
#define ADI_CV		PCout(12)
#define ADI_BUSY	PDin(0)
#define ADI_RST		PDout(2)


void AD7606Init(void);
void AD7606Reset(void);
void AD7606Start(void);
void AD7606Read(s16* data);

#endif