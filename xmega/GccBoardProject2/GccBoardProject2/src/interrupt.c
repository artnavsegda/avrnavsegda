#include <asf.h>
#include "interrupt.h"
#include "sequencer.h"
#include "spi_transfer.h"
#include "rolling.h"
#include "ad7705.h"
#include "modbus.h"
#include "pca9557.h"
#include "drv8832.h"

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
	static struct mydatastate primarystate = {
		.x20_relay = { .address = 0x18, .pin_number = 0 },
		.x19_relay = { .address = 0x19, .pin_number = 0 },
		.x22 = { .address = 0x1a,	.pin_number = 0 },
		.vt4 = { .address = 0x1a, .pin_number = 5 },
		.elemental = { .address = 0x1a, .pin_number = 4 },
		.calibration = { .address = 0x1a, .pin_number = 3 },
		.zero = {	.address = 0x1a, .pin_number = 2 },
		.ignition = {	.address = 0x1a, .pin_number = 1 },
		.cell = {
			.left_out = { .address = 0x18, .pin_number = 7 },
			.right_out = { .address = 0x18, .pin_number = 6 },
			.left_in = { .address = 0x18, .pin_number = 5 },
			.right_in = { .address = 0x18, .pin_number = 4 }
		}
	};
	static struct mydatastruct mydata;

	decrement_mode_counter(primarystate);
	increment(measurment_averaging_massive, oversample(ad7705_averaging_massive, 32));
	increment(temperature_averaging_massive, adc_scan_results[3]);
	process_data(mydata,primarystate);
	//display_data(mydata,primarystate);
	send_data(mydata,primarystate);
}
