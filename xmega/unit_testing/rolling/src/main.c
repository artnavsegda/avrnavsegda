#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "rolling.h"

uint16_t adcdata = 0;
extern struct massive firststage;

int main (void)
{
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */

	delay_ms(100);

	while (true) {
		snprintf(string,sizeof(string),"%04X", adcdata);
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%6ld", average(firststage.massive,16,firststage.position,64));
		gfx_mono_draw_string(string,8,8,&sysfont);
		snprintf(string,sizeof(string),"%6ld", average(firststage.massive,32,firststage.position,64));
		gfx_mono_draw_string(string,8,16,&sysfont);
		snprintf(string,sizeof(string),"%6ld", average(firststage.massive,64,firststage.position,64));
		gfx_mono_draw_string(string,8,24,&sysfont);
		delay_ms(500);
	}
}
