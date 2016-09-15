#include <asf.h>

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
	spi_master_init(&SPIC);
}

void spi_configure(void)
{
	struct spi_device SPI_ADC = {
		.id = SPIC_SS
	};
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

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();;

	/* Insert application code here, after the board has been initialized. */
}
