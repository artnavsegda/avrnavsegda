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
	char string[20];
	LED_Toggle(LED2);
	snprintf(string,sizeof(string),"%6X", adcdata);
	gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%6lX", oversample(&firststage,16)/16);
	gfx_mono_draw_string(string,8,8,&sysfont);
	snprintf(string,sizeof(string),"%6lX", oversample(&firststage,32)/32);
	gfx_mono_draw_string(string,8,16,&sysfont);
	snprintf(string,sizeof(string),"%6lX", oversample(&firststage,64)/64);
	gfx_mono_draw_string(string,8,24,&sysfont);
	increment(&secondstage,oversample(&firststage,64)/64);
	snprintf(string,sizeof(string),"%6lX", oversample(&secondstage,8)/8);
	gfx_mono_draw_string(string,64,0,&sysfont);
	tc_clear_overflow(&TCC0);
}
