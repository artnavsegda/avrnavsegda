#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "rolling.h"

extern struct massive adc_massive[16];
extern int16_t adc_scan_results[16];

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	do {
		LED_Toggle(LED2);
		sprintf(string,"\033[2J%04lX %04lX %04lX %04lX", oversample(&adc_massive[0],16), oversample(&adc_massive[1],16), oversample(&adc_massive[2],16), oversample(&adc_massive[3],16));
		sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[4],16), oversample(&adc_massive[5],16), oversample(&adc_massive[6],16), oversample(&adc_massive[7],16));
		sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[8],16), oversample(&adc_massive[9],16), oversample(&adc_massive[10],16), oversample(&adc_massive[11],16));
		sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[12],16), oversample(&adc_massive[13],16), oversample(&adc_massive[14],16), oversample(&adc_massive[15],16));
		delay_ms(100);
	} while (1);
}
