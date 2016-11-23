#include <asf.h>
#include <stdio.h>
#include "ad7705.h"
#include "interrupt.h"
#include "rolling.h"

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

struct ra915data {
	uint16_t pmt_current;
	uint16_t flow_rate;
	uint16_t pmt_voltage;
	int concentration;
	uint16_t bypass_pressure;
	uint16_t t_analytical_cell;
	uint16_t t_selftest_cell;
	uint16_t pressure_analytical_cell;
	uint16_t vacuum;
	uint16_t dilution_pressure;
	uint8_t status;
};

struct ra915struct {
	uint8_t marker;
	struct ra915data data;
	uint8_t checksum;
};

void tc_callback(void)
{
	LED_Toggle(LED0);
	tc_clear_overflow(&TCC0);
	struct ra915struct ra915data = {
		.marker = 0xA5
	};
	ra915data.data.concentration = adcdata - 0x7FFF;
	usart_serial_write_packet(&USARTC0, (uint8_t *)&ra915data, 23);
}
