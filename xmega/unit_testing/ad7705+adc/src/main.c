#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"

uint16_t adc_scan_results[16];
uint16_t adcdata = 0;

int main (void)
{
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */

	while (true) {
		snprintf(string,sizeof(string),"%03X %03X %03X %03X %03X", adc_scan_results[0], adc_scan_results[1], adc_scan_results[2], adc_scan_results[3], adc_scan_results[4]);
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%03X %03X %03X %03X %03X", adc_scan_results[5], adc_scan_results[6], adc_scan_results[7], adc_scan_results[8], adc_scan_results[9]);
		gfx_mono_draw_string(string,8,8,&sysfont);
		snprintf(string,sizeof(string),"%03X %03X %03X %03X %03X", adc_scan_results[10], adc_scan_results[11], adc_scan_results[12], adc_scan_results[13], adc_scan_results[14]);
		gfx_mono_draw_string(string,8,16,&sysfont);
		snprintf(string,sizeof(string),"%03X %04X", adc_scan_results[15], adcdata);
		gfx_mono_draw_string(string,8,24,&sysfont);
		delay_ms(50);
	}
}
