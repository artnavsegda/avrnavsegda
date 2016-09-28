#include <asf.h>
#include "ad7705.h"
#include "spi_transfer.h"

void ad7705_send_reset(SPI_t *spi, struct spi_device *device)
{
	spi_select_device(spi, device);
	spi_write_packet(spi, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5);
	spi_deselect_device(spi, device);
}

void ad7705_set_clock_register(SPI_t *spi, struct spi_device *device, uint8_t contents)
{
	spi_select_device(spi, device);
	spi_transfer(spi, 0x20);
	spi_transfer(spi, contents);
	spi_deselect_device(spi, device);
}

void ad7705_set_setup_register(SPI_t *spi, struct spi_device *device, uint8_t contents)
{
	spi_select_device(spi, device);
	spi_transfer(spi, 0x10);
	spi_transfer(spi, contents);
	spi_deselect_device(spi, device);
}

void ad7705_set_scale_register(SPI_t *spi, struct spi_device *device, uint32_t contents)
{
	spi_select_device(spi, device);
	spi_transfer(spi, 0x60);
	spi_transfer(spi, MSB1(contents));
	spi_transfer(spi, MSB2(contents));
	spi_transfer(spi, MSB3(contents));
	spi_deselect_device(spi, device);
}

void ad7705_set_offset_register(SPI_t *spi, struct spi_device *device, uint32_t contents)
{
	spi_select_device(spi, device);
	spi_transfer(spi, 0x70);
	spi_transfer(spi, MSB1(contents));
	spi_transfer(spi, MSB2(contents));
	spi_transfer(spi, MSB3(contents));
	spi_deselect_device(spi, device);
}
