#include <asf.h>
#include "loop.h"
#include "setup.h"
#include "interrupt.h"
#include "pca9557.h"
#include "ad7705.h"

struct spi_device SPI_ADC = { .id = SPIC_SS };

void interrupt_configure(void)
{
	ISR_init();
	cpu_irq_enable();
}

void tc_configure(void)
{
	tc_set_overflow_interrupt_callback(&TCC0, sequence_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);
}

void spi_configure(void)
{
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 50000, 0);
}

void ioport_configure(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_0, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING);
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
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
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_write_configuration(adc, &adc_conf);
	adc_set_callback(adc, &adc_handler);
	adcch_configure(adc,ADC_CH0);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000	};
	twi_master_setup(&TWIE, &opt);
}

void ad7705_enable(void)
{
	ad7705_send_reset(&SPIC, &SPI_ADC);
	ad7705_set_clock_register(&SPIC, &SPI_ADC, 0x0C);
	ad7705_set_setup_register(&SPIC, &SPI_ADC, 0x04);
	ad7705_set_scale_register(&SPIC, &SPI_ADC, 0x183A00);
	ad7705_set_offset_register(&SPIC, &SPI_ADC, 0x8978D7);
}

void setup_configure(void)
{
	tc_configure();
	spi_configure();
	ioport_configure();
	interrupt_configure();
	adc_configure(&ADCA);
	adc_configure(&ADCB);
	twi_configure();
}

void setup_enable(void)
{
	tc_enable(&TCC0);
	spi_enable(&SPIC);
	adc_enable(&ADCA);
	adc_enable(&ADCB);
	twi_master_enable(&TWIE);
	ad7705_enable();
}

void setup_init(void)
{
	sysclk_init();
	board_init();
	pmic_init();
	ioport_init();
	spi_master_init(&SPIC);
	gfx_mono_init();
	sensor_bus_init(&TWIE, 50000);
	pca9557_init(0x18);
	pca9557_init(0x19);
	pca9557_init(0x1a);
}

void setup(void)
{
	//struct mysettings settings;
	setup_init();
	setup_configure();
	setup_enable();
	//if (!recieve_settings_from_slave(settings))
	//	default_settings(settings);
	//setup_settings();
}