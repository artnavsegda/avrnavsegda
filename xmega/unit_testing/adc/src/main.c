#include <asf.h>
#include <stdio.h>

static volatile int16_t adc_scan_results[8];

void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_scan_channel = 0;
	// Store the ADC results from the scan in the result array
	if (ch_mask & ADC_CH0) {
		adc_scan_results[current_scan_channel] = result;
		current_scan_channel++;
		// When 8 pins have been scanned the SCAN OFFSET wraps to zero
		if (current_scan_channel == 8) {
			current_scan_channel = 0;
		}
	}
}

void adcch_configure(ADC_t *adc, uint8_t ch_mask)
{
	struct adc_channel_config adcch_conf;
	adcch_read_configuration(adc, ch_mask, &adcch_conf);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_enable_interrupt(&adcch_conf);
	adcch_set_pin_scan(&adcch_conf, 0, 7);
	adcch_write_configuration(adc, ch_mask, &adcch_conf);
}

void adc_configure(ADC_t *adc)
{
	struct adc_config adc_conf;
	adc_read_configuration(adc, &adc_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SWEEP, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_write_configuration(adc, &adc_conf);
	adc_set_callback(adc, &adc_handler);
	adcch_configure(adc,ADC_CH0);
}

void setup_configure(void)
{
	adc_configure(&ADCA);
	//adc_configure(&ADCB);
}

void setup_enable(void)
{
	adc_enable(&ADCA);
	//adc_enable(&ADCB);
}

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
}

int main (void)
{
	char string[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	while (true) {
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adc_scan_results[0], adc_scan_results[1], adc_scan_results[2], adc_scan_results[3]);
		gfx_mono_draw_string(string,8,0,&sysfont);
		snprintf(string,sizeof(string),"%3X %3X %3X %3X", adc_scan_results[4], adc_scan_results[5], adc_scan_results[6], adc_scan_results[7]);
		gfx_mono_draw_string(string,8,8,&sysfont);
		//snprintf(string,sizeof(string),"%3X %3X %3X %3X", adcb_scan_results[0], adcb_scan_results[1], adcb_scan_results[2], adcb_scan_results[3]);
		//gfx_mono_draw_string(string,8,16,&sysfont);
		//snprintf(string,sizeof(string),"%3X %3X %3X %3X", adcb_scan_results[4], adcb_scan_results[5], adcb_scan_results[6], adcb_scan_results[7]);
		//gfx_mono_draw_string(string,8,24,&sysfont);
	}

}
