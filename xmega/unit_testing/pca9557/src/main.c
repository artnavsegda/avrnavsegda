#include <asf.h>
#include "i2c.h"
#include "pca9557.h"

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
	pca9557_init(0x1a);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	//pca9557_set_pin_level(0x1a, 0, true);
	pca9557_set_pin_level(0x1a, 1, true);
	pca9557_set_pin_level(0x1a, 2, true);
	//pca9557_set_pin_level(0x1a, 3, true);
	//pca9557_set_pin_level(0x1a, 4, true);
	//pca9557_set_pin_level(0x1a, 5, true);
	//pca9557_set_pin_level(0x1a, 6, true);
	//pca9557_set_pin_level(0x1a, 7, true);

}