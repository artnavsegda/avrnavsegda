#include <asf.h>
#include "i2c.h"
#include "setup.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void ioport_configure(void)
{
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}

void setup_configure(void)
{
	twi_configure();
	ioport_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIE);
}
