#include <asf.h>

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
	spi_master_init(&SPIC);
}

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

void spi_configure(void)
{
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 50000, 0);
}

void setup_configure(void)
{
	spi_configure();
}

void setup_enable(void)
{
	spi_enable(&SPIC);
}

uint8_t spi_transfer(SPI_t *spi, uint8_t data)
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) { }
	return spi_get(spi);
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
