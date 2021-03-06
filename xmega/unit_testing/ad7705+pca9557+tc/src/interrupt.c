#include <asf.h>
#include <stdio.h>
#include "interrupt.h"
#include "spi_transfer.h"
#include "ad7705.h"
#include "pca9557.h"

extern struct spi_device SPI_ADC;
extern struct pca9557_pin x19_relay,x20_relay;
uint16_t adcdata;

extern uint16_t adczero, adcmax, adcresult;

void ad7705_callback(void)
{
	LED_Toggle(LED1);
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
	tc_clear_overflow(&TCC0);
	char string[20];
	LED_Toggle(LED0);
	snprintf(string,sizeof(string),"%6ld", (long)adcmax-0x17CC);
    gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%6ld", (long)adcresult-0x17CC);
	gfx_mono_draw_string(string,8,8,&sysfont);
	snprintf(string,sizeof(string),"%6ld", (long)adczero-0x17CC);
	gfx_mono_draw_string(string,8,16,&sysfont);
	snprintf(string,sizeof(string),"%f", (float)(adcresult-adczero)/(adcmax-adczero));
	gfx_mono_draw_string(string,8,24,&sysfont);
}
