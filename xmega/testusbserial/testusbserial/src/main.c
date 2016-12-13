#include <asf.h>
#include "setup.h"

int irqcount = 0;
uint8_t buffer[100];

void my_callback_rx_notify(uint8_t port)
{
	LED_Toggle(LED0);
	irqcount++;
	printf("IRQ:%d:length=%d ",irqcount,udi_cdc_get_nb_received_data());
	while (udi_cdc_is_rx_ready())
		printf(" 0x%X", udi_cdc_getc());
	printf("\n\r");
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		sleepmgr_enter_sleep();
	}
}
