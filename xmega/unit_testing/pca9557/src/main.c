#include <asf.h>
#include "i2c.h"
#include "pca9557.h"
#include "setup.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_level(0x1a, 0, false);
	pca9557_set_pin_level(0x1a, 1, false);
	pca9557_set_pin_level(0x1a, 2, false);
	//pca9557_set_pin_level(0x1a, 3, true);
	//pca9557_set_pin_level(0x1a, 4, true);
	//pca9557_set_pin_level(0x1a, 5, true);
	//pca9557_set_pin_level(0x1a, 6, true);
	//pca9557_set_pin_level(0x1a, 7, true);
}
