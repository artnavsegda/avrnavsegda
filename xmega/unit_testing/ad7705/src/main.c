#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"

uint16_t value = 0;
extern struct spi_device SPI_ADC;

int main (void)
{
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_enable();
	setup_configure();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		//value = ad7705_get_data_register(&SPIC, &SPI_ADC);
		snprintf(string,sizeof(string),"%04X", value);
		gfx_mono_draw_string(string,8,0,&sysfont);
		delay_ms(500);
	}
}
