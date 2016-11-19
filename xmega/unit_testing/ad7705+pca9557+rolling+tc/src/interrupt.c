#include <asf.h>
#include <stdio.h>
#include "interrupt.h"
#include "spi_transfer.h"
#include "ad7705.h"
#include "pca9557.h"
#include "rolling.h"

extern struct spi_device SPI_ADC;
extern struct pca9557_pin x19_relay,x20_relay;
uint16_t adcdata;
struct massive firststage, secondstage;

void ad7705_callback(void)
{
	LED_Toggle(LED1);

	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
	{
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
		increment(&firststage,adcdata);
	}
}

void tc_callback(void)
{
	tc_clear_overflow(&TCC0);
	LED_Toggle(LED0);
	increment(&secondstage,oversample(&firststage,64)/64);
}
