#include <asf.h>

void setup_init(void)
{
	sysclk_init();
	board_init();
	pmic_init();
	ioport_init();
	gfx_mono_init();
}

void ioport_configure(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP);
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_0, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING);
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING);
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

ISR(PORTF_INT0_vect)
{
	//
}

ISR(PORTF_INT1_vect)
{
	//
}

void ISR_init(void)
{
	PORTF.INT0MASK = PIN1_bm;
	PORTF.INT1MASK = PIN2_bm;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc;
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
	interrupt_configure();
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	setup_init();
	setup_configure();

	/* Insert application code here, after the board has been initialized. */
}
