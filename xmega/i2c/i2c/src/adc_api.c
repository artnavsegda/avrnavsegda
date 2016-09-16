
#include <asf.h>
#include "adc_api.h"

const float popugai = (3.3/1.6)/4095;
const int adczero = 180;

static volatile int16_t adca_scan_results[8];
static volatile int16_t adcb_scan_results[8];

static void adca_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_scan_channel = 0;
	// Store the ADC results from the scan in the result array
	if (ch_mask & ADC_CH0) {
		adca_scan_results[current_scan_channel] = result;
		current_scan_channel++;
		// When 8 pins have been scanned the SCAN OFFSET wraps to zero
		if (current_scan_channel == 8) {
			current_scan_channel = 0;
		}
	}
}

static void adcb_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	static uint8_t current_scan_channel = 0;
	// Store the ADC results from the scan in the result array
	if (ch_mask & ADC_CH0) {
		adcb_scan_results[current_scan_channel] = result;
		current_scan_channel++;
		// When 8 pins have been scanned the SCAN OFFSET wraps to zero
		if (current_scan_channel == 8) {
			current_scan_channel = 0;
		}
	}
}

void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos)
{
	struct adc_channel_config adcch_conf;
	adcch_read_configuration(adc, ch_mask, &adcch_conf);
	adcch_set_input(&adcch_conf, pos, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(adc, ch_mask, &adcch_conf);
}

void analogInputScan(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos)
{
	struct adc_channel_config adcch_conf;
	adcch_read_configuration(adc, ch_mask, &adcch_conf);
	adcch_set_input(&adcch_conf, pos, ADCCH_NEG_NONE, 1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_enable_interrupt(&adcch_conf);
	adcch_set_pin_scan(&adcch_conf, 0, 7);
	adcch_write_configuration(adc, ch_mask, &adcch_conf);
}

void adc_init(void)
{
	struct adc_config adca_conf, adcb_conf;
	adc_read_configuration(&ADCA, &adca_conf);
	adc_read_configuration(&ADCB, &adcb_conf);
	adc_set_conversion_parameters(&adca_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_parameters(&adcb_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_trigger(&adca_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_conversion_trigger(&adcb_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adca_conf, 200000UL);
	adc_set_clock_rate(&adcb_conf, 200000UL);
	analogInput(&ADCB, ADC_CH0, ADCCH_POS_PIN0); // lamp voltage
	analogInput(&ADCB, ADC_CH1, ADCCH_POS_PIN1); // lamp current
	analogInput(&ADCB, ADC_CH2, ADCCH_POS_PIN2); // flow
	analogInput(&ADCB, ADC_CH3, ADCCH_POS_PIN3); // temperature
	analogInput(&ADCA, ADC_CH0, ADCCH_POS_PIN4); // p1 x14
	analogInput(&ADCA, ADC_CH1, ADCCH_POS_PIN5); // p2 x15
	analogInput(&ADCA, ADC_CH2, ADCCH_POS_PIN6); // p3 x16
	analogInput(&ADCA, ADC_CH3, ADCCH_POS_PIN7); // p4 x17
	adc_write_configuration(&ADCA, &adca_conf);
	adc_write_configuration(&ADCB, &adcb_conf);
	adc_enable(&ADCA);
	adc_enable(&ADCB);
}

uint16_t analogRead(ADC_t *adc, uint8_t ch_mask)
{
	adc_start_conversion(adc, ch_mask);
	adc_wait_for_interrupt_flag(adc, ch_mask);
	return adc_get_result(adc, ch_mask);
}

float analogVoltage(ADC_t *adc, uint8_t ch_mask)
{
	return (analogRead(adc, ch_mask)-adczero)*popugai;
}

void adc_init_scan(void)
{
	struct adc_config adca_conf, adcb_conf;
	adc_read_configuration(&ADCA, &adca_conf);
	adc_read_configuration(&ADCB, &adcb_conf);
	adc_set_conversion_parameters(&adca_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_parameters(&adcb_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_trigger(&adca_conf, ADC_TRIG_EVENT_SWEEP, 1, 0);
	adc_set_conversion_trigger(&adca_conf, ADC_TRIG_EVENT_SWEEP, 1, 0);
	adc_set_clock_rate(&adca_conf, 200000UL);
	adc_set_clock_rate(&adcb_conf, 200000UL);
	adc_write_configuration(&ADCA, &adca_conf);
	adc_write_configuration(&ADCB, &adcb_conf);

	adc_set_callback(&ADCA, &adca_handler);
	adc_set_callback(&ADCB, &adcb_handler);

	analogInputScan(&ADCA, ADC_CH0, ADCCH_POS_PIN0);
	analogInputScan(&ADCB, ADC_CH0, ADCCH_POS_PIN0);

	adc_enable(&ADCA);
	adc_enable(&ADCB);
}