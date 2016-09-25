#include <asf.h>
#include "setup.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
}

void setup_enable(void)
{
	tc_enable(&TCC0);
}

void sequence_callback(void)
{

}

void tc_configure(void)
{
	tc_set_overflow_interrupt_callback(&TCC0, sequence_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);
}

void setup_configure(void)
{
	tc_configure();
}