#include <asf.h>
#include "setup.h"
#include "ad7705.h"
#include "interrupt.h"

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

void setup_init(void)
{
	sysclk_init();
	board_init();
	pmic_init();
	ioport_init();
	spi_master_init(&SPIC);
	gfx_mono_init();
}

void spi_configure(void)
{
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 50000, 0);
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

void interrupt_configure(void)
{
	ISR_init();
	irq_initialize_vectors();
	cpu_irq_enable();
}

void setup_configure(void)
{
	ioport_configure();
	spi_configure();
	interrupt_configure();
}

void setup_enable(void)
{
	spi_enable(&SPIC);
	ad7705_enable();
}

void ad7705_enable(void)
{
	/*spi_write_packet(&SPIC, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5);
	spi_write_packet(&SPIC, (uint8_t[]){0x20,0x0C,0x10,0x04}, 4);
	spi_write_packet(&SPIC, (uint8_t[]){0x60,0x18,0x3A,0x00}, 4);
	spi_write_packet(&SPIC, (uint8_t[]){0x70,0x89,0x78,0xD7}, 4);*/

	ad7705_send_reset(&SPIC, &SPI_ADC);
	ad7705_set_clock_register(&SPIC, &SPI_ADC, 0x0C);
	ad7705_set_setup_register(&SPIC, &SPI_ADC, 0x04);
	ad7705_set_scale_register(&SPIC, &SPI_ADC, 0x183A00);
	ad7705_set_offset_register(&SPIC, &SPI_ADC, 0x8978D7);
}