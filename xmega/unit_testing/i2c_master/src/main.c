#include <asf.h>
#include <stdio.h>
#include "i2c.h"
#include "setup.h"

int main (void)
{
	uint8_t i = 0;
	char string[20];
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


	i2c_send(&TWIE, 0x1a, 0x03, 0x00);
	i2c_send(&TWIE, 0x1a, 0x02, 0x00);

	while (true)
	{
		LED_Toggle(LED2);
		i2c_send(&TWIE, 0x1a, 0x01, 0xFF);
		delay_ms(1000);
		i2c_send(&TWIE, 0x1a, 0x01, 0x00);
		delay_ms(1000);
	}

}
