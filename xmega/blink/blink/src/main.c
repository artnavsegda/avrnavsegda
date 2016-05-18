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

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();

	/* Insert application code here, after the board has been initialized. */

	#define LED_STATUS_PIN IOPORT_CREATE_PIN( PORTD, 4 )
	#define LED_BOARD_PIN IOPORT_CREATE_PIN( PORTA, 0 )

	ioport_set_pin_dir( LED_STATUS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir( LED_BOARD_PIN, IOPORT_DIR_OUTPUT);

	while (1)
	{
		ioport_toggle_pin(LED_STATUS_PIN);
		ioport_toggle_pin(LED_BOARD_PIN);
		delay_ms(500);
	}
}
