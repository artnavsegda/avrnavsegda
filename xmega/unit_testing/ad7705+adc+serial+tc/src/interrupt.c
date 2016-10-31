#include <asf.h>
#include <stdio.h>
#include "interrupt.h"
#include "spi_transfer.h"
#include "ad7705.h"

extern struct spi_device SPI_ADC;
int16_t adc_scan_results[16];
uint16_t adcdata;

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
	LED_Toggle(LED2);
	printf("\033[2J%03X %03X %03X %03X %03X\r", adc_scan_results[0], adc_scan_results[1], adc_scan_results[2], adc_scan_results[3], adc_scan_results[4]);
	printf("%03X %03X %03X %03X %03X\r", adc_scan_results[5], adc_scan_results[6], adc_scan_results[7], adc_scan_results[8], adc_scan_results[9]);
	printf("%03X %03X %03X %03X %03X\r", adc_scan_results[10], adc_scan_results[11], adc_scan_results[12], adc_scan_results[13], adc_scan_results[14]);
	printf("%03X %04X\r", adc_scan_results[15], adcdata);
	tc_clear_overflow(&TCC0);
}
