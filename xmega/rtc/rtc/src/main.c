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
	rtc_set_alarm_relative(3000);
	//ioport_toggle_pin(LED1);
}

static void alarm(uint32_t time)
{
	ioport_toggle_pin(LED0);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	pmic_init();
	board_init();
	sysclk_init();
	ioport_init();
	//sleepmgr_init();

	ioport_set_pin_dir(LED0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED3, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(GPIO_PUSH_BUTTON_1,IOPORT_DIR_INPUT);
	ioport_set_pin_mode(GPIO_PUSH_BUTTON_1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc;

	rtc_init();
	rtc_set_callback(alarm);

	cpu_irq_enable();

	//rtc_set_alarm_relative(3000);

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		/* Alarm action is handled in alarm callback so we just go to
		 * sleep here.
		 */
		//sleepmgr_enter_sleep();
	}
}
