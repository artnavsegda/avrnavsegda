#include <asf.h>
#include "setup.h"
#include "pca9557.h"
#include "ra915.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
		ra915recieve();
	} while (1);
}
