#include <asf.h>
#include "spi_transfer.h"
#include "ad7705.h"

extern struct spi_device SPI_ADC;
extern uint16_t adcdata;

ISR(PORTC_INT0_vect)
{
	LED_Toggle(LED2);
	/*spi_select_device(&SPIC, &SPI_ADC);
	spi_transfer(&SPIC, 0x08);
	if (spi_transfer(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		spi_transfer(&SPIC,0x38);
		spi_read_packet(&SPIC, (uint8_t *)value, 2);
	}
	spi_deselect_device(&SPIC, &SPI_ADC);*/

	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
}
