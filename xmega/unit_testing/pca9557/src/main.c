#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"

struct pca9557_pin ignition = {
	.address = 0x18,
	.pin_number = 6,
	.direction = PCA9557_DIR_OUTPUT
};

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	pca9557_set_pin_dir(ignition.address, ignition.pin_number, ignition.direction);

	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 3, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 4, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 5, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 6, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 7, PCA9557_DIR_OUTPUT);

	pca9557_set_pin_dir(ignition.address, ignition.pin_number, true);
	delay_ms(1000);
	pca9557_set_pin_dir(ignition.address, ignition.pin_number, false);

	pca9557_set_pin_level(0x1a, 0, false);
	pca9557_set_pin_level(0x1a, 1, true);
	pca9557_set_pin_level(0x1a, 2, false);
	pca9557_set_pin_level(0x1a, 3, true);
	pca9557_set_pin_level(0x1a, 4, false);
	pca9557_set_pin_level(0x1a, 5, true);
	pca9557_set_pin_level(0x1a, 6, false);
	pca9557_set_pin_level(0x1a, 7, true);
}
