#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "i2c.h"

uint16_t adcdata = 0;

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
			delay_ms(250);
			i2c_send_word(&TWIE, 0x08, 0, 16);
			delay_ms(250);
	}
}
