#include <asf.h>
#include <stdio.h>
#include "i2c.h"
#include "setup.h"
#include "settings.h"

extern int16_t adc_scan_results[16];

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
	i2c_read_array(&TWIE,0x08,I2C_LENGTHTABLE,26,(uint8_t *)&mysettings.length_table);
	i2c_read_array(&TWIE,0x08,I2C_JUMPTABLE,13,(uint8_t *)&mysettings.jump_table);

	snprintf(string,sizeof(string),"%u.%u.%u.%u", mysettings.ip[0], mysettings.ip[1], mysettings.ip[2], mysettings.ip[3]);
	gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%x:%x:%x:%x:%x:%x", mysettings.mac[0], mysettings.mac[1], mysettings.mac[2], mysettings.mac[3], mysettings.mac[4], mysettings.mac[5]);
	gfx_mono_draw_string(string,8,8,&sysfont);

	while (true)
	{
		snprintf(string,sizeof(string),"%d", adc_scan_results[1]);
		i2c_send_word(&TWIE,0x08,6,adc_scan_results[0]);
		gfx_mono_draw_string(string,8,16,&sysfont);
		delay_ms(500);
	}

}
