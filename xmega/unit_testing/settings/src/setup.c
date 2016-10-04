#include <asf.h>
#include "i2c.h"
#include "setup.h"

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	spi_master_init(&SPIC);
	gfx_mono_init();
}

void spi_configure(void)
{
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 50000, 0);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	sysclk_enable_peripheral_clock(&TWIE);
	twi_master_setup(&TWIE, &opt);
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
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	ioport_configure();
	spi_configure();
	twi_configure();
}

void setup_enable(void)
{
	spi_enable(&SPIC);
	twi_master_enable(&TWIE);
	ad7705_enable();
}

void ad7705_enable(void)
{
	ad7705_send_reset(&SPIC, &SPI_ADC);
	ad7705_set_setup_register(&SPIC, &SPI_ADC, i2c_read(&TWIE,0x08,I2C_AD7705_SETUP_REGISTER));
	ad7705_set_clock_register(&SPIC, &SPI_ADC, i2c_read(&TWIE,0x08,I2C_AD7705_CLOCK_REGISTER));
	ad7705_set_scale_register(&SPIC, &SPI_ADC, 0x183A00);
	ad7705_set_offset_register(&SPIC, &SPI_ADC, 0x8978D7);
}