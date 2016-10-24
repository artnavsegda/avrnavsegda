#include <asf.h>
#include "setup.h"
#include "interrupt.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	pmic_init();
	gfx_mono_init();
}

void setup_enable(void)
{
	cpu_irq_enable();
	tc_enable(&TCC0);
}

void tc_configure(void)
{
	tc_set_overflow_interrupt_callback(&TCC0, tc_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);
}

void ioport_configure(void)
{
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
}


void setup_configure(void)
{
	ioport_configure();
	tc_configure();
}