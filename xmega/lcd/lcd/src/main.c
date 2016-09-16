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

char string[20];

ISR(PORTF_INT0_vect)
{
	LED_Toggle(LCD_BACKLIGHT_ENABLE_PIN);
}

ISR(PORTF_INT1_vect)
{
	LED_Toggle(LCD_BACKLIGHT_ENABLE_PIN);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	ioport_init();
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_1,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_2,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_2, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING);
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
	irq_initialize_vectors();
	cpu_irq_enable();
	gfx_mono_init();

	/* Insert application code here, after the board has been initialized. */

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	while (true) {
		//gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
		//gfx_mono_draw_pixel(10 , 10, GFX_PIXEL_SET);
		snprintf(string,sizeof(string),"Hello world");
		gfx_mono_draw_string(string,10,10,&sysfont);
		delay_ms(500);
	}
}
