#include <asf.h>
#include "setup.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	//do nothing as timer counter working inside interrupt itself
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
	} while (1);
}
