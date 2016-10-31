#include <asf.h>
#include <stdio.h>
#include "interrupt.h"
#include "sequencer.h"

void tc_callback(void)
{
	char string[20];
	static struct mydatastate primarystate = {
		.timetoexitmode = 10,
		.currentmode = STARTLEVEL
	};
	LED_Toggle(LED2);
	tickmode(&primarystate);
	sprintf(string,"time to exit mode: %d\r",primarystate.timetoexitmode);
	gfx_mono_draw_string(string,8,0,&sysfont);
	sprintf(string,"current mode: %d\r",primarystate.currentmode);
	gfx_mono_draw_string(string,8,8,&sysfont);
	tc_clear_overflow(&TCC0);
}