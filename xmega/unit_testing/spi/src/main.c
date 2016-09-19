#include <asf.h>
#include "setup.h"
#include "spi_transfer.h"

extern struct spi_device SPI_ADC;

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	spi_select_device(&SPIC, &SPI_ADC);

	spi_write_packet(&SPIC, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5);
	spi_write_packet(&SPIC, (uint8_t[]){0x20,0x0C,0x10,0x04}, 4);
	spi_write_packet(&SPIC, (uint8_t[]){0x60,0x18,0x3A,0x00}, 4);
	spi_write_packet(&SPIC, (uint8_t[]){0x70,0x89,0x78,0xD7}, 4);

	spi_transfer(&SPIC, 0x08);
	if (spi_transfer(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		spi_transfer(&SPIC,0x38);
		spi_read_packet(&SPIC, array, 2);
	}

	spi_deselect_device(&SPIC, &SPI_ADC);
}
