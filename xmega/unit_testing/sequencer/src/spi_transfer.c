#include <asf.h>
#include "spi_transfer.h"

uint8_t spi_transfer(SPI_t *spi, uint8_t data)
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) { }
	return spi_get(spi);
}