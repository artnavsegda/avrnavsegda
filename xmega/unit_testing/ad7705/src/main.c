#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"

//uint16_t adcdata = 0;
extern struct spi_device SPI_ADC;

int main (void)
{
	uint16_t adcdata;
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */

	spi_select_device(&SPIC, &SPI_ADC);
	spi_write_packet(&SPIC, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5);
	spi_write_packet(&SPIC, (uint8_t[]){0x20,0x0C,0x10,0x04}, 4);

	delay_ms(100);


	while (true) {
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
		snprintf(string,sizeof(string),"%04X", adcdata);
		gfx_mono_draw_string(string,8,0,&sysfont);
		delay_ms(500);
	}
}
