#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"
#include "interrupt.h"

void setup_init(void)
{
	pmic_init();
	sysclk_init();
	board_init();
	ioport_init();
}

void tc_configure(void)
{
	cpu_irq_enable();
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, tc_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_set_resolution(&TCC0, 31250);
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void setup_configure(void)
{
	tc_configure();
	twi_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIE);
	pca9557_init(0x18);
	pca9557_init(0x19);
	pca9557_init(0x1a);
}
