void setup_init(void)
{
	board_init();
	sysclk_init();
	ioport_init();
	gfx_mono_init();
}

void twi_configure(void)
{
	twi_options_t twi_options = { .speed = 400000, .chip = 0 };
	twi_master_setup(&TWIE, &twi_options);
}

void sensor_configure(void)
{

}

void setup_configure(void)
{
	twi_configure();
	sensor_configure();
}