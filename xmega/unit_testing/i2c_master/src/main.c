#include <asf.h>
#include "i2c.h"

void setup_init(void)
{
	sysclk_init();
	board_init();
	ioport_init();
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void setup_configure(void)
{
	twi_configure();
}

void setup_enable(void)
{
	twi_master_enable(&TWIE);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	i2c_send(&TWIE, 0x1a, 0x03, 0x00);
	i2c_send(&TWIE, 0x1a, 0x01, 0x00);
}
