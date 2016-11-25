#include <asf.h>
#include "setup.h"
#include "pca9557.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 3, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 4, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 5, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 6, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 7, PCA9557_DIR_OUTPUT);
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
	} while (1);
}
