#include <asf.h>
#include "interrupt.h"
#include "pca9557.h"

void tc_callback(void)
{
	pca9557_toggle_pin_level(0x1a, 1);
	//LED_Toggle(LED2);
	tc_clear_overflow(&TCC0);
}
