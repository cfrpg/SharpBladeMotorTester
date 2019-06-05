#include "pages.h"

#define CURR_F 15.3413686
#define VOL_F 3.0;

struct
{
	u8 state;
} mainp;
void PageInit_main(u8 f)
{
	if(f)
	{
		mainp.state=0;
		return;
	}
	OledClear(0);
	PagesDrawHeader(MainPage,"Main");
}

void PageUpdate_main(void)
{
	u8 key=currKey&(currKey^lastKey);
	if(key&KEY_LEFT)
		PagesNext(-1);
	if(key&KEY_RIGHT)
		PagesNext(1);
}
