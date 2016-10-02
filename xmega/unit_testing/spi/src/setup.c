#include <asf.h>
#include "setup.h"

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
	spi_master_init(&SPIC);
}

void spi_configure(void)
{
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 50000, 0);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
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
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	ioport_configure();
	spi_configure();
}

void setup_enable(void)
{
	spi_enable(&SPIC);
	twi_master_enable(&TWIE);
}