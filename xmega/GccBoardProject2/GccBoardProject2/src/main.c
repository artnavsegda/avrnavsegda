#include <asf.h>
#include "main.h"

int main(void)
{
	setup();

	/* Insert application code here, after the board has been initialized. */
	//do nothing as timer counter working inside interrupt itself
	do {
		/* Go to sleep, everything is handled by interrupts. */
		//sleepmgr_enter_sleep();
	} while (1);
}
