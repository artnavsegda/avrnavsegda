#include <asf.h>
#include "i2c.h"

void setup_init(void)
{
	twi_options_t m_options = {
		.speed     = 50000,
		.chip      = 0x50,
		.speed_reg = TWI_BAUD(sysclk_get_cpu_hz(), 50000)
	};

	sysclk_init();
	board_init();
	ioport_init();
	twi_master_init(&TWIC, &m_options);
}

void twi_configure(void)
{
	twi_master_options_t opt = {
		.speed = 50000,
		.chip  = 0x50
	};
	twi_master_setup(&TWIC, &opt);
}

void setup_configure(void)
{
	twi_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIC);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
}