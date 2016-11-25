#include <asf.h>
#include "setup.h"

sensor_t barometer;

void setup_init(void)
{
	board_init();
	sysclk_init();
	ioport_init();
	gfx_mono_init();
}

void twi_configure(void)
{
	twi_master_options_t opt = { .speed = 50000 };
	twi_master_setup(&TWIE, &opt);
}

void sensor_configure(void)
{
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
}

void setup_configure(void)
{
	twi_configure();
	sensor_configure();
}