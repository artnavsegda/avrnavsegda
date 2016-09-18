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

void ad7705_configure()
{
	//ad7705_send_reset(void);
	//ad7705_set_clock_register(0x0C);
	//ad7705_set_setup_register(0x04);
	//ad7705_set_scale_register(uint8_t[]){0x18,0x3A,0x00});
	//ad7705_set_offset_register(uint8_t[]){0x89,0x78,0xD7});
}

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

	spi_deselect_device(&SPIC, &SPI_ADC);
}
