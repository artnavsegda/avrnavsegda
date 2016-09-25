#include <asf.h>
#include <stdio.h>
#include "setup.h"

extern int16_t adca_scan_results[8];
extern int16_t adcb_scan_results[8];

int main (void)
{
	char string[20];

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adca_scan_results[0], adca_scan_results[1], adca_scan_results[2], adca_scan_results[3]);
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adca_scan_results[4], adca_scan_results[5], adca_scan_results[6], adca_scan_results[7]);
		gfx_mono_draw_string(string,8,8,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adcb_scan_results[0], adcb_scan_results[1], adcb_scan_results[2], adcb_scan_results[3]);
		gfx_mono_draw_string(string,8,16,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adcb_scan_results[4], adcb_scan_results[5], adcb_scan_results[6], adcb_scan_results[7]);
		gfx_mono_draw_string(string,8,24,&sysfont);
		//delay_ms(500);
	}
}
