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
	spi_select_device(&SPIC, &SPI_ADC);
	spi_deselect_device(&SPIC, &SPI_ADC);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();;

	/* Insert application code here, after the board has been initialized. */
	spi_select_device(&SPIC, &SPI_ADC);
	spi_transfer(&SPIC, CONFIG_SPI_MASTER_DUMMY);
	spi_deselect_device(&SPIC, &SPI_ADC);
}
