#include <asf.h>
#include "ad7705.h"
#include "setup.h"
#include "interrupt.h"
#include "pca9557.h"
#include "ra915.h"

sensor_t barometer;

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

const usart_serial_options_t usart_serial_options = {
	.baudrate     = 9600,
	.charlength   = USART_CHSIZE_8BIT_gc,
	.paritytype   = USART_PMODE_DISABLED_gc,
	.stopbits     = false
};

void setup_init(void)
{
	pmic_init();
	sysclk_init();
	board_init();
	ioport_init();
	spi_master_init(&SPIC);
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
	adc_set_clock_rate(&adc_conf, 2000UL);
	adc_write_configuration(adc, &adc_conf);
	adc_set_callback(adc, &adc_callback);
	adcch_configure(adc,ADC_CH0);
}

void ioport_configure(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(J1_PIN6, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(J1_PIN4, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(J1_PIN5, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(J1_PIN7, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(J1_PIN6, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING);
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void sensor_configure(void)
{
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
}

void spi_configure(void)
{
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 50000, 0);
}

void tc_configure(void)
{
	cpu_irq_enable();
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, tc_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	//tc_write_clock_source(&TCC0, TC_CLKSEL_DIV4_gc);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void interrupt_configure(void)
{
	ISR_init();
	irq_initialize_vectors();
	cpu_irq_enable();
}

void usart_configure(void)
{
	stdio_serial_init(&USARTC0, &usart_serial_options);
	usart_set_rx_interrupt_level(&USARTC0, USART_INT_LVL_LO);
}

void setup_configure(void)
{
	usart_configure();
	ioport_configure();
	adc_configure(&ADCA);
	adc_configure(&ADCB);
	spi_configure();
	twi_configure();
	sensor_configure();
	tc_configure();
	interrupt_configure();
}

void pca9557_configure(void)
{
	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 3, PCA9557_DIR_OUTPUT);
	//pca9557_set_pin_dir(0x1a, 4, PCA9557_DIR_OUTPUT);
	//pca9557_set_pin_dir(0x1a, 5, PCA9557_DIR_OUTPUT);
	//pca9557_set_pin_dir(0x1a, 6, PCA9557_DIR_OUTPUT);
	//pca9557_set_pin_dir(0x1a, 7, PCA9557_DIR_OUTPUT);
}

void setup_enable(void)
{
	twi_master_enable(&TWIE);
	pca9557_init(0x18);
	pca9557_init(0x19);
	pca9557_init(0x1a);
	//pca9557_configure();
	ra915init();
	adc_enable(&ADCA);
	adc_start_conversion(&ADCA, ADC_CH0);
	adc_enable(&ADCB);
	adc_start_conversion(&ADCB, ADC_CH0);
	spi_enable(&SPIC);
	ad7705_enable();
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV64_gc);
}

void ad7705_enable(void)
{
	/*spi_write_packet(&SPIC, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5);
	spi_write_packet(&SPIC, (uint8_t[]){0x20,0x0C,0x10,0x04}, 4);
	spi_write_packet(&SPIC, (uint8_t[]){0x60,0x18,0x3A,0x00}, 4);
	spi_write_packet(&SPIC, (uint8_t[]){0x70,0x89,0x78,0xD7}, 4);*/

	ad7705_send_reset(&SPIC, &SPI_ADC);
	ad7705_set_clock_register(&SPIC, &SPI_ADC, 0x0C);
	ad7705_set_setup_register(&SPIC, &SPI_ADC, 0x40);
	//ad7705_set_scale_register(&SPIC, &SPI_ADC, 0x183A00);
	//ad7705_set_offset_register(&SPIC, &SPI_ADC, 0x8978D7);
}