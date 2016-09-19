#include <asf.h>
#include "setup.h"

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