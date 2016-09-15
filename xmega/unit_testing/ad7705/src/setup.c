#include <asf.h>
#include "setup.h"

extern struct spi_device SPI_ADC;

void setup_init(void)
{
	sysclk_init();
	board_init();
	pmic_init();
	ioport_init();
	spi_master_init(&SPIC);
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
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void ISR_init(void)
{
	PORTC.INT0MASK = PIN1_bm;
	PORTC.INTCTRL = PORT_INT0LVL_HI_gc;
}

void interrupt_configure(void)
{
	ISR_init();
	irq_initialize_vectors();
	cpu_irq_enable();
}

void setup_configure(void)
{
	spi_configure();
	ioport_configure();
	interrupt_configure();
}

void setup_enable(void)
{
	spi_enable(&SPIC);
}