#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "rolling.h"

int16_t adc_scan_results[16];
uint16_t adcdata = 0;
struct massive firststage;
struct massive secondstage;
struct massive adcmassive;

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
		//i2c_send_word(&TWIE, 0x08, 0, adcdata);
		snprintf(string,sizeof(string),"%6ld", oversample(&firststage,16));
		gfx_mono_draw_string(string,8,8,&sysfont);
		//i2c_send_word(&TWIE, 0x08, 1, oversample(firststage,16));
		snprintf(string,sizeof(string),"%6ld", oversample(&firststage,32));
		gfx_mono_draw_string(string,8,16,&sysfont);
		//i2c_send_word(&TWIE, 0x08, 2, oversample(firststage,32));
		snprintf(string,sizeof(string),"%6ld", oversample(&firststage,64));
		gfx_mono_draw_string(string,8,24,&sysfont);
		//i2c_send_word(&TWIE, 0x08, 3, oversample(firststage,64));
		increment(&secondstage,oversample(&firststage,64));
		//i2c_send_word(&TWIE, 0x08, 4, oversample(secondstage,128));
		increment(&adcmassive,adc_scan_results[0]);
		//i2c_send_word(&TWIE, 0x08, 5, oversample(adcmassive,128));
		delay_ms(500);
	}
}
