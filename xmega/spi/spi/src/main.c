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

#define CS_PIN IOPORT_CREATE_PIN(PORTC, 4)
#define MOSI_PIN IOPORT_CREATE_PIN(PORTC, 5)
#define MISO_PIN IOPORT_CREATE_PIN(PORTC, 6)
#define SCK_PIN IOPORT_CREATE_PIN(PORTC, 7)

struct spi_device SPI_ADC = {
	//! Board specific select id
	.id = CS_PIN
};

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();
	ioport_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_set_pin_dir(CS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(MOSI_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(MISO_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SCK_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(MISO_PIN, IOPORT_MODE_PULLUP);
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0);
	spi_enable(&SPIC);

	spi_select_device(&SPIC, &SPI_ADC);
	spi_put(&SPIC,0x20);
	spi_put(&SPIC,0x0C);
	spi_put(&SPIC,0x10);
	spi_put(&SPIC,0x40);
	spi_deselect_device(&SPIC, &SPI_ADC);

}
