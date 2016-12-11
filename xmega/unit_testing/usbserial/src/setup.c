#include <asf.h>
#include "setup.h"

void setup_init(void)
{
	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	board_init();
}

void ioport_configure(void)
{
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	ioport_configure();
}

void setup_enable(void)
{
	udc_start();
}