#include <asf.h>
#include <stdio.h>
#include "setup.h"
#include "settings.h"

int main (void)
{

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	//do nothing as timer counter working inside interrupt itself
}
