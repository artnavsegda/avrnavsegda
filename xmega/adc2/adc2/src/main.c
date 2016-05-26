/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

void adc_init(void);

int16_t ch0_result;
int16_t ch1_result;

static void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	switch (ch_mask)
	{
	case ADC_CH0:
		ch0_result = result;
		break;
	case ADC_CH1:
		ch1_result = result;
		break;
	default:
		break;
	}
	return;
}

void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	adc_read_configuration(&ADCA, &adc_conf);
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_BANDGAP);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_set_callback(&ADCA, &adc_handler);
	adc_write_configuration(&ADCA, &adc_conf);

	adcch_enable_interrupt(&adcch_conf);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);

	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(&ADCA, ADC_CH1, &adcch_conf);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	adc_init();
	pmic_init();

	/* Insert application code here, after the board has been initialized. */
	cpu_irq_enable();
	adc_enable(&ADCA);

	do 
	{
	} while (true);
}
