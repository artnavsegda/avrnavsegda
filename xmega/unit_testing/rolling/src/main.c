#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "rolling.h"

extern int16_t adca_scan_results[8];
extern int16_t adcb_scan_results[8];

int main (void)
{
	char string[20];
	struct massive adcone;

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		increment(adcone, adca_scan_results[0]);
		snprintf(string,sizeof(string),"%ld", oversample(adcone, 500));
		gfx_mono_draw_string(string,8,0,&sysfont);
		delay_ms(1000);
	}
}
