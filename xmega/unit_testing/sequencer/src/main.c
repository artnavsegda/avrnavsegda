#include <asf.h>
#include "i2c.h"
#include "pca9557.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
	gfx_mono_init();
}

void twi_configure(void)
{
	twi_master_options_t opt = {
		.speed = 50000,
		.chip  = 0x50
	};
	twi_master_setup(&TWIC, &opt);
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
	twi_configure();
	tc_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIC);
	tc_enable(&TCC0);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
}

