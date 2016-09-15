#include <asf.h>

uint8_t spi_transfer(SPI_t *spi, uint8_t data)
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) { }
	return spi_get(spi);
}

void setup_init(void)
{
	sysclk_init();
	board_init();
	pmic_init();
	ioport_init();
	spi_master_init(&SPIC);
}

struct spi_device SPI_ADC = {
	.id = SPIC_SS
};

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

ISR(PORTC_INT0_vect)
{
	spi_select_device(&SPIC, &SPI_ADC);
	spi_deselect_device(&SPIC, &SPI_ADC);
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

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();;

	/* Insert application code here, after the board has been initialized. */
	spi_select_device(&SPIC, &SPI_ADC);
	spi_transfer(&SPIC, CONFIG_SPI_MASTER_DUMMY);
	spi_deselect_device(&SPIC, &SPI_ADC);
}
