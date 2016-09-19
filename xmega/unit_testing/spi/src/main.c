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
	spi_transfer(&SPIC, CONFIG_SPI_MASTER_DUMMY);
	spi_deselect_device(&SPIC, &SPI_ADC);
}
