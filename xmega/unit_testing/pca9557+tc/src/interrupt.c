#include <asf.h>
#include "interrupt.h"

void tc_callback(void)
{
	LED_Toggle(LED2);
	tc_clear_overflow(&TCC0);
}
