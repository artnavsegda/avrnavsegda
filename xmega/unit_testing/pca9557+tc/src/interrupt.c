#include <asf.h>
#include "interrupt.h"

void tc_callback(void)
{
	pca9557_toggle_pin_level(0x1a, 0);
	tc_clear_overflow(&TCC0);
}
