#include <asf.h>
#include <stdio.h>
#include "i2c.h"
#include "setup.h"
#include "settings.h"

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
	i2c_read_array(&TWIE,0x08,I2C_JUMPTABLE,26,(uint8_t *)&mysettings.jump_table);

	snprintf(string,sizeof(string),"%u.%u.%u.%u", mysettings.ip[0], mysettings.ip[1], mysettings.ip[2], mysettings.ip[3]);
	gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%x:%x:%x:%x:%x:%x", mysettings.mac[0], mysettings.mac[1], mysettings.mac[2], mysettings.mac[3], mysettings.mac[4], mysettings.mac[5]);
	gfx_mono_draw_string(string,8,8,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"startlevel %d %d",mysettings.length_table.startlevel, mysettings.jump_table.startlevel);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"celldelay %d %d",mysettings.length_table.celldelay, mysettings.jump_table.celldelay);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"celllevel %d %d",mysettings.length_table.celllevel, mysettings.jump_table.celllevel);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"zerodelay %d %d",mysettings.length_table.zerodelay, mysettings.jump_table.zerodelay);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"zerotest %d %d",mysettings.length_table.zerotest, mysettings.jump_table.zerotest);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"purge %d %d",mysettings.length_table.startlevel, mysettings.jump_table.purge);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"totalmercurydelay %d %d",mysettings.length_table.totalmercurydelay, mysettings.jump_table.totalmercurydelay);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"totalmercury %d %d",mysettings.length_table.totalmercury, mysettings.jump_table.totalmercury);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"elementalmercurydelay %d %d",mysettings.length_table.elementalmercurydelay, mysettings.jump_table.elementalmercurydelay);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"elementalmercury %d %d",mysettings.length_table.elementalmercury, mysettings.jump_table.elementalmercury);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"precalibrationdelay %d %d",mysettings.length_table.precalibrationdelay, mysettings.jump_table.precalibrationdelay);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"calibration %d %d",mysettings.length_table.calibration, mysettings.jump_table.calibration);
	gfx_mono_draw_string(string,8,0,&sysfont);

	delay_ms(500);
	snprintf(string,sizeof(string),"postcalibrationdelay %d %d",mysettings.length_table.postcalibrationdelay, mysettings.jump_table.postcalibrationdelay);
	gfx_mono_draw_string(string,8,0,&sysfont);

}
