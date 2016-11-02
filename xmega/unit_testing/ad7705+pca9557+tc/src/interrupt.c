#include <asf.h>
#include <stdio.h>
#include "interrupt.h"
#include "spi_transfer.h"
#include "ad7705.h"

extern struct spi_device SPI_ADC;
uint16_t adcdata;

void ad7705_callback(void)
{
	LED_Toggle(LED3);
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

void tc_callback(void)
{
	char string[20];
	LED_Toggle(LED2);
	snprintf(string,sizeof(string),"%6X", adcdata);
	gfx_mono_draw_string(string,8,0,&sysfont);
	tc_clear_overflow(&TCC0);
}
