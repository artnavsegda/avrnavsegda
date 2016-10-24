#include <asf.h>
#include <stdio.h>
#include "interrupt.h"

void tc_callback(void)
{
	LED_Toggle(LED2);
}
