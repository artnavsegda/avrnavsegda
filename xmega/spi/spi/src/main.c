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

void mediate(int income);
void interrupt_init(void);
void adc_init(void);

struct spi_device SPI_ADC = {
	//! Board specific select id
	.id = SPIC_SS
};


unsigned int massive[16];
int counter = 0;

void mediate(int income)
{
	massive[counter] = income;
	if (counter++ > 16)
		counter = 0; 
}

ISR(PORTC_INT0_vect)
{
	unsigned int result = 0;
	spi_select_device(&SPIC, &SPI_ADC);
	spi_put(&SPIC,0x08);
	if (spi_get(&SPIC) == 8)
	{
		spi_put(&SPIC,0x38);
		MSB(result) = spi_get(&SPIC);
		LSB(result) = spi_get(&SPIC);
		mediate(result);
	}
	spi_deselect_device(&SPIC, &SPI_ADC);
}

void interrupt_init(void)
{
	ioport_set_pin_dir(J1_PIN4, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(J1_PIN4, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(J1_PIN4, IOPORT_SENSE_FALLING);
	PORTC.INT0MASK = PIN1_bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	cpu_irq_enable();
}

void adc_init(void)
{
	spi_select_device(&SPIC, &SPI_ADC);
	spi_put(&SPIC,0x20);
	spi_put(&SPIC,0x0C);
	spi_put(&SPIC,0x10);
	spi_put(&SPIC,0x40);
	spi_deselect_device(&SPIC, &SPI_ADC);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();
	interrupt_init();
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0);
	spi_enable(&SPIC);
	adc_init();

	/* Insert application code here, after the board has been initialized. */

}
