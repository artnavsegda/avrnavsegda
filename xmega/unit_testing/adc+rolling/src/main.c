#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "rolling.h"

extern struct massive adc_massive[1];
extern int16_t adc_scan_results[16];
char string[40];

#define DIVIDER 1

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	do {
		LED_Toggle(LED2);
		sprintf(string,"%03X %04lX %05lX %06lX", adc_scan_results[0], oversample(&adc_massive[0],16), oversample(&adc_massive[0],256), oversample(&adc_massive[0],4096));
		gfx_mono_draw_string(string,0,0,&sysfont);

		/*sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[0],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[1],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[2],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[3],MASSIVE_SIZE)/DIVIDER);
		gfx_mono_draw_string(string,8,0,&sysfont);
		sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[4],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[5],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[6],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[7],MASSIVE_SIZE)/DIVIDER);
		gfx_mono_draw_string(string,8,8,&sysfont);
		sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[8],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[9],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[10],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[11],MASSIVE_SIZE)/DIVIDER);
		gfx_mono_draw_string(string,8,16,&sysfont);
		sprintf(string,"%04lX %04lX %04lX %04lX", oversample(&adc_massive[12],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[13],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[14],MASSIVE_SIZE)/DIVIDER, oversample(&adc_massive[15],MASSIVE_SIZE)/DIVIDER);
		gfx_mono_draw_string(string,8,24,&sysfont);*/
		delay_ms(1000);
	} while (1);
}
