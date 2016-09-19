#include <asf.h>
#include "setup.h"

uint8_t spi_transfer(SPI_t *spi, uint8_t data)
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) { }
	return spi_get(spi);
}

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

ISR(PORTC_INT0_vect)
{
	uint8_t array[2];
	spi_select_device(&SPIC, &SPI_ADC);
	spi_transfer(&SPIC, 0x08);
	if (spi_transfer(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		spi_transfer(&SPIC,0x38);
		spi_read_packet(&SPIC, array, 2);
	}
	spi_deselect_device(&SPIC, &SPI_ADC);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_enable();
	setup_configure();

	/* Insert application code here, after the board has been initialized. */

}
