#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "settings.h"
#include "i2c.h"

uint16_t adc_scan_results[16];
uint16_t adcdata = 0;

int main (void)
{
	struct mydatastruct mysettings;
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	i2c_read_array(&TWIE,0x08,I2C_IPADDRESS,4,mysettings.ip);
	i2c_read_array(&TWIE,0x08,I2C_MACADDRESS,6,mysettings.mac);
	mysettings.ad7705_setup_register = i2c_read(&TWIE,0x08,I2C_AD7705_SETUP_REGISTER);
	mysettings.ad7705_clock_register = i2c_read(&TWIE,0x08,I2C_AD7705_CLOCK_REGISTER);

	snprintf(string,sizeof(string),"%u.%u.%u.%u", mysettings.ip[0], mysettings.ip[1], mysettings.ip[2], mysettings.ip[3]);
	gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%x:%x:%x:%x:%x:%x", mysettings.mac[0], mysettings.mac[1], mysettings.mac[2], mysettings.mac[3], mysettings.mac[4], mysettings.mac[5]);
	gfx_mono_draw_string(string,8,8,&sysfont);
	snprintf(string,sizeof(string),"setup:%02X clock:%02X", mysettings.ad7705_setup_register, mysettings.ad7705_clock_register);
	gfx_mono_draw_string(string,8,16,&sysfont);

	delay_ms(3000);

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
