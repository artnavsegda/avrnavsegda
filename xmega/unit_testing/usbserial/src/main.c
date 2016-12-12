#include <asf.h>
#include "setup.h"

uint8_t buffer[100];
int irqcount = 0;
char string[100];

void my_callback_rx_notify(uint8_t port)
{
	LED_Toggle(LED0);
	irqcount++;
	int i = udi_cdc_get_nb_received_data();
	udi_cdc_read_buf(buffer,i);
	gfx_mono_draw_string(buffer,10,10,&sysfont);
	sprintf(string,"%d",irqcount);
	gfx_mono_draw_string(string,20,20,&sysfont);
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
