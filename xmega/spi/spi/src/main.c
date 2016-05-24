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
#include "stdio.h"

void mediate(int income);
void interrupt_init(void);
void adc_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message);

char string[10];

struct spi_device SPI_ADC = {
	//! Board specific select id
	.id = SPIC_SS
};

twi_master_options_t opt = {
	.speed = 50000,
};

#define AVERAGING 32
#define STEP 5
#define REFRESH 1000

unsigned int massive[AVERAGING];
int counter = 0;

void mediate(int income)
{
	massive[counter] = income;
	if (counter++ > AVERAGING)
		counter = 0; 
}

unsigned int result = 0;

ISR(PORTC_INT0_vect)
{
	LED_Toggle(LED2);
	spi_select_device(&SPIC, &SPI_ADC);
	spi_gut(&SPIC,0x08);
	if (spi_gut(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		spi_gut(&SPIC,0x38);
		MSB(result) = spi_gut(&SPIC,0xFF);
		LSB(result) = spi_gut(&SPIC,0xFF);
		mediate(result);
		//snprintf(string, sizeof(string), "%5ld ", (long)result-0x17CC);
		//gfx_mono_draw_string(string,10,10,&sysfont);
	}
	spi_deselect_device(&SPIC, &SPI_ADC);
}

ISR(PORTF_INT1_vect)
{
	i2c_send(&TWIE, 0x18, "\x01\x40");
}

ISR(PORTF_INT0_vect)
{
	i2c_send(&TWIE, 0x18, "\x01\x80");
}

void interrupt_init(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING);
	PORTC.INT0MASK = PIN1_bm;
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	cpu_irq_enable();
}

void adc_init(void)
{
	spi_select_device(&SPIC, &SPI_ADC);
	spi_write_packet(&SPIC, "\x20\x0C\x10\x04", 4);
	spi_write_packet(&SPIC, "\x60\x18\x3A\x00", 4);
	spi_write_packet(&SPIC, "\x70\x89\x78\xD7", 4);
	spi_deselect_device(&SPIC, &SPI_ADC);
}

uint8_t spi_gut(SPI_t *spi, uint8_t data)
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) {
	}
	return spi_get(spi);
}

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message)
{
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = (void *)message, // transfer data source buffer
		.length       = sizeof(message)  // transfer data size (bytes)
	};
	return twi_master_write(twi, &packet);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	ioport_init();
	board_init();
	twi_master_setup(&TWIE, &opt);
	gfx_mono_init();
	interrupt_init();

	ioport_set_pin_dir(SPIC_SS, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPIC_MOSI, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(SPIC_MISO, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(SPIC_SCK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(SPIC_MISO, IOPORT_MODE_PULLUP);

	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0);
	spi_enable(&SPIC);
	adc_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	i2c_send(&TWIE, 0x18, "\x03\x3f");
	//gfx_mono_draw_string("Hello world",10,10,&sysfont);

	long x = 0;
	int i;

	while (1)
	{
		x = 0;
		for(i=0;i<AVERAGING;i++)
			x=x+massive[i];
		snprintf(string, sizeof(string), "  %5ld ", (long)result-0x17CC);
		gfx_mono_draw_string(string,10,10,&sysfont);
		snprintf(string, sizeof(string), "  %5ld ", (x >> STEP)-0x17CC);
		gfx_mono_draw_string(string,10,20,&sysfont);
		delay_ms(REFRESH);
	}

}
