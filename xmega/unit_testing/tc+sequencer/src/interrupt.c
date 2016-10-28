#include <asf.h>
#include <stdio.h>
#include "interrupt.h"

void tc_callback(void)
{
	LED_Toggle(LED2);
	primarystate.timetoexitmode--;
	if (primarystate.timetoexitmode == 0)
	exitmode(primarystate.currentmode,&primarystate);
	tc_clear_overflow(&TCC0);
}