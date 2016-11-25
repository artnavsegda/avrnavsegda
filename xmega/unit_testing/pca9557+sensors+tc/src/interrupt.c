#include <asf.h>
#include "interrupt.h"
#include "pca9557.h"

void tc_callback(void)
{
	static int i = 0;
	pca9557_toggle_pin_level(0x1a, i++);
	if (i == 8)
		i = 0;
	LED_Toggle(LED0);
	tc_clear_overflow(&TCC0);
}
