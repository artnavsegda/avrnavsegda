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

ISR(PORTF_INT0_vect)
{
	ioport_toggle_pin(LCD_BACKLIGHT_ENABLE_PIN);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	ioport_init();
	board_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_set_pin_dir(LED0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED3, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LCD_BACKLIGHT_ENABLE_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_1,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc;
	irq_initialize_vectors()
	cpu_irq_enable();

	//bool value;

	while (1)
	{
		//value = ioport_get_pin_level(GPIO_PUSH_BUTTON_1);
		//ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN,value);
		ioport_toggle_pin(LED0);
		ioport_toggle_pin(LED1);
		ioport_toggle_pin(LED2);
		ioport_toggle_pin(LED3);
		delay_ms(500);
	}
}
