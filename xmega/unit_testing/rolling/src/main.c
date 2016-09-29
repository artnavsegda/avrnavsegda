#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "rolling.h"

int16_t adc_scan_results[16];
struct massive firststage;

int main (void)
{
	char string[20];
	struct massive adcone;

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		increment(adcone, adc_scan_results[0]);
		snprintf(string,sizeof(string),"%ld", oversample(adcone, 500));
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%ld", oversample(firststage, 32));
		gfx_mono_draw_string(string,8,8,&sysfont);
		delay_ms(1000);
	}
}
