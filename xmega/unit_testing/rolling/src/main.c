#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "rolling.h"
#include "i2c.h"

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
		//i2c_send_word(&TWIE, 0x08, I2C_AD7705_DATA, adcdata);
		snprintf(string,sizeof(string),"%04lX", oversample(&firststage,16)/16);
		gfx_mono_draw_string(string,8,8,&sysfont);
		//i2c_send_word(&TWIE, 0x08, I2C_AD7705_DATAX16, oversample(firststage,16));
		snprintf(string,sizeof(string),"%04lX", oversample(&firststage,32)/32);
		gfx_mono_draw_string(string,8,16,&sysfont);
		//i2c_send_word(&TWIE, 0x08, I2C_AD7705_DATA32, oversample(firststage,32));
		snprintf(string,sizeof(string),"%04lX", oversample(&firststage,64)/64);
		gfx_mono_draw_string(string,8,24,&sysfont);
		//i2c_send_word(&TWIE, 0x08, I2C_AD7705_DATA64, oversample(firststage,64));
		increment(&secondstage,oversample(&firststage,64)/64);
		snprintf(string,sizeof(string),"%04lX", oversample(&secondstage,8)/8);
		gfx_mono_draw_string(string,64,0,&sysfont);
		//i2c_send_word(&TWIE, 0x08, 4, oversample(secondstage,8));
		//increment(&adcmassive,adc_scan_results[0]);
		//i2c_send_word(&TWIE, 0x08, 5, oversample(adcmassive,128));
		delay_ms(500);
	}
}
