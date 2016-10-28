#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"

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
		snprintf(string,sizeof(string),"%04X", adc_scan_results[15], adcdata);
		gfx_mono_draw_string(string,8,24,&sysfont);
		delay_ms(50);
	}
}
