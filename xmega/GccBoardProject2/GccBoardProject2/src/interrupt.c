#include <asf.h>
#include "interrupt.h"
#include "sequencer.h"
#include "spi_transfer.h"
#include "rolling.h"
#include "ad7705.h"
#include "modbus.h"

int16_t adc_scan_results[16];
struct massive ad7705_averaging_massive, measurment_averaging_massive, temperature_averaging_massive;
extern struct spi_device SPI_ADC;

void adc_callback(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_adca_scan_channel = 0, current_adcb_scan_channel = 0;
	if (adc == &ADCA) {
		if (ch_mask & ADC_CH0) {
			adc_scan_results[current_adca_scan_channel] = result;
			current_adca_scan_channel++;
			if (current_adca_scan_channel == 8) current_adca_scan_channel = 0;
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
	if (ad7705_get_communication_register(&SPIC, &SPI_ADC) == 8)
		increment(ad7705_averaging_massive,ad7705_get_data_register(&SPIC, &SPI_ADC));
}

void tc_callback(void)
{
	static struct mydatastate primarystate;
	static struct mydatastruct mydata;

	decrement_mode_counter(primarystate);
	increment(measurment_averaging_massive, oversample(ad7705_averaging_massive, 32));
	increment(temperature_averaging_massive, adc_scan_results[3]);
	process_data(mydata,primarystate);
	//display_data(mydata,primarystate);
	send_data(mydata,primarystate);
}
