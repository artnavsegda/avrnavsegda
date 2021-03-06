#include <asf.h>
#include <stdio.h>
#include "interrupt.h"

int16_t adc_scan_results[16];

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

void tc_callback(void)
{
	char string[20];
	LED_Toggle(LED2);
	snprintf(string,sizeof(string),"%03X %03X %03X %03X %03X", adc_scan_results[0], adc_scan_results[1], adc_scan_results[2], adc_scan_results[3], adc_scan_results[4]);
	gfx_mono_draw_string(string,8,0,&sysfont);
	snprintf(string,sizeof(string),"%03X %03X %03X %03X %03X", adc_scan_results[5], adc_scan_results[6], adc_scan_results[7], adc_scan_results[8], adc_scan_results[9]);
	gfx_mono_draw_string(string,8,8,&sysfont);
	snprintf(string,sizeof(string),"%03X %03X %03X %03X %03X", adc_scan_results[10], adc_scan_results[11], adc_scan_results[12], adc_scan_results[13], adc_scan_results[14]);
	gfx_mono_draw_string(string,8,16,&sysfont);
	snprintf(string,sizeof(string),"%03X", adc_scan_results[15]);
	gfx_mono_draw_string(string,8,24,&sysfont);
	tc_clear_overflow(&TCC0);
}
