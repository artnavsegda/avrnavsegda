#include <asf.h>
#include "setup.h"
#include "interrupt.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	pmic_init();
	gfx_mono_init();
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
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_write_configuration(adc, &adc_conf);
	adc_set_callback(adc, &adc_callback);
	adcch_configure(adc,ADC_CH0);
}

void tc_configure(void)
{
	tc_set_overflow_interrupt_callback(&TCC0, tc_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void ioport_configure(void)
{
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	adc_configure(&ADCA);
	adc_configure(&ADCB);
	twi_configure();
	ioport_configure();
	tc_configure();
}

void setup_enable(void)
{
	adc_enable(&ADCA);
	adc_start_conversion(&ADCA, ADC_CH0);
	adc_enable(&ADCB);
	adc_start_conversion(&ADCB, ADC_CH0);
	twi_master_enable(&TWIE);
	cpu_irq_enable();
	tc_enable(&TCC0);
}