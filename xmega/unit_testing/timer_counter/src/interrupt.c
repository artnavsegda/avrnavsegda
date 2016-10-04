#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "ad7705.h"
#include "spi_transfer.h"
#include "rolling.h"
#include "i2c.h"

extern struct spi_device SPI_ADC;
uint16_t adcdata;
struct massive firststage;
struct massive secondstage;
struct massive adcmassive;
int16_t adc_scan_results[16];

void tc_callback(void)
{
	char string[20];
	snprintf(string,sizeof(string),"%04X", adcdata);
	gfx_mono_draw_string(string,8,0,&sysfont);
	increment(&secondstage,oversample(&firststage,64));
	increment(&adcmassive,adc_scan_results[0]);
	i2c_send_word(&TWIE, 0x08, 0, adcdata);
	i2c_send_word(&TWIE, 0x08, 1, oversample(&firststage,64));
	i2c_send_word(&TWIE, 0x08, 2, oversample(&secondstage,128));
	i2c_send_word(&TWIE, 0x08, 3, oversample(&adcmassive,128));
}

void adc_callback(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_adca_scan_channel = 0, current_adcb_scan_channel = 0;
	if (adc == &ADCA) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adca_scan_channel] = result;
			current_adca_scan_channel++;
			if (current_adca_scan_channel == 8)	current_adca_scan_channel = 0;
		}
	}
	else if (adc == &ADCB) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adcb_scan_channel+8] = result;
			current_adcb_scan_channel++;
			if (current_adcb_scan_channel == 8)	current_adcb_scan_channel = 0;
		}
	}
}

void ad7705_callback(void)
{
	LED_Toggle(LED2);
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
	{
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
		increment(&firststage,adcdata);
	}
}
