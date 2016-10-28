#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "rolling.h"

uint16_t adcdata = 0;
struct massive firststage, secondstage;

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
		snprintf(string,sizeof(string),"%6X", adcdata);
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%6lX", oversample(&firststage,16)/16);
		gfx_mono_draw_string(string,8,8,&sysfont);
		snprintf(string,sizeof(string),"%6lX", oversample(&firststage,32)/32);
		gfx_mono_draw_string(string,8,16,&sysfont);
		snprintf(string,sizeof(string),"%6lX", oversample(&firststage,64)/64);
		gfx_mono_draw_string(string,8,24,&sysfont);
		increment(&secondstage,oversample(&firststage,64)/64);
		snprintf(string,sizeof(string),"%6lX", oversample(&secondstage,8)/8);
		gfx_mono_draw_string(string,64,0,&sysfont);
		delay_ms(500);
	}
}
