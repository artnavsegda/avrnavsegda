#include <asf.h>
#include "setup.h"

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_enable();
	setup_configure();

	/* Insert application code here, after the board has been initialized. */

}
