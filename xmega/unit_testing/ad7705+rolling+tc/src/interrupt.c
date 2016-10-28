#include <asf.h>
#include "spi_transfer.h"
#include "rolling.h"
#include "interrupt.h"
#include "ad7705.h"

extern struct spi_device SPI_ADC;
extern uint16_t adcdata;
extern struct massive firststage;

void ad7705_callback(void)
{
	LED_Toggle(LED2);
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
	{
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
		increment(&firststage,adcdata);
	}
}
