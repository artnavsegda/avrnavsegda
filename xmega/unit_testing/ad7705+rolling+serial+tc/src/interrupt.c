#include <asf.h>
#include <stdio.h>
#include "spi_transfer.h"
#include "rolling.h"
#include "interrupt.h"
#include "ad7705.h"

extern struct spi_device SPI_ADC;
extern uint16_t adcdata;
struct massive firststage, secondstage;

void ad7705_callback(void)
{
	LED_Toggle(LED3);
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
	{
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
		increment(&firststage,adcdata);
	}
}

void tc_callback(void)
{
	LED_Toggle(LED2);
	increment(&secondstage,oversample(&firststage,64)/64);
	printf("\033[2J%6X\r", adcdata);
	printf("%6lX\r", oversample(&firststage,16)/16);
	printf("%6lX\r", oversample(&firststage,32)/32);
	printf("%6lX\r", oversample(&firststage,64)/64);
	printf("%6lX\r", oversample(&secondstage,8)/8);
	tc_clear_overflow(&TCC0);
}
