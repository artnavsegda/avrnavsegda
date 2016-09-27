#include <asf.h>
#include <stdio.h>
#include "i2c.h"
#include "setup.h"

#define I2C_IPADDRESS 100
#define I2C_MACADDRESS 101

struct mydatastruct
{
	uint8_t ip[4];
	uint8_t mac[6];
	uint16_t length_table[13];
	uint8_t jump_table[13];
	uint8_t ad7705_setup_register;
	uint8_t ad7705_clock_register;
	uint8_t ad7705_zero_scale[3];
	uint8_t ad7705_full_scale[3];
	float standard_concentration;
	float c_twentie_five;
	float kfactor;
};

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

	snprintf(string,sizeof(string),"%u.%u.%u.%u", mysettings.ip[0], mysettings.ip[1], mysettings.ip[2], mysettings.ip[3]);
	gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%x:%x:%x:%x:%x:%x", mysettings.mac[0], mysettings.mac[1], mysettings.mac[2], mysettings.mac[3], mysettings.mac[4], mysettings.mac[5]);
	gfx_mono_draw_string(string,8,8,&sysfont);

}
