#include <asf.h>
#include <stdio.h>
#include "setup.h"

uint16_t value;

int main (void)
{
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_enable();
	setup_configure();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		snprintf(string,sizeof(string),"%04X", value);
		gfx_mono_draw_string(string,8,0,&sysfont);
		delay_ms(500);
	}
}
