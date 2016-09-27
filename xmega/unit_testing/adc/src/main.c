#include <asf.h>
#include <stdio.h>
#include "setup.h"

static volatile int16_t adca_scan_results[8];
static volatile int16_t adcb_scan_results[8];

void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_adca_scan_channel = 0, current_adcb_scan_channel = 0;
	// Store the ADC results from the scan in the result array
	if (adc == &ADCA) {
		if (ch_mask & ADC_CH0) {
			adca_scan_results[current_adca_scan_channel] = result;
			current_adca_scan_channel++;
			// When 8 pins have been scanned the SCAN OFFSET wraps to zero
			if (current_adca_scan_channel == 8) {
				current_adca_scan_channel = 0;
			}
		}
	}
	else if (adc == &ADCB) {
		if (ch_mask & ADC_CH0) {
			adcb_scan_results[current_adcb_scan_channel] = result;
			current_adcb_scan_channel++;
			// When 8 pins have been scanned the SCAN OFFSET wraps to zero
			if (current_adcb_scan_channel == 8) {
				current_adcb_scan_channel = 0;
			}
		}
	}
}

float adc_voltage(uint16_t adcvalue)
{
	const float popugai = (3.3/1.6)/4095;
	const int adczero = 180;
	return (adcvalue-adczero)*popugai;
}

int main (void)
{
	char string[20];

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adca_scan_results[0], adca_scan_results[1], adca_scan_results[2], adca_scan_results[3]);
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adca_scan_results[4], adca_scan_results[5], adca_scan_results[6], adca_scan_results[7]);
		gfx_mono_draw_string(string,8,8,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adcb_scan_results[0], adcb_scan_results[1], adcb_scan_results[2], adcb_scan_results[3]);
		gfx_mono_draw_string(string,8,16,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adcb_scan_results[4], adcb_scan_results[5], adcb_scan_results[6], adcb_scan_results[7]);
		gfx_mono_draw_string(string,8,24,&sysfont);
		//delay_ms(500);
	}
}
