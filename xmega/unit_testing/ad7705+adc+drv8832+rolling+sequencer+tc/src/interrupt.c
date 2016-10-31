#include <asf.h>
#include <stdio.h>
#include "ad7705.h"
#include "interrupt.h"
#include "sequencer.h"
#include "modbus.h"
#include "rolling.h"

extern struct spi_device SPI_ADC;
int16_t adc_scan_results[16];
uint16_t adcdata;
struct massive firststage, secondstage;

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
	adc_start_conversion(adc, ch_mask);
}

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
	static struct mydatastate primarystate = {
		.timetoexitmode = 10,
		.currentmode = STARTLEVEL
	};
	LED_Toggle(LED2);
	increment(&secondstage,oversample(&firststage,64)/64);
	tickmode(&primarystate);
	send_data(&primarystate);
	tc_clear_overflow(&TCC0);
}
