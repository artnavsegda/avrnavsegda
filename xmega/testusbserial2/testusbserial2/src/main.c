#include <asf.h>
#include "setup.h"

char buffer[100];
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
	printf("IRQ:%d:length=%d:%s",irqcount,i,buffer);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */ 

	setup_init();
	setup_configure();
	setup_enable();

	/* Insert application code here, after the board has been initialized. */
	// The main loop manages only the power mode
	// because the USB management is done by interrupt
	gfx_mono_draw_string("funny",10,10,&sysfont);
	while (true) {
		sleepmgr_enter_sleep();
	}
}
