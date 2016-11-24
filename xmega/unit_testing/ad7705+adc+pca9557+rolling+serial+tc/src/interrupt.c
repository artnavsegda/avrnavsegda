#include <asf.h>
#include <stdio.h>
#include "ad7705.h"
#include "interrupt.h"
#include "rolling.h"
#include "ra915.h"

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
	LED_Toggle(LED1);
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
		adcdata = ad7705_get_data_register(&SPIC, &SPI_ADC);
}

void tc_callback(void)
{
	LED_Toggle(LED0);
	tc_clear_overflow(&TCC0);
	struct ra915struct frame = {
		.marker = 0xA5
	};
	frame.data.pmt_current = adc_scan_results[0];
	frame.data.flow_rate = adc_scan_results[1];
	frame.data.pmt_voltage = adc_scan_results[2];
	frame.data.concentration = adcdata - 0x7FFF;
	frame.data.bypass_pressure = adc_scan_results[3];
	frame.data.t_analytical_cell = adc_scan_results[4];
	frame.data.t_selftest_cell = adc_scan_results[5];
	frame.data.pressure_analytical_cell = adc_scan_results[6];
	frame.data.vacuum = adc_scan_results[7];
	frame.data.dilution_pressure = adc_scan_results[8];

	frame.checksum = genchecksum((uint8_t *)&frame.data,21);
	usart_serial_write_packet(&USARTC0, (uint8_t *)&frame, 23);
	//printf(" %d ",frame.checksum);
}
