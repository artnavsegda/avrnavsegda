#include <asf.h>
#include "setup.h"

uint8_t buffer[100];

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		udi_cdc_read_buf(buffer,10);
		LED_Toggle(LED0);
		delay_ms(100);
		//sleepmgr_enter_sleep();
	}
}
