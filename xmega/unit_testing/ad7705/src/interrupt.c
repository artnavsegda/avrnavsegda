#include <asf.h>
#include "spi_transfer.h"

extern struct spi_device SPI_ADC;

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
