#include <asf.h>

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	adc_read_configuration(&ADCA, &adc_conf);
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
	ADC_REF_BANDGAP);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adc_write_configuration(&ADCA, &adc_conf);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
}

int main (void)
{
	uint16_t result;
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	adc_init();
	adc_enable(&ADCA);

	/* Insert application code here, after the board has been initialized. */
	while (true)
	{
		adc_start_conversion(&ADCA, ADC_CH0);
		adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
		result = adc_get_result(&ADCA, ADC_CH0); 
	}
}
