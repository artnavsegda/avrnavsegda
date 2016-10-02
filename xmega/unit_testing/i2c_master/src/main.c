#include <asf.h>
#include <stdio.h>
#include "i2c.h"
#include "setup.h"

struct MyObject {
	char zero[4];
	char one[6];
	char two[26];
	char three[13];
};

struct MyObject e;

struct MyObject d = {
	.zero = "432",
	.one = "65432",
	.two = "abcdefghijklmnopqrstuwxy",
	.three = "0123456789ab"
};

struct MyObject c;

int main (void)
{
	char string[20] = "shit";
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */

	//i2c_send(&TWIE, 0x08, 5, 42);
	//i2c_send_word(&TWIE, 0x08, 6, 420);
	//i2c_send_double(&TWIE, 0x08, 7, (float)4.20);
	//i2c_send_array(&TWIE, 0x08, 8, 5, "hell");

	//snprintf(string,sizeof(string),"%d", i2c_read(&TWIE, 0x08, 5));
	//gfx_mono_draw_string(string,8,8,&sysfont);
	//snprintf(string,sizeof(string),"%d", i2c_read_word(&TWIE, 0x08, 6));
	//gfx_mono_draw_string(string,8,8,&sysfont);
	//snprintf(string,sizeof(string),"%f", i2c_read_double(&TWIE, 0x08, 7));
	//gfx_mono_draw_string(string,8,8,&sysfont);
	//i2c_read_array(&TWIE, 0x08, 8, 5, string);
	//gfx_mono_draw_string(string,8,8,&sysfont);

	i2c_read_array(&TWIE, 0x08, 0, 4, e.zero);
	i2c_read_array(&TWIE, 0x08, 1, 6, e.one);
	i2c_read_array(&TWIE, 0x08, 2, 20, e.two);
	i2c_read_array(&TWIE, 0x08, 3, 13, e.three);

	gfx_mono_draw_string(e.zero,0,0,&sysfont);
	gfx_mono_draw_string(e.one,0,8,&sysfont);
	gfx_mono_draw_string(e.two,0,16,&sysfont);
	gfx_mono_draw_string(e.three,0,24,&sysfont);

	delay_ms(1000);

	i2c_send_array(&TWIE, 0x08, 0, 4, d.zero);
	i2c_send_array(&TWIE, 0x08, 1, 6, d.one);
	i2c_send_array(&TWIE, 0x08, 2, 20, d.two);
	i2c_send_array(&TWIE, 0x08, 3, 13, d.three);

	i2c_read_array(&TWIE, 0x08, 0, 4, c.zero);
	i2c_read_array(&TWIE, 0x08, 1, 6, c.one);
	i2c_read_array(&TWIE, 0x08, 2, 20, c.two);
	i2c_read_array(&TWIE, 0x08, 3, 13, c.three);

	gfx_mono_draw_string(c.zero,0,0,&sysfont);
	gfx_mono_draw_string(c.one,0,8,&sysfont);
	gfx_mono_draw_string(c.two,0,16,&sysfont);
	gfx_mono_draw_string(c.three,0,24,&sysfont);

	i2c_send_array(&TWIE, 0x08, 0, 4, e.zero);
	i2c_send_array(&TWIE, 0x08, 1, 6, e.one);
	i2c_send_array(&TWIE, 0x08, 2, 20, e.two);
	i2c_send_array(&TWIE, 0x08, 3, 13, e.three);
}
