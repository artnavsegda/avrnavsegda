#include <asf.h>
#include <stdio.h>
#include "interrupt.h"

void tc_callback(void)
{
	static int i = 0;
	char string[20];
	LED_Toggle(LED2);
	sprintf(string,"%d",i++);
	gfx_mono_draw_string(string,10,10,&sysfont);
	tc_clear_overflow(&TCC0);
}
