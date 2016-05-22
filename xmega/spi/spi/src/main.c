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

#define LED_STATUS_PIN IOPORT_CREATE_PIN( PORTD, 4 )
#define MY_LED IOPORT_CREATE_PIN( PORTE, 4 )
#define MY_BUTTON IOPORT_CREATE_PIN( PORTF, 1 )

void mediate(int income);
void interrupt_init(void);
void adc_init(void);

char string[10];

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

int num = 0;

ISR(PORTC_INT0_vect)
{
	unsigned int result = 0;
	LED_Toggle(LED2);
	snprintf(string,10,"%d",num++);
	//gfx_mono_draw_string(string,10,10,&sysfont);
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

ISR(PORTF_INT0_vect)
{
	LED_Toggle(LCD_BACKLIGHT_ENABLE_PIN);
	snprintf(string,10,"%d",num++);
	gfx_mono_draw_string(string,10,10,&sysfont);
}

void interrupt_init(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_1,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	PORTC.INT0MASK = PIN1_bm;
	PORTF.INT0MASK = PIN1_bm;
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc;
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
	uint8_t read_buffer[3] = {0x00, 0x00, 0x00};
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	ioport_init();
	board_init();
	gfx_mono_init();
	//interrupt_init();

	ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	ioport_configure_port_pin(&PORTC, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_port_pin(&PORTC, PIN6_bm, IOPORT_DIR_INPUT);
	ioport_configure_port_pin(&PORTC, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);

	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0);
	spi_enable(&SPIC);
	adc_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	gfx_mono_draw_string("Hello world",10,10,&sysfont);

	unsigned int result = 0;

	while (1)
	{
		spi_select_device(&SPIC, &SPI_ADC);
		spi_put(&SPIC,0x08);
		if (spi_get(&SPIC) == 8)
		{
			spi_put(&SPIC,0x38);
			spi_read_packet(&SPIC, &read_buffer, 2);
			MSB(result) = read_buffer[0];
			LSB(result) = read_buffer[1];
			snprintf(string, 10, " %2.2X%2.2X", read_buffer[0],read_buffer[1]);
			gfx_mono_draw_string(string,10,10,&sysfont);
		}
		ioport_toggle_pin(LED_STATUS_PIN);
		delay_ms(500);
	}

}
