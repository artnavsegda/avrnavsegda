#include <asf.h>
#include <stdio.h>
#include "interrupt.h"
#include "sequencer.h"

void tc_callback(void)
{
	static struct mydatastate primarystate = {
		.timetoexitmode = 10,
		.currentmode = STARTLEVEL
	};
	LED_Toggle(LED2);
	tickmode(&primarystate);
	printf("time to exit mode: %d\r",primarystate.timetoexitmode);
	printf("current mode: %d\r",primarystate.currentmode);
	tc_clear_overflow(&TCC0);
}
